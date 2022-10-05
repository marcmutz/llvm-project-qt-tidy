//===--- Port511SmartPtrToStdCompatibleApiCheck.cpp - clang-tidy ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PortToStdCompatibleApiCheck.h"
#include "../utils/OptionsUtils.h"
#include "../utils/TransformerClangTidyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/RewriteRule.h"
#include "clang/Tooling/Transformer/Stencil.h"
#include "llvm/ADT/StringRef.h"

using namespace ::clang::ast_matchers;
using namespace ::clang::transformer;

namespace clang {
namespace tidy {
namespace qt {

constexpr char QtTargetOptionKey[] = "QtTarget";
constexpr char ScopeOptionKey[] = "Scope";
constexpr char QtSmartPointerClassesOptionKey[] = "QtSmartPointerClasses";
constexpr char QtContainerClassesOptionKey[] = "QtContainerClasses";

constexpr char SmartPtrScope[] = "SmartPtr";
constexpr char ContainerScope[] = "Container";

constexpr int64_t DefaultQtTarget = 0x06'FF'FF; // means "lastest", for now
constexpr char DefaultScope[] = "";
constexpr char DefaultSmartPointerClasses[] = "all";
constexpr char DefaultContainerClasses[] = "all";

static bool all_classes(ArrayRef<StringRef> array) noexcept
{
    return array.empty() || (array.size() == 1 && array.front() == "all");
}

static std::vector<StringRef> normalizeSmartPointerClasses(std::vector<StringRef> classes)
{
    if (all_classes(classes)) {
        classes = {
            "QExplicitlySharedDataPointer",
            "QPointer",
            "QScopedPointer",
            "QSharedDataPointer",
            "QSharedPointer",
            "QWeakPointer",
        };
    }
    return std::move(classes);
}

static std::vector<StringRef> normalizeContainerClasses(std::vector<StringRef> classes)
{
    if (all_classes(classes)) {
        classes = {
            // views:
            "QAnyStringView",
            "QByteArrayView",
            "QLatin1String",
            "QLatin1StringView", // alias to QLatin1String, already the primary in Qt bootstrap builds
            "QStringView",
            "QUtf8StringView",
            "QBasicUtf8StringView", // template underlying QUtf8StringView
            // sequential:
            "QBitArray",
            "QByteArray",
            "QByteArrayList",
            "QContiguousCache",
            "QList",
            "QQueue",
            "QStack",
            "QString",
            "QStringList",
            "QVarLengthArray",
            "QVector",
            // associative:
            "QHash",
            "QMultiHash",
            "QMap",
            "QMultiMap",
            "QSet",
            // Qt has no QMultiSet
        };
    }
    return std::move(classes);
}

PortToStdCompatibleApiCheck::PortToStdCompatibleApiCheck(llvm::StringRef Name, ClangTidyContext *Context)
  : utils::TransformerClangTidyCheck(Name, Context),
    QtTargetOption(Options.get(QtTargetOptionKey, DefaultQtTarget)),
    ScopeOption(utils::options::parseStringList(
        Options.get(ScopeOptionKey, DefaultScope))),
    QtSmartPointerClassesOption(normalizeSmartPointerClasses(
        utils::options::parseStringList(
            Options.get(QtSmartPointerClassesOptionKey, DefaultSmartPointerClasses)))),
    QtContainerClassesOption(normalizeContainerClasses(
        utils::options::parseStringList(
            Options.get(QtContainerClassesOptionKey, DefaultContainerClasses))))
{
    const std::string o = "object";

    auto AnyOfQtClasses = [&](llvm::ArrayRef<llvm::StringRef> classes) {
        return anyOf(
            // TODO: the first one alone fails the v2() and v3() unit tests, find out why
            expr(hasType(cxxRecordDecl(isSameOrDerivedFrom(hasAnyName(classes))))).bind(o),
            expr(hasType(namedDecl(hasAnyName(classes)))).bind(o));
    };

    auto renameMethod = [&] (llvm::ArrayRef<llvm::StringRef> classes,
                            unsigned int arity,
                            llvm::StringRef from, llvm::StringRef to) {
        return makeRule(cxxMemberCallExpr(on(AnyOfQtClasses(classes)),
                            callee(cxxMethodDecl(hasName(from), parameterCountIs(arity)))),
                        changeTo(cat(access(o, cat(to)), "()")),
                        cat("use '", to, "' instead of '", from, "'"));
    };

    std::vector<RewriteRuleWith<std::string>> rules;

    if (scope(SmartPtrScope)) {
        if (qt(5, 11)) {
            rules.push_back(renameMethod(QtSmartPointerClassesOption, 0,
                                         "data", "get"));
        }
        if (qt(6, 5)) {
            rules.push_back(renameMethod(QtSmartPointerClassesOption, 0,
                                         "clear", "reset"));
        }
        if (qt(5, 4)) {
            rules.push_back(renameMethod(QtSmartPointerClassesOption, 0,
                                         "toStrongRef", "lock"));
        }
    }

    if (scope(ContainerScope)) {
        rules.push_back(renameMethod(QtContainerClassesOption, 0,
                        "count", "size"));
        rules.push_back(renameMethod(QtContainerClassesOption, 0,
                        "length", "size"));
    }

    setRule(applyFirst(rules));
}

PortToStdCompatibleApiCheck::~PortToStdCompatibleApiCheck()
  = default;

void PortToStdCompatibleApiCheck::storeOptions(
    ClangTidyOptions::OptionMap &Opts) {
  TransformerClangTidyCheck::storeOptions(Opts);
  Options.store(Opts, QtTargetOptionKey, QtTargetOption);
  Options.store(Opts, QtSmartPointerClassesOptionKey,
                utils::options::serializeStringList(QtSmartPointerClassesOption));
  Options.store(Opts, QtContainerClassesOptionKey,
                utils::options::serializeStringList(QtContainerClassesOption));
  Options.store(Opts, ScopeOptionKey,
                utils::options::serializeStringList(ScopeOption));
}

bool PortToStdCompatibleApiCheck::qt(uint8_t major, uint8_t minor) const noexcept
{
    return (major << 16 | minor << 8) <= QtTargetOption;
}

bool PortToStdCompatibleApiCheck::scope(llvm::StringRef name) const noexcept
{
    return ScopeOption.empty() || llvm::find(ScopeOption, name) != ScopeOption.cend();
}

} // namespace qt
} // namespace tidy
} // namespace clang
