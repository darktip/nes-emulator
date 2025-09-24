#include "CPU6502.h"

#include "../Utils/address_utils.h"
#include "../Utils/bitwise_utils.h"

using namespace nes;

void CPU6502::resetInternal()
{
    a_reg = 0x00;
    x_reg = 0x00;
    y_reg = 0x00;
    pc_reg = 0x0000;
    sp_reg = 0x00;
    status_reg = 0x00;

    impliedFlag = false;
    impliedValue = 0x00;
    opCyclesCount = 0;
    globalCyclesCount = 0;
}

void CPU6502::initializeOpcodes()
{
    for (int opcode = 0x00; opcode <= 0xFF; ++opcode) {
        opcodes[opcode] = Instruction{"ILL (IMP)", &CPU6502::IMP, &CPU6502::ILL, 1};
    }

    // Row 0x
    opcodes[0x00] = Instruction{"BRK (IMP)", &CPU6502::IMP, &CPU6502::BRK, 7};
    opcodes[0x01] = Instruction{"ORA (INX)", &CPU6502::INX, &CPU6502::ORA, 6};
    opcodes[0x05] = Instruction{"ORA (ZPG)", &CPU6502::ZPG, &CPU6502::ORA, 3};
    opcodes[0x06] = Instruction{"ASL (ZPG)", &CPU6502::ZPG, &CPU6502::ASL, 5};
    opcodes[0x08] = Instruction{"PHP (IMP)", &CPU6502::IMP, &CPU6502::PHP, 3};
    opcodes[0x09] = Instruction{"ORA (IMM)", &CPU6502::IMM, &CPU6502::ORA, 2};
    opcodes[0x0A] = Instruction{"ASL (ACC)", &CPU6502::ACC, &CPU6502::ASL, 2};
    opcodes[0x0D] = Instruction{"ORA (ABS)", &CPU6502::ABS, &CPU6502::ORA, 4};
    opcodes[0x0E] = Instruction{"ASL (ABS)", &CPU6502::ABS, &CPU6502::ASL, 6};

    // Row 1x
    opcodes[0x10] = Instruction{"BPL (REL)", &CPU6502::REL, &CPU6502::BPL, 2};
    opcodes[0x11] = Instruction{"ORA (INY)", &CPU6502::INY, &CPU6502::ORA, 5};
    opcodes[0x15] = Instruction{"ORA (ZPX)", &CPU6502::ZPX, &CPU6502::ORA, 4};
    opcodes[0x16] = Instruction{"ASL (ZPX)", &CPU6502::ZPX, &CPU6502::ASL, 6};
    opcodes[0x18] = Instruction{"CLC (IMP)", &CPU6502::IMP, &CPU6502::CLC, 2};
    opcodes[0x19] = Instruction{"ORA (ABY)", &CPU6502::ABY, &CPU6502::ORA, 4};
    opcodes[0x1D] = Instruction{"ORA (ABX)", &CPU6502::ABX, &CPU6502::ORA, 4};
    opcodes[0x1E] = Instruction{"ASL (ABX)", &CPU6502::ABX, &CPU6502::ASL, 7};

    // Row 2x
    opcodes[0x20] = Instruction{"JSR (ABS)", &CPU6502::ABS, &CPU6502::JSR, 6};
    opcodes[0x21] = Instruction{"AND (INX)", &CPU6502::INX, &CPU6502::AND, 6};
    opcodes[0x24] = Instruction{"BIT (ZPG)", &CPU6502::ZPG, &CPU6502::BIT, 3};
    opcodes[0x25] = Instruction{"AND (ZPG)", &CPU6502::ZPG, &CPU6502::AND, 3};
    opcodes[0x26] = Instruction{"ROL (ZPG)", &CPU6502::ZPG, &CPU6502::ROL, 5};
    opcodes[0x28] = Instruction{"PLP (IMP)", &CPU6502::IMP, &CPU6502::PLP, 4};
    opcodes[0x29] = Instruction{"AND (IMM)", &CPU6502::IMM, &CPU6502::AND, 2};
    opcodes[0x2A] = Instruction{"ROL (ACC)", &CPU6502::ACC, &CPU6502::ROL, 2};
    opcodes[0x2C] = Instruction{"BIT (ABS)", &CPU6502::ABS, &CPU6502::BIT, 4};
    opcodes[0x2D] = Instruction{"AND (ABS)", &CPU6502::ABS, &CPU6502::AND, 4};
    opcodes[0x2E] = Instruction{"ROL (ABS)", &CPU6502::ABS, &CPU6502::ROL, 6};

    // Row 3x
    opcodes[0x30] = Instruction{"BMI (REL)", &CPU6502::REL, &CPU6502::BMI, 2};
    opcodes[0x31] = Instruction{"AND (INY)", &CPU6502::INY, &CPU6502::AND, 5};
    opcodes[0x35] = Instruction{"AND (ZPX)", &CPU6502::ZPX, &CPU6502::AND, 4};
    opcodes[0x36] = Instruction{"ROL (ZPX)", &CPU6502::ZPX, &CPU6502::ROL, 6};
    opcodes[0x38] = Instruction{"SEC (IMP)", &CPU6502::IMP, &CPU6502::SEC, 2};
    opcodes[0x39] = Instruction{"AND (ABY)", &CPU6502::ABY, &CPU6502::AND, 4};
    opcodes[0x3D] = Instruction{"AND (ABX)", &CPU6502::ABX, &CPU6502::AND, 4};
    opcodes[0x3E] = Instruction{"ROL (ABX)", &CPU6502::ABX, &CPU6502::ROL, 7};

    // Row 4x
    opcodes[0x40] = Instruction{"RTI (IMP)", &CPU6502::IMP, &CPU6502::RTI, 6};
    opcodes[0x41] = Instruction{"EOR (INX)", &CPU6502::INX, &CPU6502::EOR, 6};
    opcodes[0x45] = Instruction{"EOR (ZPG)", &CPU6502::ZPG, &CPU6502::EOR, 3};
    opcodes[0x46] = Instruction{"LSR (ZPG)", &CPU6502::ZPG, &CPU6502::LSR, 5};
    opcodes[0x48] = Instruction{"PHA (IMP)", &CPU6502::IMP, &CPU6502::PHA, 3};
    opcodes[0x49] = Instruction{"EOR (IMM)", &CPU6502::IMM, &CPU6502::EOR, 2};
    opcodes[0x4A] = Instruction{"LSR (ACC)", &CPU6502::ACC, &CPU6502::LSR, 2};
    opcodes[0x4C] = Instruction{"JMP (ABS)", &CPU6502::ABS, &CPU6502::JMP, 3};
    opcodes[0x4D] = Instruction{"EOR (ABS)", &CPU6502::ABS, &CPU6502::EOR, 4};
    opcodes[0x4E] = Instruction{"LSR (ABS)", &CPU6502::ABS, &CPU6502::LSR, 6};

    // Row 5x
    opcodes[0x50] = Instruction{"BVC (REL)", &CPU6502::REL, &CPU6502::BVC, 2};
    opcodes[0x51] = Instruction{"EOR (INY)", &CPU6502::INY, &CPU6502::EOR, 5};
    opcodes[0x55] = Instruction{"EOR (ZPX)", &CPU6502::ZPX, &CPU6502::EOR, 4};
    opcodes[0x56] = Instruction{"LSR (ZPX)", &CPU6502::ZPX, &CPU6502::LSR, 6};
    opcodes[0x58] = Instruction{"CLI (IMP)", &CPU6502::IMP, &CPU6502::CLI, 2};
    opcodes[0x59] = Instruction{"EOR (ABY)", &CPU6502::ABY, &CPU6502::EOR, 4};
    opcodes[0x5D] = Instruction{"EOR (ABX)", &CPU6502::ABX, &CPU6502::EOR, 4};
    opcodes[0x5E] = Instruction{"LSR (ABX)", &CPU6502::ABX, &CPU6502::LSR, 7};

    // Row 6x
    opcodes[0x60] = Instruction{"RTS (IMP)", &CPU6502::IMP, &CPU6502::RTS, 6};
    opcodes[0x61] = Instruction{"ADC (INX)", &CPU6502::INX, &CPU6502::ADC, 6};
    opcodes[0x65] = Instruction{"ADC (ZPG)", &CPU6502::ZPG, &CPU6502::ADC, 3};
    opcodes[0x66] = Instruction{"ROR (ZPG)", &CPU6502::ZPG, &CPU6502::ROR, 5};
    opcodes[0x68] = Instruction{"PLA (IMP)", &CPU6502::IMP, &CPU6502::PLA, 4};
    opcodes[0x69] = Instruction{"ADC (IMM)", &CPU6502::IMM, &CPU6502::ADC, 2};
    opcodes[0x6A] = Instruction{"ROR (ACC)", &CPU6502::ACC, &CPU6502::ROR, 2};
    opcodes[0x6C] = Instruction{"JMP (IND)", &CPU6502::IND, &CPU6502::JMP, 5};
    opcodes[0x6D] = Instruction{"ADC (ABS)", &CPU6502::ABS, &CPU6502::ADC, 4};
    opcodes[0x6E] = Instruction{"ROR (ABS)", &CPU6502::ABS, &CPU6502::ROR, 6};

    // Row 7x
    opcodes[0x70] = Instruction{"BVS (REL)", &CPU6502::REL, &CPU6502::BVS, 2};
    opcodes[0x71] = Instruction{"ADC (INY)", &CPU6502::INY, &CPU6502::ADC, 5};
    opcodes[0x75] = Instruction{"ADC (ZPX)", &CPU6502::ZPX, &CPU6502::ADC, 4};
    opcodes[0x76] = Instruction{"ROR (ZPX)", &CPU6502::ZPX, &CPU6502::ROR, 6};
    opcodes[0x78] = Instruction{"SEI (IMP)", &CPU6502::IMP, &CPU6502::SEI, 2};
    opcodes[0x79] = Instruction{"ADC (ABY)", &CPU6502::ABY, &CPU6502::ADC, 4};
    opcodes[0x7D] = Instruction{"ADC (ABX)", &CPU6502::ABX, &CPU6502::ADC, 4};
    opcodes[0x7E] = Instruction{"ROR (ABX)", &CPU6502::ABX, &CPU6502::ROR, 7};

    // Row 8x
    opcodes[0x81] = Instruction{"STA (INX)", &CPU6502::INX, &CPU6502::STA, 6};
    opcodes[0x84] = Instruction{"STY (ZPG)", &CPU6502::ZPG, &CPU6502::STY, 3};
    opcodes[0x85] = Instruction{"STA (ZPG)", &CPU6502::ZPG, &CPU6502::STA, 3};
    opcodes[0x86] = Instruction{"STX (ZPG)", &CPU6502::ZPG, &CPU6502::STX, 3};
    opcodes[0x88] = Instruction{"DEY (IMP)", &CPU6502::IMP, &CPU6502::DEY, 2};
    opcodes[0x8A] = Instruction{"TXA (IMP)", &CPU6502::IMP, &CPU6502::TXA, 2};
    opcodes[0x8C] = Instruction{"STY (ABS)", &CPU6502::ABS, &CPU6502::STY, 4};
    opcodes[0x8D] = Instruction{"STA (ABS)", &CPU6502::ABS, &CPU6502::STA, 4};
    opcodes[0x8E] = Instruction{"STX (ABS)", &CPU6502::ABS, &CPU6502::STX, 4};

    // Row 9x
    opcodes[0x90] = Instruction{"BCC (REL)", &CPU6502::REL, &CPU6502::BCC, 2};
    opcodes[0x91] = Instruction{"STA (INY)", &CPU6502::INY, &CPU6502::STA, 6};
    opcodes[0x94] = Instruction{"STY (ZPX)", &CPU6502::ZPX, &CPU6502::STY, 4};
    opcodes[0x95] = Instruction{"STA (ZPX)", &CPU6502::ZPX, &CPU6502::STA, 4};
    opcodes[0x96] = Instruction{"STX (ZPY)", &CPU6502::ZPY, &CPU6502::STX, 4};
    opcodes[0x98] = Instruction{"TYA (IMP)", &CPU6502::IMP, &CPU6502::TYA, 2};
    opcodes[0x99] = Instruction{"STA (ABY)", &CPU6502::ABY, &CPU6502::STA, 5};
    opcodes[0x9A] = Instruction{"TXS (IMP)", &CPU6502::IMP, &CPU6502::TXS, 2};
    opcodes[0x9D] = Instruction{"STA (ABX)", &CPU6502::ABX, &CPU6502::STA, 5};

    // Row Ax
    opcodes[0xA0] = Instruction{"LDY (IMM)", &CPU6502::IMM, &CPU6502::LDY, 2};
    opcodes[0xA1] = Instruction{"LDA (INX)", &CPU6502::INX, &CPU6502::LDA, 6};
    opcodes[0xA2] = Instruction{"LDX (IMM)", &CPU6502::IMM, &CPU6502::LDX, 2};
    opcodes[0xA4] = Instruction{"LDY (ZPG)", &CPU6502::ZPG, &CPU6502::LDY, 3};
    opcodes[0xA5] = Instruction{"LDA (ZPG)", &CPU6502::ZPG, &CPU6502::LDA, 3};
    opcodes[0xA6] = Instruction{"LDX (ZPG)", &CPU6502::ZPG, &CPU6502::LDX, 3};
    opcodes[0xA8] = Instruction{"TAY (IMP)", &CPU6502::IMP, &CPU6502::TAY, 2};
    opcodes[0xA9] = Instruction{"LDA (IMM)", &CPU6502::IMM, &CPU6502::LDA, 2};
    opcodes[0xAA] = Instruction{"TAX (IMP)", &CPU6502::IMP, &CPU6502::TAX, 2};
    opcodes[0xAC] = Instruction{"LDY (ABS)", &CPU6502::ABS, &CPU6502::LDY, 4};
    opcodes[0xAD] = Instruction{"LDA (ABS)", &CPU6502::ABS, &CPU6502::LDA, 4};
    opcodes[0xAE] = Instruction{"LDX (ABS)", &CPU6502::ABS, &CPU6502::LDX, 4};

    // Row Bx
    opcodes[0xB0] = Instruction{"BCS (REL)", &CPU6502::REL, &CPU6502::BCS, 2};
    opcodes[0xB1] = Instruction{"LDA (INY)", &CPU6502::INY, &CPU6502::LDA, 5};
    opcodes[0xB4] = Instruction{"LDY (ZPX)", &CPU6502::ZPX, &CPU6502::LDY, 4};
    opcodes[0xB5] = Instruction{"LDA (ZPX)", &CPU6502::ZPX, &CPU6502::LDA, 4};
    opcodes[0xB6] = Instruction{"LDX (ZPY)", &CPU6502::ZPY, &CPU6502::LDX, 4};
    opcodes[0xB8] = Instruction{"CLV (IMP)", &CPU6502::IMP, &CPU6502::CLV, 2};
    opcodes[0xB9] = Instruction{"LDA (ABY)", &CPU6502::ABY, &CPU6502::LDA, 4};
    opcodes[0xBA] = Instruction{"TSX (IMP)", &CPU6502::IMP, &CPU6502::TSX, 2};
    opcodes[0xBC] = Instruction{"LDY (ABX)", &CPU6502::ABX, &CPU6502::LDY, 4};
    opcodes[0xBD] = Instruction{"LDA (ABX)", &CPU6502::ABX, &CPU6502::LDA, 4};
    opcodes[0xBE] = Instruction{"LDX (ABY)", &CPU6502::ABY, &CPU6502::LDX, 4};

    // Row Cx
    opcodes[0xC0] = Instruction{"CPY (IMM)", &CPU6502::IMM, &CPU6502::CPY, 2};
    opcodes[0xC1] = Instruction{"CMP (INX)", &CPU6502::INX, &CPU6502::CMP, 6};
    opcodes[0xC4] = Instruction{"CPY (ZPG)", &CPU6502::ZPG, &CPU6502::CPY, 3};
    opcodes[0xC5] = Instruction{"CMP (ZPG)", &CPU6502::ZPG, &CPU6502::CMP, 3};
    opcodes[0xC6] = Instruction{"DEC (ZPG)", &CPU6502::ZPG, &CPU6502::DEC, 5};
    opcodes[0xC8] = Instruction{"INY (IMP)", &CPU6502::IMP, &CPU6502::INY, 2};
    opcodes[0xC9] = Instruction{"CMP (IMM)", &CPU6502::IMM, &CPU6502::CMP, 2};
    opcodes[0xCA] = Instruction{"DEX (IMP)", &CPU6502::IMP, &CPU6502::DEX, 2};
    opcodes[0xCC] = Instruction{"CPY (ABS)", &CPU6502::ABS, &CPU6502::CPY, 4};
    opcodes[0xCD] = Instruction{"CMP (ABS)", &CPU6502::ABS, &CPU6502::CMP, 4};
    opcodes[0xCE] = Instruction{"DEC (ABS)", &CPU6502::ABS, &CPU6502::DEC, 6};

    // Row Dx
    opcodes[0xD0] = Instruction{"BNE (REL)", &CPU6502::REL, &CPU6502::BNE, 2};
    opcodes[0xD1] = Instruction{"CMP (INY)", &CPU6502::INY, &CPU6502::CMP, 5};
    opcodes[0xD5] = Instruction{"CMP (ZPX)", &CPU6502::ZPX, &CPU6502::CMP, 4};
    opcodes[0xD6] = Instruction{"DEC (ZPX)", &CPU6502::ZPX, &CPU6502::DEC, 6};
    opcodes[0xD8] = Instruction{"CLD (IMP)", &CPU6502::IMP, &CPU6502::CLD, 2};
    opcodes[0xD9] = Instruction{"CMP (ABY)", &CPU6502::ABY, &CPU6502::CMP, 4};
    opcodes[0xDD] = Instruction{"CMP (ABX)", &CPU6502::ABX, &CPU6502::CMP, 4};
    opcodes[0xDE] = Instruction{"DEC (ABX)", &CPU6502::ABX, &CPU6502::DEC, 7};

    // Row Ex
    opcodes[0xE0] = Instruction{"CPX (IMM)", &CPU6502::IMM, &CPU6502::CPX, 2};
    opcodes[0xE1] = Instruction{"SBC (INX)", &CPU6502::INX, &CPU6502::SBC, 6};
    opcodes[0xE4] = Instruction{"CPX (ZPG)", &CPU6502::ZPG, &CPU6502::CPX, 3};
    opcodes[0xE5] = Instruction{"SBC (ZPG)", &CPU6502::ZPG, &CPU6502::SBC, 3};
    opcodes[0xE6] = Instruction{"INC (ZPG)", &CPU6502::ZPG, &CPU6502::INC, 5};
    opcodes[0xE8] = Instruction{"INX (IMP)", &CPU6502::IMP, &CPU6502::INX, 2};
    opcodes[0xE9] = Instruction{"SBC (IMM)", &CPU6502::IMM, &CPU6502::SBC, 2};
    opcodes[0xEA] = Instruction{"NOP (IMP)", &CPU6502::IMP, &CPU6502::NOP, 2};
    opcodes[0xEC] = Instruction{"CPX (ABS)", &CPU6502::ABS, &CPU6502::CPX, 4};
    opcodes[0xED] = Instruction{"SBC (ABS)", &CPU6502::ABS, &CPU6502::SBC, 4};
    opcodes[0xEE] = Instruction{"INC (ABS)", &CPU6502::ABS, &CPU6502::INC, 6};

    // Row Fx
    opcodes[0xF0] = Instruction{"BEQ (REL)", &CPU6502::REL, &CPU6502::BEQ, 2};
    opcodes[0xF1] = Instruction{"SBC (INY)", &CPU6502::INY, &CPU6502::SBC, 5};
    opcodes[0xF5] = Instruction{"SBC (ZPX)", &CPU6502::ZPX, &CPU6502::SBC, 4};
    opcodes[0xF6] = Instruction{"INC (ZPX)", &CPU6502::ZPX, &CPU6502::INC, 6};
    opcodes[0xF8] = Instruction{"SED (IMP)", &CPU6502::IMP, &CPU6502::SED, 2};
    opcodes[0xF9] = Instruction{"SBC (ABY)", &CPU6502::ABY, &CPU6502::SBC, 4};
    opcodes[0xFD] = Instruction{"SBC (ABX)", &CPU6502::ABX, &CPU6502::SBC, 4};
    opcodes[0xFE] = Instruction{"INC (ABX)", &CPU6502::ABX, &CPU6502::INC, 7};
}

