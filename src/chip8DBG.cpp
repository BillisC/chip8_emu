#include "../include/chip8DBG.h"

// Terminal Output
void Chip8Debugger::prntMemoryDUMP(){
    for(unsigned int i = 0; i < 4096; ++i){
        printf("0x%04x: %02x", i, C8S->memory[i]); 
    }
}

void Chip8Debugger::prntGraphicsMemoryDUMP(){
    for(unsigned int i = 0; i < 2048; ++i){
        printf("0x%04x: %02x", i, C8S->gfx[i]);
    }
}

// File Output
void Chip8Debugger::memoryDUMP(){
    std::fstream salta_kai_gamhsou;
    salta_kai_gamhsou.open("debug/memory_dump.bin", std::ios::out | std::ios::binary);
    if(salta_kai_gamhsou){
        for(unsigned int i = 0; i < 4096; ++i){
            salta_kai_gamhsou.write(reinterpret_cast<char *>(&C8S->memory[i]), 1);
        }
        salta_kai_gamhsou.close();
    }
    else{
        printf("I/O ERROR: Memory dump file issue");
    }
}

void Chip8Debugger::graphicsMemoryDUMP(){
    std::fstream salta_kai_gamhsou;
    salta_kai_gamhsou.open("debug/graphics_memory_dump.bin", std::ios::out | std::ios::binary);
    if(salta_kai_gamhsou){
        for(unsigned int i = 0; i < 2048; ++i){
            salta_kai_gamhsou.write(reinterpret_cast<char *>(&C8S->gfx[i]), 1);
        }
        salta_kai_gamhsou.close();
    }
    else{
        printf("I/O ERROR: Graphics memory dump file issue");
    }
}

// Updates
void Chip8Debugger::registerChanges(uint8_t *prev_state){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FlushConsoleInputBuffer(hConsole);
    SetConsoleTextAttribute(hConsole, 159);
    for(unsigned int i = 0; i < 16; ++i){
        if(prev_state[i] != C8S->V[i]) printf("-Register %d: %02x -> %02x", i, prev_state[i], C8S->V[i]);
    }
    SetConsoleTextAttribute(hConsole, 7);
    printf("\n");
}

void Chip8Debugger::stackChanges(uint16_t &prev_state){
    if(C8S->sp != prev_state){
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        FlushConsoleInputBuffer(hConsole);
        SetConsoleTextAttribute(hConsole, 143);
        for(unsigned int i = 0; i < C8S->sp; ++i){
            printf("-Stack %d: %04x\n", i, C8S->stack[i]);
        }
        SetConsoleTextAttribute(hConsole, 7);
    }
    printf("\n");
}

void Chip8Debugger::memoryChanges(uint8_t *prev_state){
    for(unsigned int i = 0; i < 4096; ++i){
        if(prev_state[i] != C8S->memory[i]) printf("+Change at 0x%02x: %02x -> %02x\n", i, prev_state[i], C8S->memory[i]);
    }
}

void Chip8Debugger::prntKeyboard(){
    for(int i = 0; i < 16; ++i){
        printf("Key %x: %d\n", i, C8S->key[i]);
    }
}