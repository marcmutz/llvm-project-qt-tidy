//===--- UseUnreachableReturnCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseUnreachableReturnCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/RewriteRule.h"
#include "clang/Tooling/Transformer/Stencil.h"

using namespace ::clang::ast_matchers;
using namespace ::clang::ast_matchers::internal;
using namespace ::clang::transformer;

namespace clang {
namespace {
/// Matches a Stmt whose parent is a CompoundStmt, and which is directly
/// followed by a Stmt matching the inner matcher.
AST_MATCHER_P(Stmt, nextStmt, Matcher<Stmt>, InnerMatcher) {
  // Duplicated from UseAnyOfAllOfCheck.cpp...
  DynTypedNodeList Parents = Finder->getASTContext().getParents(Node);
  if (Parents.size() != 1)
    return false;

  auto *C = Parents[0].get<CompoundStmt>();
  if (!C)
    return false;

  const auto *I = llvm::find(C->body(), &Node);
  assert(I != C->body_end() && "C is parent of Node");
  if (++I == C->body_end())
    return false; // Node is last statement.

  return InnerMatcher.matches(**I, Finder, Builder);
}

AST_MATCHER_P(SwitchCase, subStmt, Matcher<Stmt>, InnerMatcher) {
    auto *Sub = Node.getSubStmt();
    return Sub && InnerMatcher.matches(*Sub, Finder, Builder);
}
} // namespace

namespace tidy {
namespace qt {

UseUnreachableReturnCheck::UseUnreachableReturnCheck(llvm::StringRef Name, ClangTidyContext *Context)
    : utils::TransformerClangTidyCheck(Name, Context)
{
    const std::string unr = "unr", val = "val", ret = "ret";
    auto makeUnreachableReturn = cat("Q_UNREACHABLE_RETURN(",
                                    ifBound(val, cat(node(val)), cat("")),
                                    ")");
    auto ignoringSwitchCases = [](auto stmt) {
        return anyOf(stmt, switchCase(subStmt(stmt)));
    };
    setRule(
        makeRule(stmt(ignoringSwitchCases(stmt(isExpandedFromMacro("Q_UNREACHABLE")).bind(unr)),
                      nextStmt(returnStmt(optionally(hasReturnValue(expr().bind(val)))).bind(ret))),
                 {changeTo(node(unr), cat(makeUnreachableReturn, ";")), // TODO: why is the ; lost w/o this?
                  changeTo(node(ret), cat(""))},
                 cat("use ", makeUnreachableReturn))
    );
}

UseUnreachableReturnCheck::~UseUnreachableReturnCheck()
    = default;

} // namespace qt
} // namespace tidy
} // namespace clang