CPU6502::CPU6502() : stack(this, &sp_reg)
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

bool CPU6502::clock()
{
    if (opCyclesCount == 0)
    {
        clearImpliedFlag();
        
        uint8_t opcode = read(pc_reg++);
        Instruction instruction = opcodes[opcode];

        uint16_t target_addr = (this->*instruction.addressing)(instruction.cycles);
        (this->*instruction.operation)(target_addr, instruction.cycles);

        opCyclesCount += instruction.cycles;
    }
    
    globalCyclesCount++;
    
    if (--opCyclesCount == 0)
    {
        return true;
    }
    return false;
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

bool CPU6502::getStatusFlag(StatusRegistryFlags flag)
{
    return (status_reg & flag) != 0;
}

void CPU6502::pushAddressToStack(uint16_t addr)
{
    stack.push(addr >> HI_BYTE_SHIFT & LO_BYTE_MASK);
    stack.push(addr & LO_BYTE_MASK);
}

uint16_t CPU6502::popAddressFromStack()
{
    uint8_t lo_byte = stack.pop();
    uint8_t hi_byte = stack.pop();
    uint16_t addr = lo_byte | hi_byte << HI_BYTE_SHIFT;
    return addr;
}

void CPU6502::clearImpliedFlag()
{
    impliedFlag = false;
}

void CPU6502::setImpliedFlag(uint8_t value)
{
    impliedFlag = true;
    impliedValue = value;
}

uint8_t CPU6502::fetch(uint16_t address)
{
    if (impliedFlag)
    {
        return impliedValue;
    }
    uint8_t valueFromAddress = read(address);
    return valueFromAddress;
}

void CPU6502::submit(uint16_t address, uint8_t value)
{
    if (impliedFlag)
    {
        a_reg = value;
    }
    else
    {
        write(address, value);
    }
}

void CPU6502::branch(uint8_t relAddr, uint8_t& cycles)
{
    cycles++;
    uint16_t currentPC = pc_reg;
    uint16_t nextPC = pc_reg + relAddr;
    if (!isAddressOnSamePage(currentPC, nextPC))
    {
        cycles++;
    }
    pc_reg = nextPC;
}

void CPU6502::compare(uint16_t addr, uint8_t reg)
{
    uint8_t memory = fetch(addr);
    uint8_t result = reg - memory;

    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Carry, reg >= memory);
}

