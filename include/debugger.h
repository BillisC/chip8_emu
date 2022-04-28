#ifndef CHIP8DBG_H
#define CHIP8DBG_H

#include <fstream>

#include "chip8.h"

class Chip8Debugger{
    Chip8System *C8S{nullptr};

public:
    Chip8Debugger(Chip8System *C8S){ this->C8S = C8S; }

    // Terminal Output
    void opcodeAnalyser(const uint16_t &full_opcode, const uint16_t &pc);

    // File Output
    void memoryDUMP();
    void graphicsMemoryDUMP();
};

#endif