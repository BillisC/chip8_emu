#ifndef CHIP8DBG_H
#define CHIP8DBG_H

#include <iostream>
#include <fstream>

#include "chip8.h"
#include <windows.h>

class Chip8Debugger{
    Chip8System *C8S{nullptr};

public:
    Chip8Debugger(Chip8System *C8S){ this->C8S = C8S; };
    
    // Terminal Output
    void prntMemoryDUMP();
    void prntGraphicsMemoryDUMP();
    void prntKeyboard();

    // File Output
    void memoryDUMP();
    void graphicsMemoryDUMP();

    // Updates
    void registerChanges(uint8_t *prev_state);
    void stackChanges(uint16_t &prev_state);
    void memoryChanges(uint8_t *prev_state);
};

#endif