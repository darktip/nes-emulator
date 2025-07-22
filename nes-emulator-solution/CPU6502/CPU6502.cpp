#include "CPU6502.h"

#include "../Utils/address_utils.h"

void CPU6502::resetInternal()
{
    a_reg = 0x00;
    x_reg = 0x00;
    y_reg = 0x00;
    pc_reg = 0x0000;
    sp_reg = 0x00;
    status_reg = 0x00;

    fetch = 0x00;
    opCycles = 0;
    cyclesCount = 0;
}

void CPU6502::initializeOpcodes()
{
    // Row 0x
    opcodes[0x00] = Instruction{"BRK (IMP)", &CPU6502::IMP, &CPU6502::BRK};
    opcodes[0x01] = Instruction{"ORA (INX)", &CPU6502::INX, &CPU6502::ORA};
    opcodes[0x02] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x03] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x04] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x05] = Instruction{"ORA (ZPG)", &CPU6502::ZPG, &CPU6502::ORA};
    opcodes[0x06] = Instruction{"ASL (ZPG)", &CPU6502::ZPG, &CPU6502::ASL};
    opcodes[0x07] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x08] = Instruction{"PHP (IMP)", &CPU6502::IMP, &CPU6502::PHP};
    opcodes[0x09] = Instruction{"ORA (IMM)", &CPU6502::IMM, &CPU6502::ORA};
    opcodes[0x0A] = Instruction{"ASL (ACC)", &CPU6502::ACC, &CPU6502::ASL};
    opcodes[0x0B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x0C] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x0D] = Instruction{"ORA (ABS)", &CPU6502::ABS, &CPU6502::ORA};
    opcodes[0x0E] = Instruction{"ASL (ABS)", &CPU6502::ABS, &CPU6502::ASL};
    opcodes[0x0F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 1x
    opcodes[0x10] = Instruction{"BPL (REL)", &CPU6502::REL, &CPU6502::BPL};
    opcodes[0x11] = Instruction{"ORA (INY)", &CPU6502::INY, &CPU6502::ORA};
    opcodes[0x12] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x13] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x14] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x15] = Instruction{"ORA (ZPX)", &CPU6502::ZPX, &CPU6502::ORA};
    opcodes[0x16] = Instruction{"ASL (ZPX)", &CPU6502::ZPX, &CPU6502::ASL};
    opcodes[0x17] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x18] = Instruction{"CLC (IMP)", &CPU6502::IMP, &CPU6502::CLC};
    opcodes[0x19] = Instruction{"ORA (ABY)", &CPU6502::ABY, &CPU6502::ORA};
    opcodes[0x1A] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x1B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x1C] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x1D] = Instruction{"ORA (ABX)", &CPU6502::ABX, &CPU6502::ORA};
    opcodes[0x1E] = Instruction{"ASL (ABX)", &CPU6502::ABX, &CPU6502::ASL};
    opcodes[0x1F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 2x
    opcodes[0x20] = Instruction{"JSR (ABS)", &CPU6502::ABS, &CPU6502::JSR};
    opcodes[0x21] = Instruction{"AND (INX)", &CPU6502::INX, &CPU6502::AND};
    opcodes[0x22] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x23] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x24] = Instruction{"BIT (ZPG)", &CPU6502::ZPG, &CPU6502::BIT};
    opcodes[0x25] = Instruction{"AND (ZPG)", &CPU6502::ZPG, &CPU6502::AND};
    opcodes[0x26] = Instruction{"ROL (ZPG)", &CPU6502::ZPG, &CPU6502::ROL};
    opcodes[0x27] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x28] = Instruction{"PLP (IMP)", &CPU6502::IMP, &CPU6502::PLP};
    opcodes[0x29] = Instruction{"AND (IMM)", &CPU6502::IMM, &CPU6502::AND};
    opcodes[0x2A] = Instruction{"ROL (ACC)", &CPU6502::ACC, &CPU6502::ROL};
    opcodes[0x2B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x2C] = Instruction{"BIT (ABS)", &CPU6502::ABS, &CPU6502::BIT};
    opcodes[0x2D] = Instruction{"AND (ABS)", &CPU6502::ABS, &CPU6502::AND};
    opcodes[0x2E] = Instruction{"ROL (ABS)", &CPU6502::ABS, &CPU6502::ROL};
    opcodes[0x2F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 3x
    opcodes[0x30] = Instruction{"BMI (REL)", &CPU6502::REL, &CPU6502::BMI};
    opcodes[0x31] = Instruction{"AND (INY)", &CPU6502::INY, &CPU6502::AND};
    opcodes[0x32] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x33] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x34] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x35] = Instruction{"AND (ZPX)", &CPU6502::ZPX, &CPU6502::AND};
    opcodes[0x36] = Instruction{"ROL (ZPX)", &CPU6502::ZPX, &CPU6502::ROL};
    opcodes[0x37] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x38] = Instruction{"SEC (IMP)", &CPU6502::IMP, &CPU6502::SEC};
    opcodes[0x39] = Instruction{"AND (ABY)", &CPU6502::ABY, &CPU6502::AND};
    opcodes[0x3A] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x3B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x3C] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x3D] = Instruction{"AND (ABX)", &CPU6502::ABX, &CPU6502::AND};
    opcodes[0x3E] = Instruction{"ROL (ABX)", &CPU6502::ABX, &CPU6502::ROL};
    opcodes[0x3F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 4x
    opcodes[0x40] = Instruction{"RTI (IMP)", &CPU6502::IMP, &CPU6502::RTI};
    opcodes[0x41] = Instruction{"EOR (INX)", &CPU6502::INX, &CPU6502::EOR};
    opcodes[0x42] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x43] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x44] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x45] = Instruction{"EOR (ZPG)", &CPU6502::ZPG, &CPU6502::EOR};
    opcodes[0x46] = Instruction{"LSR (ZPG)", &CPU6502::ZPG, &CPU6502::LSR};
    opcodes[0x47] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x48] = Instruction{"PHA (IMP)", &CPU6502::IMP, &CPU6502::PHA};
    opcodes[0x49] = Instruction{"EOR (IMM)", &CPU6502::IMM, &CPU6502::EOR};
    opcodes[0x4A] = Instruction{"LSR (ACC)", &CPU6502::ACC, &CPU6502::LSR};
    opcodes[0x4B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x4C] = Instruction{"JMP (ABS)", &CPU6502::ABS, &CPU6502::JMP};
    opcodes[0x4D] = Instruction{"EOR (ABS)", &CPU6502::ABS, &CPU6502::EOR};
    opcodes[0x4E] = Instruction{"LSR (ABS)", &CPU6502::ABS, &CPU6502::LSR};
    opcodes[0x4F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 5x
    opcodes[0x50] = Instruction{"BVC (REL)", &CPU6502::REL, &CPU6502::BVC};
    opcodes[0x51] = Instruction{"EOR (INY)", &CPU6502::INY, &CPU6502::EOR};
    opcodes[0x52] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x53] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x54] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x55] = Instruction{"EOR (ZPX)", &CPU6502::ZPX, &CPU6502::EOR};
    opcodes[0x56] = Instruction{"LSR (ZPX)", &CPU6502::ZPX, &CPU6502::LSR};
    opcodes[0x57] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x58] = Instruction{"CLI (IMP)", &CPU6502::IMP, &CPU6502::CLI};
    opcodes[0x59] = Instruction{"EOR (ABY)", &CPU6502::ABY, &CPU6502::EOR};
    opcodes[0x5A] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x5B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x5C] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x5D] = Instruction{"EOR (ABX)", &CPU6502::ABX, &CPU6502::EOR};
    opcodes[0x5E] = Instruction{"LSR (ABX)", &CPU6502::ABX, &CPU6502::LSR};
    opcodes[0x5F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 6x
    opcodes[0x60] = Instruction{"RTS (IMP)", &CPU6502::IMP, &CPU6502::RTS};
    opcodes[0x61] = Instruction{"ADC (INX)", &CPU6502::INX, &CPU6502::ADC};
    opcodes[0x62] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x63] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x64] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x65] = Instruction{"ADC (ZPG)", &CPU6502::ZPG, &CPU6502::ADC};
    opcodes[0x66] = Instruction{"ROR (ZPG)", &CPU6502::ZPG, &CPU6502::ROR};
    opcodes[0x67] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x68] = Instruction{"PLA (IMP)", &CPU6502::IMP, &CPU6502::PLA};
    opcodes[0x69] = Instruction{"ADC (IMM)", &CPU6502::IMM, &CPU6502::ADC};
    opcodes[0x6A] = Instruction{"ROR (ACC)", &CPU6502::ACC, &CPU6502::ROR};
    opcodes[0x6B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x6C] = Instruction{"JMP (IND)", &CPU6502::IND, &CPU6502::JMP};
    opcodes[0x6D] = Instruction{"ADC (ABS)", &CPU6502::ABS, &CPU6502::ADC};
    opcodes[0x6E] = Instruction{"ROR (ABS)", &CPU6502::ABS, &CPU6502::ROR};
    opcodes[0x6F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 7x
    opcodes[0x70] = Instruction{"BVS (REL)", &CPU6502::REL, &CPU6502::BVS};
    opcodes[0x71] = Instruction{"ADC (INY)", &CPU6502::INY, &CPU6502::ADC};
    opcodes[0x72] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x73] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x74] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x75] = Instruction{"ADC (ZPX)", &CPU6502::ZPX, &CPU6502::ADC};
    opcodes[0x76] = Instruction{"ROR (ZPX)", &CPU6502::ZPX, &CPU6502::ROR};
    opcodes[0x77] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x78] = Instruction{"SEI (IMP)", &CPU6502::IMP, &CPU6502::SEI};
    opcodes[0x79] = Instruction{"ADC (ABY)", &CPU6502::ABY, &CPU6502::ADC};
    opcodes[0x7A] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x7B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x7C] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x7D] = Instruction{"ADC (ABX)", &CPU6502::ABX, &CPU6502::ADC};
    opcodes[0x7E] = Instruction{"ROR (ABX)", &CPU6502::ABX, &CPU6502::ROR};
    opcodes[0x7F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 8x
    opcodes[0x80] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x81] = Instruction{"STA (INX)", &CPU6502::INX, &CPU6502::STA};
    opcodes[0x82] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x83] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x84] = Instruction{"STY (ZPG)", &CPU6502::ZPG, &CPU6502::STY};
    opcodes[0x85] = Instruction{"STA (ZPG)", &CPU6502::ZPG, &CPU6502::STA};
    opcodes[0x86] = Instruction{"STX (ZPG)", &CPU6502::ZPG, &CPU6502::STX};
    opcodes[0x87] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x88] = Instruction{"DEY (IMP)", &CPU6502::IMP, &CPU6502::DEY};
    opcodes[0x89] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x8A] = Instruction{"TXA (IMP)", &CPU6502::IMP, &CPU6502::TXA};
    opcodes[0x8B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x8C] = Instruction{"STY (ABS)", &CPU6502::ABS, &CPU6502::STY};
    opcodes[0x8D] = Instruction{"STA (ABS)", &CPU6502::ABS, &CPU6502::STA};
    opcodes[0x8E] = Instruction{"STX (ABS)", &CPU6502::ABS, &CPU6502::STX};
    opcodes[0x8F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row 9x
    opcodes[0x90] = Instruction{"BCC (REL)", &CPU6502::REL, &CPU6502::BCC};
    opcodes[0x91] = Instruction{"STA (INY)", &CPU6502::INY, &CPU6502::STA};
    opcodes[0x92] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x93] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x94] = Instruction{"STY (ZPX)", &CPU6502::ZPX, &CPU6502::STY};
    opcodes[0x95] = Instruction{"STA (ZPX)", &CPU6502::ZPX, &CPU6502::STA};
    opcodes[0x96] = Instruction{"STX (ZPY)", &CPU6502::ZPY, &CPU6502::STX};
    opcodes[0x97] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x98] = Instruction{"TYA (IMP)", &CPU6502::IMP, &CPU6502::TYA};
    opcodes[0x99] = Instruction{"STA (ABY)", &CPU6502::ABY, &CPU6502::STA};
    opcodes[0x9A] = Instruction{"TXS (IMP)", &CPU6502::IMP, &CPU6502::TXS};
    opcodes[0x9B] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x9C] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x9D] = Instruction{"STA (ABX)", &CPU6502::ABX, &CPU6502::STA};
    opcodes[0x9E] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0x9F] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row Ax
    opcodes[0xA0] = Instruction{"LDY (IMM)", &CPU6502::IMM, &CPU6502::LDY};
    opcodes[0xA1] = Instruction{"LDA (INX)", &CPU6502::INX, &CPU6502::LDA};
    opcodes[0xA2] = Instruction{"LDX (IMM)", &CPU6502::IMM, &CPU6502::LDX};
    opcodes[0xA3] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xA4] = Instruction{"LDY (ZPG)", &CPU6502::ZPG, &CPU6502::LDY};
    opcodes[0xA5] = Instruction{"LDA (ZPG)", &CPU6502::ZPG, &CPU6502::LDA};
    opcodes[0xA6] = Instruction{"LDX (ZPG)", &CPU6502::ZPG, &CPU6502::LDX};
    opcodes[0xA7] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xA8] = Instruction{"TAY (IMP)", &CPU6502::IMP, &CPU6502::TAY};
    opcodes[0xA9] = Instruction{"LDA (IMM)", &CPU6502::IMM, &CPU6502::LDA};
    opcodes[0xAA] = Instruction{"TAX (IMP)", &CPU6502::IMP, &CPU6502::TAX};
    opcodes[0xAB] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xAC] = Instruction{"LDY (ABS)", &CPU6502::ABS, &CPU6502::LDY};
    opcodes[0xAD] = Instruction{"LDA (ABS)", &CPU6502::ABS, &CPU6502::LDA};
    opcodes[0xAE] = Instruction{"LDX (ABS)", &CPU6502::ABS, &CPU6502::LDX};
    opcodes[0xAF] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row Bx
    opcodes[0xB0] = Instruction{"BCS (REL)", &CPU6502::REL, &CPU6502::BCS};
    opcodes[0xB1] = Instruction{"LDA (INY)", &CPU6502::INY, &CPU6502::LDA};
    opcodes[0xB2] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xB3] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xB4] = Instruction{"LDY (ZPX)", &CPU6502::ZPX, &CPU6502::LDY};
    opcodes[0xB5] = Instruction{"LDA (ZPX)", &CPU6502::ZPX, &CPU6502::LDA};
    opcodes[0xB6] = Instruction{"LDX (ZPY)", &CPU6502::ZPY, &CPU6502::LDX};
    opcodes[0xB7] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xB8] = Instruction{"CLV (IMP)", &CPU6502::IMP, &CPU6502::CLV};
    opcodes[0xB9] = Instruction{"LDA (ABY)", &CPU6502::ABY, &CPU6502::LDA};
    opcodes[0xBA] = Instruction{"TSX (IMP)", &CPU6502::IMP, &CPU6502::TSX};
    opcodes[0xBB] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xBC] = Instruction{"LDY (ABX)", &CPU6502::ABX, &CPU6502::LDY};
    opcodes[0xBD] = Instruction{"LDA (ABX)", &CPU6502::ABX, &CPU6502::LDA};
    opcodes[0xBE] = Instruction{"LDX (ABY)", &CPU6502::ABY, &CPU6502::LDX};
    opcodes[0xBF] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row Cx
    opcodes[0xC0] = Instruction{"CPY (IMM)", &CPU6502::IMM, &CPU6502::CPY};
    opcodes[0xC1] = Instruction{"CMP (INX)", &CPU6502::INX, &CPU6502::CMP};
    opcodes[0xC2] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xC3] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xC4] = Instruction{"CPY (ZPG)", &CPU6502::ZPG, &CPU6502::CPY};
    opcodes[0xC5] = Instruction{"CMP (ZPG)", &CPU6502::ZPG, &CPU6502::CMP};
    opcodes[0xC6] = Instruction{"DEC (ZPG)", &CPU6502::ZPG, &CPU6502::DEC};
    opcodes[0xC7] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xC8] = Instruction{"INY (IMP)", &CPU6502::IMP, &CPU6502::INY};
    opcodes[0xC9] = Instruction{"CMP (IMM)", &CPU6502::IMM, &CPU6502::CMP};
    opcodes[0xCA] = Instruction{"DEX (IMP)", &CPU6502::IMP, &CPU6502::DEX};
    opcodes[0xCB] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xCC] = Instruction{"CPY (ABS)", &CPU6502::ABS, &CPU6502::CPY};
    opcodes[0xCD] = Instruction{"CMP (ABS)", &CPU6502::ABS, &CPU6502::CMP};
    opcodes[0xCE] = Instruction{"DEC (ABS)", &CPU6502::ABS, &CPU6502::DEC};
    opcodes[0xCF] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row Dx
    opcodes[0xD0] = Instruction{"BNE (REL)", &CPU6502::REL, &CPU6502::BNE};
    opcodes[0xD1] = Instruction{"CMP (INY)", &CPU6502::INY, &CPU6502::CMP};
    opcodes[0xD2] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xD3] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xD4] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xD5] = Instruction{"CMP (ZPX)", &CPU6502::ZPX, &CPU6502::CMP};
    opcodes[0xD6] = Instruction{"DEC (ZPX)", &CPU6502::ZPX, &CPU6502::DEC};
    opcodes[0xD7] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xD8] = Instruction{"CLD (IMP)", &CPU6502::IMP, &CPU6502::CLD};
    opcodes[0xD9] = Instruction{"CMP (ABY)", &CPU6502::ABY, &CPU6502::CMP};
    opcodes[0xDA] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xDB] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xDC] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xDD] = Instruction{"CMP (ABX)", &CPU6502::ABX, &CPU6502::CMP};
    opcodes[0xDE] = Instruction{"DEC (ABX)", &CPU6502::ABX, &CPU6502::DEC};
    opcodes[0xDF] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row Ex
    opcodes[0xE0] = Instruction{"CPX (IMM)", &CPU6502::IMM, &CPU6502::CPX};
    opcodes[0xE1] = Instruction{"SBC (INX)", &CPU6502::INX, &CPU6502::SBC};
    opcodes[0xE2] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xE3] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xE4] = Instruction{"CPX (ZPG)", &CPU6502::ZPG, &CPU6502::CPX};
    opcodes[0xE5] = Instruction{"SBC (ZPG)", &CPU6502::ZPG, &CPU6502::SBC};
    opcodes[0xE6] = Instruction{"INC (ZPG)", &CPU6502::ZPG, &CPU6502::INC};
    opcodes[0xE7] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xE8] = Instruction{"INX (IMP)", &CPU6502::IMP, &CPU6502::INX};
    opcodes[0xE9] = Instruction{"SBC (IMM)", &CPU6502::IMM, &CPU6502::SBC};
    opcodes[0xEA] = Instruction{"NOP (IMP)", &CPU6502::IMP, &CPU6502::NOP};
    opcodes[0xEB] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xEC] = Instruction{"CPX (ABS)", &CPU6502::ABS, &CPU6502::CPX};
    opcodes[0xED] = Instruction{"SBC (ABS)", &CPU6502::ABS, &CPU6502::SBC};
    opcodes[0xEE] = Instruction{"INC (ABS)", &CPU6502::ABS, &CPU6502::INC};
    opcodes[0xEF] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    // Row Fx
    opcodes[0xF0] = Instruction{"BEQ (REL)", &CPU6502::REL, &CPU6502::BEQ};
    opcodes[0xF1] = Instruction{"SBC (INY)", &CPU6502::INY, &CPU6502::SBC};
    opcodes[0xF2] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xF3] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xF4] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xF5] = Instruction{"SBC (ZPX)", &CPU6502::ZPX, &CPU6502::SBC};
    opcodes[0xF6] = Instruction{"INC (ZPX)", &CPU6502::ZPX, &CPU6502::INC};
    opcodes[0xF7] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xF8] = Instruction{"SED (IMP)", &CPU6502::IMP, &CPU6502::SED};
    opcodes[0xF9] = Instruction{"SBC (ABY)", &CPU6502::ABY, &CPU6502::SBC};
    opcodes[0xFA] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xFB] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xFC] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
    opcodes[0xFD] = Instruction{"SBC (ABX)", &CPU6502::ABX, &CPU6502::SBC};
    opcodes[0xFE] = Instruction{"INC (ABX)", &CPU6502::ABX, &CPU6502::INC};
    opcodes[0xFF] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL};
}