// Addressing modes
uint16_t CPU6502::ACC(uint8_t& cycles)
{
    setImpliedFlag(a_reg);
    return 0;
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
    setImpliedFlag(a_reg);
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
    uint16_t lo = read((zp_pointer) & LO_BYTE_MASK);
    uint16_t hi = read((zp_pointer + 1) & LO_BYTE_MASK);
    uint16_t addr = static_cast<uint16_t>(hi << HI_BYTE_SHIFT) | lo;
    uint16_t shifted_addr = addr + y_reg;
    if (!isAddressOnSamePage(addr, shifted_addr))
    {
        cycles++;
    }
    return shifted_addr;
}

uint16_t CPU6502::REL(uint8_t& cycles)
{
    uint8_t addr_shift = read(pc_reg++);
    uint16_t shift16bit = addr_shift;
    if (isSigned(addr_shift))
    {
        shift16bit |= HI_BYTE_MASK;
    }
    return shift16bit;
}

uint16_t CPU6502::ZPG(uint8_t& cycles)
{
    uint16_t zpg_addr = read(pc_reg++);
    return zpg_addr;
}

uint16_t CPU6502::ZPX(uint8_t& cycles)
{
    uint16_t zpg_addr = read(pc_reg++);
    uint16_t addr = (zpg_addr + x_reg) & LO_BYTE_MASK;
    return addr;
}

