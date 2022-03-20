// This file has copywrite protection from me uwu -BASILHS 2022
#include <chrono>
#include <thread>

#define SDL_MAIN_HANDLED

#include "../include/chip8.h"
#include "../include/Chip8Handler.h"
#include "../include/chip8DBG.h"
#include "../include/Graphics/GraphicsCore.h"

using namespace std::chrono_literals;

int main(int argc, char* argv[]){
    bool POWER = true;
    
    // Initialize graphics window
    GraphicsHandler *GFH = new GraphicsHandler(640, 320);

    // Prepare chip8 system
    Chip8System *C8S = new Chip8System;
    Chip8Handler *C8H = new Chip8Handler(C8S);
    Chip8Debugger *C8D = new Chip8Debugger(C8S);

    // Initialization
    C8H->chipINIT();

    // Read file
    if(argv[1] != nullptr){
        std::string path(argv[1]);
        if(!C8H->romLoader(path)){
            return -1;
        }
    }
    else{
        printf("No files found\n");
        return 1;
    }

    // Dump memories in file
    C8D->memoryDUMP();
    C8D->graphicsMemoryDUMP();

    // Debug
    uint8_t memory_buffer[4096];
    uint8_t reg_buffer[16];
    uint16_t sp_buffer = C8S->sp;

    std::chrono::system_clock::time_point prev = std::chrono::system_clock::now();

    while(POWER){
        if(std::chrono::duration(std::chrono::system_clock::now() - prev) > std::chrono::microseconds(1666)){
            //memcpy(memory_buffer, C8S->memory, 4096); // DEBUG
            //memcpy(reg_buffer, C8S->V, 16); // DEBUG
            //sp_buffer = C8S->sp; // DEBUG

            //C8H->instructionDebugging();
            C8H->instructionHandler();
            C8H->timerHandler();

            if(C8H->drawMode){
                GFH->updateGFXBuffer(C8S->gfx);
                C8H->drawMode = false;
            }

            if(GFH->EventCheck(C8S->key) == 1) break;

            //C8D->prntKeyboard(); // DEBUG
            //C8D->registerChanges(reg_buffer); // DEBUG
            //C8D->memoryChanges(memory_buffer); // DEBUG
            //C8D->stackChanges(sp_buffer); // DEBUG
            //C8D->memoryDUMP(); // DEBUG
            //C8D->graphicsMemoryDUMP(); // DEBUG

            //std::cin.get();
            prev = std::chrono::system_clock::now();
        }
    }

    delete GFH;
    delete C8D;
    delete C8H;

    return 0;
}