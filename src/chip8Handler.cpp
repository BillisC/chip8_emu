#include "../include/chip8Handler.h"

// Initialization
void Chip8Handler::chipINIT(){
    C8S->PC = 0x200;
    C8S->I = 0x00;
    C8S->sp = 0x00;

    for(unsigned int i = 0; i < 16; ++i){
        C8S->V[i] = 0;
    }

    // Reset keyboard
    memset(C8S->key, 0, 16);

    // Reset memory
    resetMemory();

    // Reset graphics buffer
    resetGraphicsMemory();

    // Reset stack
    resetStack();

    // Set font
    fontLoader();
}

int Chip8Handler::romLoader(const std::string file_name){
    std::fstream rom;
    rom.open(file_name, std::ios::in | std::ios::binary);
    
    if(rom.is_open()){
        // Get the file's size
        rom.seekg(0, std::ios::end);
        size_t file_size = rom.tellg();
        rom.seekg(0, std::ios::beg);

        // Load the ROM in the RAM
        rom.read(reinterpret_cast<char *>(C8S->memory + 0x200), file_size);
        if(rom){
            printf("\nSuccessfully loaded rom file in the memory");
        }
        else{
            printf("\nFailed to load the rom file in the memory");
            return -2;
        }

        // Print ROM info
        printf("\nROM Size: %d\n", file_size);

        rom.close();
    }
    else{
        printf("\nFailed to load file"); 
        return -1; 
    }

    return 1;
}

