#ifndef Chip8Handler_H
#define Chip8Handler_H

#include <cstring>
#include <fstream>

#include "chip8.h"

class Chip8Handler{
    Chip8System *C8S{nullptr};

private:
    void fontLoader();

public:
    Chip8Handler(Chip8System *C8S){ this->C8S = C8S; }
    ~Chip8Handler(){ delete C8S; }
    
    // Initialization
    void chipINIT();
    int romLoader(const std::string file_name);

    // Handlers
    void instructionHandler();
    void graphicsHandler(uint8_t X, uint8_t Y, uint8_t height);
    void timerHandler();

    // Debugging
    void instructionDebugging();

    // Reset
    void resetGraphicsMemory();
    void resetMemory();
    void resetStack();

    bool drawMode{false};
};

#endif