uint16_t CPU6502::ZPY(uint8_t& cycles)
{
    uint16_t zpg_addr = read(pc_reg++);
    uint16_t addr = (zpg_addr + y_reg) & LO_BYTE_MASK;
    return addr;
}

// Operations
void CPU6502::ADC(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t a = a_reg;
    uint8_t c = getStatusFlag(Carry) ? 1 : 0;
    uint16_t result_16bit = memory + a + c;
    uint8_t result = result_16bit & LO_BYTE_MASK;

    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Carry, isCarryFromUnsignedAdd(result_16bit));
    setStatusFlag(Overflow, isOverflowFromSignedAdd(a, memory, result));
    
    a_reg = result;
}

void CPU6502::AND(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t result = memory & a_reg;
    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, isSigned(result));
    a_reg = result;
}

void CPU6502::ASL(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t result = memory << 1;
    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Carry, testBitMask8Bit(BIT_7, memory));
    submit(address, result);
}

void CPU6502::BCC(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Carry) == true)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::BCS(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Carry) == false)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::BEQ(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Zero) == false)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::BIT(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t result = a_reg & memory;
    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, testBitMask8Bit(BIT_7, memory));
    setStatusFlag(Overflow, testBitMask8Bit(BIT_6, memory));
}

void CPU6502::BMI(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Negative) == false)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::BNE(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Zero) == true)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::BPL(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Negative) == true)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::BRK(uint16_t address, uint8_t& cycles)
{
    pc_reg++;
    uint8_t sr = status_reg | BRKCommand | Unused; // bit 5 always set when pushing SR
    
    pushAddressToStack(pc_reg);
    stack.push(sr);

    setStatusFlag(IRQDisable, true);
    
    uint16_t next_pc = readFullAddress(*bus, SYSTEM_VECTOR_IRQ);
    pc_reg = next_pc;
}

