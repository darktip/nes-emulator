#include "CPU6502Stack.h"

using namespace nes;

CPU6502Stack::CPU6502Stack(IODevice* cpu, uint8_t* stackPointer)
{
    this->cpu = cpu;
    this->stackPointer = stackPointer;
}

void CPU6502Stack::push(uint8_t byte)
{
    uint16_t addr = getAddress();
    cpu->write(addr, byte);
    (*stackPointer)--;
}

uint8_t CPU6502Stack::pop()
{
    (*stackPointer)++;
    uint16_t addr = getAddress();
    uint8_t byte = cpu->read(addr);
    return byte;
}

uint16_t CPU6502Stack::getAddress()
{
    uint16_t addr = STACK_BASE_ADDRESS + *stackPointer;
    return addr;
}
