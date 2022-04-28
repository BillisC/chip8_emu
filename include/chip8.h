#ifndef CHIP8_H
#define CHIP8_H

struct Chip8System{
    // Memory
    uint8_t memory[4096];
    uint8_t V[16];

    // Graphics memory
    uint8_t gfx[64 * 32];

    // Memory indexers
    uint16_t I;
    uint16_t PC;

    // Timers
    unsigned int clock_speed;
    unsigned int delay_timer;
    unsigned int sound_timer;

    // The stack
    uint16_t stack[16];
    uint16_t sp;

    // Keypad
    uint8_t key[16];
};

#endif