CPU6502::CPU6502()
{
    resetInternal();
    initializeOpcodes();
}

CPU6502::~CPU6502() = default;

void CPU6502::write(uint16_t addr, uint8_t val)
{
    bus->write(addr, val);
}

uint8_t CPU6502::read(uint16_t addr) const
{
    return bus->read(addr);
}

void CPU6502::setStatusFlag(StatusRegistryFlags flag, bool predicate)
{
    if (predicate)
    {
        status_reg |= flag;
    }
    else
    {
        status_reg &= ~flag;
    }
}

uint8_t CPU6502::getStatusFlag(StatusRegistryFlags flag)
{
    return status_reg & flag;
}

uint16_t CPU6502::ACC(uint8_t& cycles)
{
    uint16_t addr = a_reg;
    return addr;
}

uint16_t CPU6502::ABS(uint8_t& cycles)
{
    uint16_t addr = readFullAddressWithIncrementPC(*bus, pc_reg);
    return addr;
}

uint16_t CPU6502::ABX(uint8_t& cycles)
{
    uint16_t addr = readFullAddressWithIncrementPC(*bus, pc_reg);
    uint16_t shifted_addr = addr + x_reg;
    if (!isAddressOnSamePage(addr, shifted_addr))
    {
        cycles++;
    }
    return shifted_addr;
}

