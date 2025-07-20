#pragma once

#include <cstdint>
#include <string>

#include "../Bus/BusDevice.h"

class CPU6502 : public BusDevice
{
private:
    static constexpr int INSTRUCTION_SET_SIZE = 256;
    
    uint8_t a_reg = 0x00;       // Accumulator register
    uint8_t x_reg = 0x00;       // X register
    uint8_t y_reg = 0x00;       // Y register
    uint16_t pc_reg = 0x0000;   // Program Counter
    uint8_t sp_reg = 0x00;      // Stack Pointer
    uint8_t status_reg = 0x00;  // Status register
    
    uint8_t fetch = 0x00;
    uint8_t cycles = 0;         // Number of cycles left to be executed
    uint64_t cyclesCount = 0;   // Number of cycles since cpu start
    
    void resetInternal();       // Resets everything to zero
    void initializeOpcodes();   // Generates opcodes lookup map
    
public:
    CPU6502();
    ~CPU6502() override;
    
    void write(uint16_t addr, uint8_t val) override;
    uint8_t read(uint16_t addr) override;
    
    void IRQ();     // Interrupt request signal
    void NMI();     // Non-maskable interrupt
    void reset();   // Init cpu, when NES is powered or on reset
    
    bool clock();   // Execute one clock cycle. Returns true if operation completed all cycles
    
private:
    enum StatusRegistryFlags : uint8_t
    {
        Carry       = (1 << 0),
        Zero        = (1 << 1),
        IRQDisable  = (1 << 2),
        DecimalMode = (1 << 3),
        BRKCommand  = (1 << 4),
        Unused      = (1 << 5),
        Overflow    = (1 << 6),
        Negative    = (1 << 7),
    };

    void setStatusFlag(StatusRegistryFlags flag, bool predicate);
    uint8_t getStatusFlag(StatusRegistryFlags flag);

    struct Instruction
    {
        std::string mnemonic;                                           // Mnemonic to use for debugging purposes
        uint16_t(CPU6502::*addressing)(uint8_t& cycles);                // Function pointer for addressing mode
        void(CPU6502::*operation)(uint16_t address, uint8_t& cycles);   // Function pointer for operation
    };

    Instruction opcodes[INSTRUCTION_SET_SIZE];

    // Addressing modes
    uint16_t ACC(uint8_t& cycles); // accumulator
    uint16_t ABS(uint8_t& cycles); // absolute
    uint16_t ABX(uint8_t& cycles); // absolute, X-indexed
    uint16_t ABY(uint8_t& cycles); // absolute, Y-indexed
    uint16_t IMM(uint8_t& cycles); // immediate
    uint16_t IMP(uint8_t& cycles); // implied
    uint16_t IND(uint8_t& cycles); // indirect
    uint16_t INX(uint8_t& cycles); // X-indexed, indirect
    uint16_t INY(uint8_t& cycles); // indirect, Y-indexed
    uint16_t REL(uint8_t& cycles); // relative
    uint16_t ZPG(uint8_t& cycles); // zeropage
    uint16_t ZPX(uint8_t& cycles); // zeropage, X-indexed	
    uint16_t ZPY(uint8_t& cycles); // zeropage, Y-indexed

    // Operations
    void ADC(uint16_t address, uint8_t& cycles); // add with carry
    void AND(uint16_t address, uint8_t& cycles); // and (with accumulator)
    void ASL(uint16_t address, uint8_t& cycles); // arithmetic shift left
    void BCC(uint16_t address, uint8_t& cycles); // branch on carry clear
    void BCS(uint16_t address, uint8_t& cycles); // branch on carry set
    void BEQ(uint16_t address, uint8_t& cycles); // branch on equal (zero set)
    void BIT(uint16_t address, uint8_t& cycles); // bit test
    void BMI(uint16_t address, uint8_t& cycles); // branch on minus (negative set)
    void BNE(uint16_t address, uint8_t& cycles); // branch on not equal (zero clear)
    void BPL(uint16_t address, uint8_t& cycles); // branch on plus (negative clear)
    void BRK(uint16_t address, uint8_t& cycles); // break / interrupt
    void BVC(uint16_t address, uint8_t& cycles); // branch on overflow clear
    void BVS(uint16_t address, uint8_t& cycles); // branch on overflow set
    void CLC(uint16_t address, uint8_t& cycles); // clear carry
    void CLD(uint16_t address, uint8_t& cycles); // clear decimal
    void CLI(uint16_t address, uint8_t& cycles); // clear interrupt disable
    void CLV(uint16_t address, uint8_t& cycles); // clear overflow
    void CMP(uint16_t address, uint8_t& cycles); // compare (with accumulator)
    void CPX(uint16_t address, uint8_t& cycles); // compare with X
    void CPY(uint16_t address, uint8_t& cycles); // compare with Y
    void DEC(uint16_t address, uint8_t& cycles); // decrement
    void DEX(uint16_t address, uint8_t& cycles); // decrement X
    void DEY(uint16_t address, uint8_t& cycles); // decrement Y
    void EOR(uint16_t address, uint8_t& cycles); // exclusive or (with accumulator)
    void INC(uint16_t address, uint8_t& cycles); // increment
    void INX(uint16_t address, uint8_t& cycles); // increment X
    void INY(uint16_t address, uint8_t& cycles); // increment Y
    void JMP(uint16_t address, uint8_t& cycles); // jump
    void JSR(uint16_t address, uint8_t& cycles); // jump subroutine
    void LDA(uint16_t address, uint8_t& cycles); // load accumulator
    void LDX(uint16_t address, uint8_t& cycles); // load X
    void LDY(uint16_t address, uint8_t& cycles); // load Y
    void LSR(uint16_t address, uint8_t& cycles); // logical shift right
    void NOP(uint16_t address, uint8_t& cycles); // no operation
    void ORA(uint16_t address, uint8_t& cycles); // or with accumulator
    void PHA(uint16_t address, uint8_t& cycles); // push accumulator
    void PHP(uint16_t address, uint8_t& cycles); // push processor status (SR)
    void PLA(uint16_t address, uint8_t& cycles); // pull accumulator
    void PLP(uint16_t address, uint8_t& cycles); // pull processor status (SR)
    void ROL(uint16_t address, uint8_t& cycles); // rotate left
    void ROR(uint16_t address, uint8_t& cycles); // rotate right
    void RTI(uint16_t address, uint8_t& cycles); // return from interrupt
    void RTS(uint16_t address, uint8_t& cycles); // return from subroutine
    void SBC(uint16_t address, uint8_t& cycles); // subtract with carry
    void SEC(uint16_t address, uint8_t& cycles); // set carry
    void SED(uint16_t address, uint8_t& cycles); // set decimal
    void SEI(uint16_t address, uint8_t& cycles); // set interrupt disable
    void STA(uint16_t address, uint8_t& cycles); // store accumulator
    void STX(uint16_t address, uint8_t& cycles); // store X
    void STY(uint16_t address, uint8_t& cycles); // store Y
    void TAX(uint16_t address, uint8_t& cycles); // transfer accumulator to X
    void TAY(uint16_t address, uint8_t& cycles); // transfer accumulator to Y
    void TSX(uint16_t address, uint8_t& cycles); // transfer stack pointer to X
    void TXA(uint16_t address, uint8_t& cycles); // transfer X to accumulator
    void TXS(uint16_t address, uint8_t& cycles); // transfer X to stack pointer
    void TYA(uint16_t address, uint8_t& cycles); // transfer Y to accumulator
    void ILL(uint16_t address, uint8_t& cycles); // illegal opcode
};
