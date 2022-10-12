//===--- UseUnreachableReturnCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_QT_USEUNREACHABLERETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_QT_USEUNREACHABLERETURNCHECK_H

#include "../utils/TransformerClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace qt {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/qt/port-65-unreachable-return.html
class UseUnreachableReturnCheck : public utils::TransformerClangTidyCheck {
public:
  UseUnreachableReturnCheck(StringRef Name, ClangTidyContext *Context);
  ~UseUnreachableReturnCheck() override;
};

} // namespace qt
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_QT_USEUNREACHABLERETURNCHECK_H
