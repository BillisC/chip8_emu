#include <chrono>

#define SDL_MAIN_HANDLED

#include "../include/chip8.h"
#include "../include/handler.h"
#include "../include/debugger.h"
#include "../include/window.h"

Chip8System *C8S{nullptr};
Chip8Handler *C8H{nullptr};
Chip8Debugger *C8D{nullptr};

bool DBG = false;

int main(int argc, char* argv[]){  
    // Initialize graphics window
    WindowHandler *GFH = new WindowHandler(640, 320);
    if(GFH->failCheck()) return -3;

    // Prepare chip8 system
    Chip8System *C8S = new Chip8System;
    Chip8Handler *C8H = new Chip8Handler(C8S);

    // Argument check
    if(argc <= 1){ printf("Missing parameters"); return -1; }
    else{
        // Load file 
        std::string path(argv[1]);
        if(C8H->romLoader(path) == -1) return -2;

        // Look for other arguments
        for(uint8_t it = 2; it < argc; ++it){
            std::string arg(argv[it]);
            if(arg == "-c" && it+1 < argc) C8S->clock_speed = 1000000 / stoi(std::string(argv[++it]));
            else if(arg == "-dbg") DBG = true;
        }
    }

    // Enable debugging
    if(DBG) C8D = new Chip8Debugger(C8S);
    uint16_t prev_pc;

    std::chrono::system_clock::time_point prev = std::chrono::system_clock::now();
    while(true){
        if(std::chrono::duration(std::chrono::system_clock::now() - prev) > std::chrono::microseconds(C8S->clock_speed)){
            prev_pc = C8S->PC;

            // Next cycle
            C8H->clock();

            // Draw graphics only when needed
            if(C8H->drawMode){
                GFH->updateGFXBuffer(C8S->gfx);
                C8H->drawMode = false;
            }

            // Events
            if(GFH->EventCheck(C8S->key) == 1) break;
            
            // Debugging
            if(DBG){ 
                C8D->opcodeAnalyser(C8H->full_opcode, prev_pc);
                C8D->graphicsMemoryDUMP(); 
                C8D->memoryDUMP();
            }

            prev = std::chrono::system_clock::now();
        }
    }

    // Clear and quit
    delete GFH;
    delete C8D;
    delete C8H;

    return 0;
}