//===-- EmulateInstructionMIPS.h ------------------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_SOURCE_PLUGINS_INSTRUCTION_MIPS_EMULATEINSTRUCTIONMIPS_H
#define LLDB_SOURCE_PLUGINS_INSTRUCTION_MIPS_EMULATEINSTRUCTIONMIPS_H

namespace llvm {
class MCDisassembler;
class MCSubtargetInfo;
class MCRegisterInfo;
class MCAsmInfo;
class MCContext;
class MCInstrInfo;
class MCInst;
}

namespace lldb_private {
  class OptionValueDictionary;
}

#include "lldb/Core/EmulateInstruction.h"
#include "lldb/Utility/Status.h"

class EmulateInstructionMIPS : public lldb_private::EmulateInstruction {
public:
  static void Initialize();

  static void Terminate();

  static llvm::StringRef GetPluginNameStatic() { return "mips32"; }

  static llvm::StringRef GetPluginDescriptionStatic();

  static lldb_private::EmulateInstruction *
  CreateInstance(const lldb_private::ArchSpec &arch,
                 lldb_private::InstructionType inst_type);

  static bool SupportsEmulatingInstructionsOfTypeStatic(
      lldb_private::InstructionType inst_type) {
    switch (inst_type) {
    case lldb_private::eInstructionTypeAny:
    case lldb_private::eInstructionTypePrologueEpilogue:
    case lldb_private::eInstructionTypePCModifying:
      return true;

    case lldb_private::eInstructionTypeAll:
      return false;
    }
    return false;
  }

  llvm::StringRef GetPluginName() override { return GetPluginNameStatic(); }

  bool SetTargetTriple(const lldb_private::ArchSpec &arch) override;

  EmulateInstructionMIPS(const lldb_private::ArchSpec &arch);

  bool SupportsEmulatingInstructionsOfType(
      lldb_private::InstructionType inst_type) override {
    return SupportsEmulatingInstructionsOfTypeStatic(inst_type);
  }

  bool ReadInstruction() override;

  bool EvaluateInstruction(uint32_t evaluate_options) override;

  bool SetInstruction(const lldb_private::Opcode &insn_opcode,
                      const lldb_private::Address &inst_addr,
                      lldb_private::Target *target) override;

  bool TestEmulation(lldb_private::Stream *out_stream,
                     lldb_private::ArchSpec &arch,
                     lldb_private::OptionValueDictionary *test_data) override {
    return false;
  }

  using EmulateInstruction::GetRegisterInfo;

  llvm::Optional<lldb_private::RegisterInfo>
  GetRegisterInfo(lldb::RegisterKind reg_kind, uint32_t reg_num) override;

  bool
  CreateFunctionEntryUnwind(lldb_private::UnwindPlan &unwind_plan) override;

protected:
  typedef struct {
    const char *op_name;
    bool (EmulateInstructionMIPS::*callback)(llvm::MCInst &insn);
    const char *insn_name;
  } MipsOpcode;

  static MipsOpcode *GetOpcodeForInstruction(llvm::StringRef name);

  uint32_t GetSizeOfInstruction(lldb_private::DataExtractor &data,
                                uint64_t inst_addr);

  bool Emulate_ADDiu(llvm::MCInst &insn);

  bool Emulate_SUBU_ADDU(llvm::MCInst &insn);

  bool Emulate_LUI(llvm::MCInst &insn);

  bool Emulate_SW(llvm::MCInst &insn);

  bool Emulate_LW(llvm::MCInst &insn);

  bool Emulate_ADDIUSP(llvm::MCInst &insn);

  bool Emulate_ADDIUS5(llvm::MCInst &insn);

  bool Emulate_SWSP(llvm::MCInst &insn);

  bool Emulate_SWM16_32(llvm::MCInst &insn);

  bool Emulate_LWSP(llvm::MCInst &insn);

  bool Emulate_LWM16_32(llvm::MCInst &insn);

  bool Emulate_JRADDIUSP(llvm::MCInst &insn);

  bool Emulate_LDST_Imm(llvm::MCInst &insn);

  bool Emulate_LDST_Reg(llvm::MCInst &insn);

  bool Emulate_BXX_3ops(llvm::MCInst &insn);

  bool Emulate_BXX_3ops_C(llvm::MCInst &insn);

  bool Emulate_BXX_2ops(llvm::MCInst &insn);

  bool Emulate_BXX_2ops_C(llvm::MCInst &insn);

  bool Emulate_Bcond_Link_C(llvm::MCInst &insn);

  bool Emulate_Bcond_Link(llvm::MCInst &insn);

  bool Emulate_FP_branch(llvm::MCInst &insn);

  bool Emulate_3D_branch(llvm::MCInst &insn);

  bool Emulate_BAL(llvm::MCInst &insn);

  bool Emulate_BALC(llvm::MCInst &insn);

  bool Emulate_BC(llvm::MCInst &insn);

  bool Emulate_J(llvm::MCInst &insn);

  bool Emulate_JAL(llvm::MCInst &insn);

  bool Emulate_JALR(llvm::MCInst &insn);

  bool Emulate_JIALC(llvm::MCInst &insn);

  bool Emulate_JIC(llvm::MCInst &insn);

  bool Emulate_JR(llvm::MCInst &insn);

  bool Emulate_BC1EQZ(llvm::MCInst &insn);

  bool Emulate_BC1NEZ(llvm::MCInst &insn);

  bool Emulate_BNZB(llvm::MCInst &insn);

  bool Emulate_BNZH(llvm::MCInst &insn);

  bool Emulate_BNZW(llvm::MCInst &insn);

  bool Emulate_BNZD(llvm::MCInst &insn);

  bool Emulate_BZB(llvm::MCInst &insn);

  bool Emulate_BZH(llvm::MCInst &insn);

  bool Emulate_BZW(llvm::MCInst &insn);

  bool Emulate_BZD(llvm::MCInst &insn);

  bool Emulate_MSA_Branch_DF(llvm::MCInst &insn, int element_byte_size,
                             bool bnz);

  bool Emulate_BNZV(llvm::MCInst &insn);

  bool Emulate_BZV(llvm::MCInst &insn);

  bool Emulate_MSA_Branch_V(llvm::MCInst &insn, bool bnz);

  bool Emulate_B16_MM(llvm::MCInst &insn);

  bool Emulate_Branch_MM(llvm::MCInst &insn);

  bool Emulate_JALRx16_MM(llvm::MCInst &insn);

  bool Emulate_JALx(llvm::MCInst &insn);

  bool Emulate_JALRS(llvm::MCInst &insn);

  bool nonvolatile_reg_p(uint32_t regnum);

  const char *GetRegisterName(unsigned reg_num, bool alternate_name);

private:
  std::unique_ptr<llvm::MCDisassembler> m_disasm;
  std::unique_ptr<llvm::MCDisassembler> m_alt_disasm;
  std::unique_ptr<llvm::MCSubtargetInfo> m_subtype_info;
  std::unique_ptr<llvm::MCSubtargetInfo> m_alt_subtype_info;
  std::unique_ptr<llvm::MCRegisterInfo> m_reg_info;
  std::unique_ptr<llvm::MCAsmInfo> m_asm_info;
  std::unique_ptr<llvm::MCContext> m_context;
  std::unique_ptr<llvm::MCInstrInfo> m_insn_info;
  uint32_t m_next_inst_size;
  bool m_use_alt_disaasm;
};

#endif // LLDB_SOURCE_PLUGINS_INSTRUCTION_MIPS_EMULATEINSTRUCTIONMIPS_H