void CPU6502::BVC(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Overflow) == false)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::BVS(uint16_t address, uint8_t& cycles)
{
    if (getStatusFlag(Overflow) == true)
    {
        branch(static_cast<uint8_t>(address), cycles);
    }
}

void CPU6502::CLC(uint16_t address, uint8_t& cycles)
{
    setStatusFlag(Carry, false);
}

void CPU6502::CLD(uint16_t address, uint8_t& cycles)
{
    setStatusFlag(DecimalMode, false);
}

void CPU6502::CLI(uint16_t address, uint8_t& cycles)
{
    setStatusFlag(IRQDisable, false);
}

void CPU6502::CLV(uint16_t address, uint8_t& cycles)
{
    setStatusFlag(Overflow, false);
}

void CPU6502::CMP(uint16_t address, uint8_t& cycles)
{
    compare(address, a_reg);
}

void CPU6502::CPX(uint16_t address, uint8_t& cycles)
{
    compare(address, x_reg);
}

void CPU6502::CPY(uint16_t address, uint8_t& cycles)
{
    compare(address, y_reg);
}

void CPU6502::DEC(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    memory--;
    
    setStatusFlag(Negative, isSigned(memory));
    setStatusFlag(Zero, isZero(memory));
    submit(address, memory);
}

void CPU6502::DEX(uint16_t address, uint8_t& cycles)
{
    x_reg--;
    setStatusFlag(Negative, isSigned(x_reg));
    setStatusFlag(Zero, isZero(x_reg));
}

