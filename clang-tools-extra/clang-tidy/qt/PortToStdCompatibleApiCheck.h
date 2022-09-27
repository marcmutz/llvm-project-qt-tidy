//===--- PortToStdCompatibleApiCheck.h - clang-tidy --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_QT_PORTTOSTDCOMPATIBLEAPICHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_QT_PORTTOSTDCOMPATIBLEAPICHECK_H

#include "../utils/TransformerClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace qt {

/// Ports uses of Qt's duplicate, non-std-compatible API subset to the
/// std-compatible API subset.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/qt/port-to-std-compatible-api.html
class PortToStdCompatibleApiCheck : public utils::TransformerClangTidyCheck {
public:
  PortToStdCompatibleApiCheck(StringRef Name, ClangTidyContext *Context);
  ~PortToStdCompatibleApiCheck() override;

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  bool qt(uint8_t major, uint8_t minor = 0) const noexcept;
  bool scope(StringRef name) const noexcept;

  const int64_t QtTargetOption; // 0xMMmmPP format (MM - major, mm - minor, PP - patch)
  const std::vector<StringRef> ScopeOption;
  const std::vector<StringRef> QtSmartPointerClassesOption;
};

} // namespace qt
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_QT_PORTTOSTDCOMPATIBLEAPICHECK_H