void Chip8Handler::fontLoader(){
    uint8_t fontBuffer[] = {0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                            0x20, 0x60, 0x20, 0x20, 0x70, // 1
                            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    memcpy(C8S->memory + 0x050, fontBuffer, sizeof(fontBuffer) / sizeof(uint8_t));
}

// Handlers
void Chip8Handler::instructionHandler(){
    uint8_t *opcode = &C8S->memory[C8S->PC];

    uint16_t full_opcode = (opcode[0] << 8) | opcode[1];

    uint8_t second_nib = (opcode[0] & 0x0F);
    uint8_t third_nib  = (opcode[1] >> 4);

    switch(full_opcode & 0xF000){
        case 0x0:
            switch(opcode[1]){
                case 0xE0:
                    resetGraphicsMemory();
                    C8S->PC += 2;
                    break;
                case 0xEE:
                    --C8S->sp;
                    C8S->PC = C8S->stack[C8S->sp];
                    break;
            }
            break;

        case 0x1000:
            C8S->PC = full_opcode & 0x0FFF; 
            break;

        case 0x2000:
            C8S->stack[C8S->sp] = C8S->PC + 2;
            ++C8S->sp;
            C8S->PC = full_opcode & 0x0FFF;
            break;

        case 0x3000: 
            if(C8S->V[second_nib] == opcode[1]) C8S->PC+=2;
            C8S->PC += 2;
            break;

        case 0x4000: 
            if(C8S->V[second_nib] != opcode[1]) C8S->PC+=2;
            C8S->PC += 2;
            break;

        case 0x5000: 
            if(C8S->V[second_nib] == C8S->V[third_nib]) C8S->PC+=2;
            C8S->PC += 2;
            break;

        case 0x6000:
            C8S->V[second_nib] = opcode[1];
            C8S->PC += 2;
            break;

        case 0x7000:
            C8S->V[second_nib] += opcode[1];
            C8S->PC += 2;
            break;

        case 0x8000: 
            switch(full_opcode & 0x000F){
                case 0x0:
                    C8S->V[second_nib] = C8S->V[third_nib];
                    C8S->PC += 2;
                    break;

                case 0x1:
                    C8S->V[second_nib] = C8S->V[second_nib] | C8S->V[third_nib];
                    C8S->PC += 2;
                    break;

                case 0x2:
                    C8S->V[second_nib] = C8S->V[second_nib] & C8S->V[third_nib];
                    C8S->PC += 2;
                    break;

                case 0x3:
                    C8S->V[second_nib] = C8S->V[second_nib] ^ C8S->V[third_nib];
                    C8S->PC += 2;
                    break;

                case 0x4:
                {
                    uint16_t res = C8S->V[second_nib] + C8S->V[third_nib];
                    C8S->V[second_nib] = (uint8_t)res;
                    (res >> 8) ? C8S->V[15] = 1 : C8S->V[15] = 0;
                    C8S->PC += 2;
                    break;
                }
                case 0x5:
                    if(C8S->V[second_nib] > C8S->V[third_nib]) C8S->V[15] = 1;
                    else C8S->V[15] = 0;
                    C8S->V[second_nib] -= C8S->V[third_nib];
                    C8S->PC += 2;
                    break;

                case 0x6:
                    (C8S->V[second_nib] & 0x1) ? C8S->V[15] = 1 : C8S->V[15] = 0;
                    C8S->V[second_nib] >>= 1;
                    C8S->PC += 2;
                    break;

                case 0x7:
                    C8S->V[second_nib] = C8S->V[third_nib] - C8S->V[second_nib];
                    (C8S->V[third_nib] > C8S->V[second_nib]) ? C8S->V[15] = 1 : C8S->V[15] = 0;
                    C8S->PC += 2;
                    break;

                case 0xE:
                    (C8S->V[second_nib] & 0x80) ? C8S->V[15] = 1 : C8S->V[15] = 0;
                    C8S->V[second_nib] <<= 1;
                    C8S->PC += 2;
                    break;
                break;
            }
            break;

        case 0x9000:
            if(C8S->V[second_nib] != C8S->V[third_nib]) C8S->PC+=2;
            C8S->PC += 2;
            break;
            
        case 0xA000: 
            C8S->I = full_opcode & 0x0FFF;
            C8S->PC += 2;
            break;

        case 0xB000:
            C8S->PC = full_opcode & 0x0FFF + C8S->V[0];
            break;

        case 0xC000: 
            C8S->V[second_nib] = (uint8_t)(rand() % 256) & opcode[1];
            C8S->PC += 2;
            break;

        case 0xD000:
            graphicsHandler(C8S->V[second_nib], C8S->V[third_nib], full_opcode & 0x000F);
            C8S->PC += 2;
            break;

        case 0xE000: 
            switch(opcode[1]){
                case 0x9E:
                    if(C8S->key[C8S->V[second_nib]] != 0) C8S->PC += 4;  
                    else C8S->PC += 2;
                    break;
                case 0xA1:
                    if(C8S->key[C8S->V[second_nib]] == 0) C8S->PC += 4;
                    else C8S->PC += 2;
                    break;
            }
            break;

        case 0xF000: 
            switch(opcode[1]){
                case 0x07:
                    C8S->V[second_nib] = C8S->delay_timer;
                    C8S->PC += 2;
                    break;
                case 0x0A:
                    for(unsigned int i = 0; i < 0xF; ++i){
                        if(C8S->key[i]){
                            C8S->V[second_nib] = C8S->key[i];
                            C8S->PC += 2;
                            break;
                        }
                    }
                    break;
                case 0x15:
                    C8S->delay_timer = C8S->V[second_nib];
                    C8S->PC += 2;
                    break;
                case 0x18:
                    C8S->sound_timer = C8S->V[second_nib];
                    C8S->PC += 2;
                    break;
                case 0x1E:
                    C8S->I += C8S->V[second_nib];
                    C8S->PC += 2;
                    break;
                case 0x29:
                    C8S->I = 0x50 + (5 * C8S->V[second_nib]);
                    C8S->PC += 2;
                    break;
                case 0x33:
                    C8S->memory[C8S->I] = C8S->V[second_nib] / 100;
                    C8S->memory[C8S->I + 1] = (C8S->V[second_nib] / 10) % 10;
                    C8S->memory[C8S->I + 2] = (C8S->V[second_nib] % 10);
                    C8S->PC += 2;
                    break;
                case 0x55:
                    for(unsigned int i = 0; i <= second_nib; ++i){
                        C8S->memory[C8S->I + i] = C8S->V[i];
                    }
                    C8S->PC += 2;
                    break;
                case 0x65:
                    for(unsigned int i = 0; i <= second_nib; ++i){
                        C8S->V[i] = C8S->memory[C8S->I + i];
                    }
                    C8S->PC += 2;
                    break;
            }
            break;
    }
}

void Chip8Handler::graphicsHandler(uint8_t X, uint8_t Y, uint8_t height){
    C8S->V[15] = 0;
    uint8_t pixel;

    for(unsigned int y = 0; y < height; ++y){
        pixel = C8S->memory[C8S->I + y];
        for(unsigned int x = 0; x < 8; ++x){
            if((pixel & (0x80 >> x)) != 0){
                if(C8S->gfx[((Y+y)) * 64 + ((X+x))] == 1) C8S->V[15] = 1;
                C8S->gfx[((Y+y)) * 64 + ((X+x))] ^= 1;
            }
        }
    }

    drawMode = true;
}

void Chip8Handler::timerHandler(){
    if(C8S->delay_timer > 0){ --C8S->delay_timer; }
    if(C8S->sound_timer > 0){ --C8S->sound_timer; /*printf("\nBEEP\n"); */ }
}

// Debugging
void Chip8Handler::instructionDebugging(){
    uint8_t *opcode = &C8S->memory[C8S->PC];
    uint16_t full_opcode = (opcode[0] << 8) | opcode[1];
    printf("full opcode: 0x%04x\n", full_opcode);

    uint8_t second_nib = (opcode[0] & 0x0F);
    uint8_t third_nib  = (opcode[1] >> 4);

    switch(full_opcode & 0xF000){
        case 0x0: 
            if(opcode[1] == 0xE0)       printf("0x%04x: CLEAR DISPLAY\n", C8S->PC); 
            else if(opcode[1] == 0xEE)  printf("0x%04x: RET %04x\n", C8S->PC, C8S->stack[C8S->sp - 1]); 
            else printf("0x%04x: UNKNOWN OPCODE!\n", C8S->PC); 
            break;
        case 0x1000:
            printf("0x%04x: JMP %04x\n", C8S->PC, (uint16_t)(0x00 | (second_nib << 8) | (third_nib << 4) | full_opcode & 0x000F)); break;
        case 0x2000:
            printf("0x%04x: CALL %04x\n", C8S->PC, (uint16_t)(0x00 | (second_nib << 8) | (third_nib << 4) | full_opcode & 0x000F)); break;
        case 0x3000: 
            printf("0x%04x: SKIP IF EQUAL V%d, %02x\n", C8S->PC, second_nib, opcode[1]); break;
        case 0x4000: 
            printf("0x%04x: SKIP IF NOT EQUAL V%d, %02x\n", C8S->PC, second_nib, opcode[1]); break;
        case 0x5000: 
            printf("0x%04x: SKIP IF EQUAL V%d, V%d\n", C8S->PC, second_nib, third_nib); break;
        case 0x6000:
            printf("0x%04x: MOVE V%d, %02x\n", C8S->PC, second_nib, opcode[1]); break;
        case 0x7000:
            printf("0x%04x: ADD V%d, %02x\n", C8S->PC, second_nib, opcode[1]); break;
        case 0x8000: 
            if(full_opcode & 0x000F == 0x0)           printf("0x%04x: MOV V%d, V%d\n", C8S->PC, second_nib, third_nib);
            else if((full_opcode & 0x000F) == 0x1)    printf("0x%04x: OR V%d, V%d\n", C8S->PC, second_nib, third_nib);
            else if((full_opcode & 0x000F) == 0x2)    printf("0x%04x: AND V%d, V%d\n", C8S->PC, second_nib, third_nib);
            else if((full_opcode & 0x000F) == 0x3)    printf("0x%04x: XOR V%d, V%d\n", C8S->PC, second_nib, third_nib);
            else if((full_opcode & 0x000F) == 0x4)    printf("0x%04x: ADD V%d, V%d\n", C8S->PC, second_nib, third_nib);
            else if((full_opcode & 0x000F) == 0x5)    printf("0x%04x: SUB V%d, V%d\n", C8S->PC, second_nib, third_nib);
            else if((full_opcode & 0x000F) == 0x6)    printf("0x%04x: SHR V%d\n", C8S->PC, second_nib);
            else if((full_opcode & 0x000F) == 0x7)    printf("0x%04x: SUB V%d, V%d\n", C8S->PC, third_nib, second_nib);
            else if((full_opcode & 0x000F) == 0xE)    printf("0x%04x: SHL V%d\n", C8S->PC, second_nib);
            else printf("0x%04x: UNKNOWN OPCODE!\n", C8S->PC);
            break;
        case 0x9000:
            printf("0x%04x: SKIP IF NOT EQUAL V%d, V%d\n", C8S->PC, second_nib, third_nib); break;
        case 0xA000: 
            printf("0x%04x: MOVE I, %03x\n", C8S->PC, (uint16_t)(0x00 | (second_nib << 8) | (third_nib << 4) | full_opcode & 0x000F)); break;
        case 0xB000:
            printf("0x%04x: JMPV %03x\n", C8S->PC, (uint16_t)(0x00 | (second_nib << 8) | (third_nib << 4) | full_opcode & 0x000F) + C8S->V[0]); break;
        case 0xC000: 
            printf("0x%04x: RAND %02x\n", C8S->PC, opcode[1]); break;
        case 0xD000:
            printf("0x%04x: GRAPH (%02x, %02x), %d\n", C8S->PC, C8S->V[second_nib], C8S->V[third_nib], full_opcode & 0x000F); break;
        case 0xE000:
            if(opcode[1] == 0x9E)       printf("0x%04x: KEYPRESSED %02x\n", C8S->PC, C8S->V[second_nib]);
            else if(opcode[1] == 0xA1)  printf("0x%04x: NOT KEYPRESSED %02x\n", C8S->PC, C8S->V[second_nib]);
            else printf("0x%04x: UNKNOWN OPCODE!\n", C8S->PC);
            break;
        case 0xF000: 
            if(opcode[1] == 0x07)       printf("0x%04x: MOV DTIMER, V%d\n", C8S->PC, second_nib);
            else if(opcode[1] == 0x0A)  printf("0x%04x: WAIT KEYPRESS\n", C8S->PC);
            else if(opcode[1] == 0x15)  printf("0x%04x: MOV V%d, DTIMER\n", C8S->PC, second_nib);
            else if(opcode[1] == 0x18)  printf("0x%04x: MOV V%d, STIMER\n", C8S->PC, second_nib);
            else if(opcode[1] == 0x1E)  printf("0x%04x: ADD I, V%d\n", C8S->PC, second_nib);
            else if(opcode[1] == 0x29)  printf("0x%04x: SPRITE I->0x%02x\n", C8S->PC, (0x50 + (5 * C8S->V[second_nib])));
            else if(opcode[1] == 0x33)  printf("0x%04x: BCD %02x, #$%04x\n", C8S->PC, C8S->V[second_nib], C8S->I);
            else if(opcode[1] == 0x55)  printf("0x%04x: ALLV, #$%02x\n", C8S->PC, C8S->V[second_nib], C8S->I);
            else if(opcode[1] == 0x65)  printf("0x%04x: #$%02x, ALLV\n", C8S->PC, C8S->V[second_nib], C8S->I);
            else printf("0x%04x: UNKNOWN OPCODE!\n", C8S->PC);
            break;
    }
}

// Reset
void Chip8Handler::resetGraphicsMemory(){
    memset(C8S->gfx, 0, 64 * 32);
    drawMode = true;
}

void Chip8Handler::resetMemory(){
    memset(C8S->memory, 0, 4096);
}

void Chip8Handler::resetStack(){
    for(int i = 0; i < 16; ++i){
        C8S->stack[i] = 0; 
    }
}