void CPU6502::DEY(uint16_t address, uint8_t& cycles)
{
    y_reg--;
    setStatusFlag(Negative, isSigned(y_reg));
    setStatusFlag(Zero, isZero(y_reg));
}

void CPU6502::EOR(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t result = a_reg ^ memory;
    
    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Zero, isZero(result));
    a_reg = result;
}

void CPU6502::INC(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    memory++;
    
    setStatusFlag(Negative, isSigned(memory));
    setStatusFlag(Zero, isZero(memory));
    submit(address, memory);
}

void CPU6502::INX(uint16_t address, uint8_t& cycles)
{
    x_reg++;
    setStatusFlag(Negative, isSigned(x_reg));
    setStatusFlag(Zero, isZero(x_reg));
}

void CPU6502::INY(uint16_t address, uint8_t& cycles)
{
    y_reg++;
    setStatusFlag(Negative, isSigned(y_reg));
    setStatusFlag(Zero, isZero(y_reg));
}

void CPU6502::JMP(uint16_t address, uint8_t& cycles)
{
    pc_reg = address;
}

void CPU6502::JSR(uint16_t address, uint8_t& cycles)
{
    pushAddressToStack(pc_reg - 1);
    pc_reg = address;
}

void CPU6502::LDA(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);

    setStatusFlag(Negative, isSigned(memory));
    setStatusFlag(Zero, isZero(memory));
    a_reg = memory;
}

