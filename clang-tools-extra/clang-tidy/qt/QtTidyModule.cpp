//===------- QtTidyModule.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "PortToStdCompatibleApiCheck.h"
#include "UseUnreachableReturnCheck.h"

namespace clang {
namespace tidy {
namespace qt {

class QtModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<PortToStdCompatibleApiCheck>(
        "qt-port-to-std-compatible-api");
    CheckFactories.registerCheck<UseUnreachableReturnCheck>(
        "qt-use-unreachable-return");
  }
};

// Register the QtModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<QtModule> X("qt-module",
                                                   "Add Qt checks.");

} // namespace qt

// This anchor is used to force the linker to link in the generated object file
// and thus register the QtModule.
volatile int QtModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