uint16_t CPU6502::ABY(uint8_t& cycles)
{
    uint16_t addr = readFullAddressWithIncrementPC(*bus, pc_reg);
    uint16_t shifted_addr = addr + y_reg;
    if (!isAddressOnSamePage(addr, shifted_addr))
    {
        cycles++;
    }
    return shifted_addr;
}

uint16_t CPU6502::IMM(uint8_t& cycles)
{
    uint16_t addr = pc_reg++;
    return addr;
}

uint16_t CPU6502::IMP(uint8_t& cycles)
{
    return 0;
}

uint16_t CPU6502::IND(uint8_t& cycles)
{
    uint16_t pointer_addr = readFullAddressWithIncrementPC(*bus, pc_reg);
    if ((pointer_addr & LO_BYTE_MASK) == LO_BYTE_MASK) // Simulate bug when addressing 0x--FF
    {
        uint16_t lo = read(pointer_addr);
        uint16_t hi = read(pointer_addr & HI_BYTE_MASK);
        uint16_t addr = static_cast<uint16_t>(hi << HI_BYTE_SHIFT) | lo;
        return addr;
    }
    else
    {
        uint16_t addr = readFullAddress(*bus, pointer_addr);
        return addr;   
    }
}

uint16_t CPU6502::INX(uint8_t& cycles)
{
    uint16_t zp_pointer = read(pc_reg++);
    uint16_t x = x_reg;
    uint16_t lo = read((zp_pointer + x) & LO_BYTE_MASK);
    uint16_t hi = read((zp_pointer + x + 1) & LO_BYTE_MASK);
    uint16_t addr = static_cast<uint16_t>(hi << HI_BYTE_SHIFT) | lo;
    return addr;
}

uint16_t CPU6502::INY(uint8_t& cycles)
{
    uint16_t zp_pointer = read(pc_reg++);
    uint16_t addr = readFullAddress(*bus, zp_pointer);
    uint16_t shifted_addr = addr + y_reg;
    if (!isAddressOnSamePage(addr, shifted_addr))
    {
        cycles++;
    }
    return shifted_addr;
}

uint16_t CPU6502::REL(uint8_t& cycles)
{
    return 0;
}