void CPU6502::LDX(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);

    setStatusFlag(Negative, isSigned(memory));
    setStatusFlag(Zero, isZero(memory));
    x_reg = memory;
}

void CPU6502::LDY(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);

    setStatusFlag(Negative, isSigned(memory));
    setStatusFlag(Zero, isZero(memory));
    y_reg = memory;
}

void CPU6502::LSR(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t result = memory >> 1;
    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, false);
    setStatusFlag(Carry, testBitMask8Bit(BIT_0, memory));
    submit(address, result);
}

void CPU6502::NOP(uint16_t address, uint8_t& cycles)
{
    // Place your Ad here
}

void CPU6502::ORA(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t result = a_reg | memory;
    
    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Zero, isZero(result));
    a_reg = result;
}

void CPU6502::PHA(uint16_t address, uint8_t& cycles)
{
    stack.push(a_reg);
}

void CPU6502::PHP(uint16_t address, uint8_t& cycles)
{
    uint8_t sr = status_reg | BRKCommand | Unused; // B, U flag should be set for this instruction
    stack.push(sr);
}

void CPU6502::PLA(uint16_t address, uint8_t& cycles)
{
    uint8_t a = stack.pop();
    setStatusFlag(Zero, isZero(a));
    setStatusFlag(Negative, isSigned(a));
    a_reg = a;
}

void CPU6502::PLP(uint16_t address, uint8_t& cycles)
{
    uint8_t sr = (stack.pop() & ~BRKCommand) | Unused; // B flag is ignored, U flag is always set
    status_reg = sr;
}

void CPU6502::ROL(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t carry = getStatusFlag(Carry) ? 1 : 0;
    uint8_t result = memory << 1 | carry;
    
    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Carry, testBitMask8Bit(BIT_7, memory));
    submit(address, result);
}

void CPU6502::ROR(uint16_t address, uint8_t& cycles)
{
    uint8_t memory = fetch(address);
    uint8_t carry = getStatusFlag(Carry) ? 1 : 0;
    uint8_t result = memory >> 1 | carry << 7;
    
    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Carry, testBitMask8Bit(BIT_0, memory));
    submit(address, result);
}

void CPU6502::RTI(uint16_t address, uint8_t& cycles)
{
    uint8_t sr = (stack.pop() & ~BRKCommand) | Unused; // B flag is ignored, U flag is always set
    status_reg = sr;
    uint16_t pc = popAddressFromStack();
    pc_reg = pc;
}

void CPU6502::RTS(uint16_t address, uint8_t& cycles)
{
    uint16_t pc = popAddressFromStack();
    pc_reg = pc + 1;
}

void CPU6502::SBC(uint16_t address, uint8_t& cycles)
{
    // SBC operation should be A -> A - M - (1 - C)
    // But it can be simplified to A -> A + ~M + C
    // Which is exactly the same as addition
    uint8_t memory = fetch(address);
    uint8_t a = a_reg;
    uint8_t c = getStatusFlag(Carry) ? 1 : 0;
    
    uint8_t invM = ~memory;
    uint16_t result_16bit = a + invM + c;
    uint8_t result = result_16bit & LO_BYTE_MASK;

    setStatusFlag(Zero, isZero(result));
    setStatusFlag(Negative, isSigned(result));
    setStatusFlag(Carry, isCarryFromUnsignedAdd(result_16bit));
    setStatusFlag(Overflow, isOverflowFromSignedAdd(a, invM, result));
    
    a_reg = result;
}

void CPU6502::SEC(uint16_t address, uint8_t& cycles)
{
    setStatusFlag(Carry, true);
}

void CPU6502::SED(uint16_t address, uint8_t& cycles)
{
    setStatusFlag(DecimalMode, true);
}

void CPU6502::SEI(uint16_t address, uint8_t& cycles)
{
    setStatusFlag(IRQDisable, true);
}

void CPU6502::STA(uint16_t address, uint8_t& cycles)
{
    uint8_t value = a_reg;
    write(address, value);
}

void CPU6502::STX(uint16_t address, uint8_t& cycles)
{
    uint8_t value = x_reg;
    write(address, value);
}

void CPU6502::STY(uint16_t address, uint8_t& cycles)
{
    uint8_t value = y_reg;
    write(address, value);
}

void CPU6502::TAX(uint16_t address, uint8_t& cycles)
{
    uint8_t value = a_reg;
    x_reg = value;
    setStatusFlag(Zero, isZero(value));
    setStatusFlag(Negative, isSigned(value));
}

void CPU6502::TAY(uint16_t address, uint8_t& cycles)
{
    uint8_t value = a_reg;
    y_reg = value;
    setStatusFlag(Zero, isZero(value));
    setStatusFlag(Negative, isSigned(value));
}

void CPU6502::TSX(uint16_t address, uint8_t& cycles)
{
    uint8_t value = sp_reg;
    x_reg = value;
    setStatusFlag(Zero, isZero(value));
    setStatusFlag(Negative, isSigned(value));
}

void CPU6502::TXA(uint16_t address, uint8_t& cycles)
{
    uint8_t value = x_reg;
    a_reg = value;
    setStatusFlag(Zero, isZero(value));
    setStatusFlag(Negative, isSigned(value));
}

void CPU6502::TXS(uint16_t address, uint8_t& cycles)
{
    uint8_t value = x_reg;
    sp_reg = value;
}

void CPU6502::TYA(uint16_t address, uint8_t& cycles)
{
    uint8_t value = y_reg;
    a_reg = value;
    setStatusFlag(Zero, isZero(value));
    setStatusFlag(Negative, isSigned(value));
}

void CPU6502::ILL(uint16_t address, uint8_t& cycles)
{
    // Do nothing. No illigal opcodes implementation for this NES emulator
}
