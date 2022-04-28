#include "../include/debugger.h"

#ifndef OP_NIBB
#define OP_NIBB     ((full_opcode & 0xF000u) >> 12)
#define SEC_BYTE    (full_opcode & 0x00FFu)
#define ADDR_BYTE   (full_opcode & 0x0FFFu)
#define SEC_NIBB    ((full_opcode & 0x0F00u) >> 8)
#define TRI_NIBB    ((full_opcode & 0x00F0u) >> 4)
#define FTH_NIBB    (full_opcode & 0x000Fu)
#endif

// Terminal Output
void Chip8Debugger::opcodeAnalyser(const uint16_t &full_opcode, const uint16_t &pc){
    printf("%04x: ", pc);

    switch(OP_NIBB){
        case 0x0: 
            if(SEC_BYTE == 0xE0)       printf("CLR\n"); 
            else if(SEC_BYTE == 0xEE)  printf("RET %04x\n", C8S->stack[C8S->sp - 1]); 
            else printf("UNKNOWN OPCODE!\n"); break;
        case 0x1: printf("JMP %04x\n", ADDR_BYTE); break;
        case 0x2: printf("CALL %04x\n", ADDR_BYTE); break;
        case 0x3: printf("SEQ V%d, %02x\n", SEC_NIBB, SEC_BYTE); break;
        case 0x4: printf("SNEQ V%d, %02x\n", SEC_NIBB, SEC_BYTE); break;
        case 0x5: printf("SEQ V%d, V%d\n", SEC_NIBB, TRI_NIBB); break;
        case 0x6: printf("LD V%d, %02x\n", SEC_NIBB, SEC_BYTE); break;
        case 0x7: printf("ADD V%d, %02x\n", SEC_NIBB, SEC_BYTE); break;
        case 0x8: 
            if(FTH_NIBB == 0x0)         printf("MOV V%d, V%d\n", SEC_NIBB, TRI_NIBB);
            else if(FTH_NIBB == 0x1)    printf("OR V%d, V%d\n",  SEC_NIBB, TRI_NIBB);
            else if(FTH_NIBB == 0x2)    printf("AND V%d, V%d\n", SEC_NIBB, TRI_NIBB);
            else if(FTH_NIBB == 0x3)    printf("XOR V%d, V%d\n", SEC_NIBB, TRI_NIBB);
            else if(FTH_NIBB == 0x4)    printf("ADD V%d, V%d\n", SEC_NIBB, TRI_NIBB);
            else if(FTH_NIBB == 0x5)    printf("SUB V%d, V%d\n", SEC_NIBB, TRI_NIBB);
            else if(FTH_NIBB == 0x6)    printf("SHR V%d\n",      SEC_NIBB);
            else if(FTH_NIBB == 0x7)    printf("SUB V%d, V%d\n", TRI_NIBB, SEC_NIBB);
            else if(FTH_NIBB == 0xE)    printf("SHL V%d\n",      SEC_NIBB);
            else printf("UNKNOWN OPCODE!\n"); break;
        case 0x9: printf("SNEQ V%d, V%d\n", C8S->PC, SEC_NIBB, TRI_NIBB); break;
        case 0xA: printf("MOV I, %03x\n", ADDR_BYTE); break;
        case 0xB: printf("JMP %03x\n", ADDR_BYTE + C8S->V[0]); break;
        case 0xC: printf("RAND %02x\n", SEC_BYTE); break;
        case 0xD: printf("GRAPH (%02x, %02x), %d\n", C8S->V[SEC_NIBB], C8S->V[TRI_NIBB], FTH_NIBB); break;
        case 0xE: 
            if(FTH_NIBB == 0xE)       printf("KEYPRESSED %02x\n", C8S->V[SEC_NIBB]);
            else if(FTH_NIBB == 0x1)  printf("NOT KEYPRESSED %02x\n", C8S->V[SEC_NIBB]);
            else printf("UNKNOWN OPCODE!\n"); break;
        case 0xF: 
            if(SEC_BYTE == 0x07)       printf("MOV DTIMER, V%d\n", SEC_BYTE);
            else if(SEC_BYTE == 0x0A)  printf("WAIT KEYPRESS\n");
            else if(SEC_BYTE == 0x15)  printf("MOV V%d, DTIMER\n", SEC_BYTE);
            else if(SEC_BYTE == 0x18)  printf("MOV V%d, STIMER\n", SEC_BYTE);
            else if(SEC_BYTE == 0x1E)  printf("ADD I, V%d\n", SEC_BYTE);
            else if(SEC_BYTE == 0x29)  printf("SPR I->0x%02x\n", (0x50 + (5 * C8S->V[SEC_NIBB])));
            else if(SEC_BYTE == 0x33)  printf("BCD %02x, $%04x\n", C8S->V[SEC_BYTE], C8S->I);
            else if(SEC_BYTE == 0x55)  printf("STRM $%04x", C8S->I);
            else if(SEC_BYTE == 0x65)  printf("STRR $%04x", C8S->I);
            else printf("UNKNOWN OPCODE!\n"); break;
    }
}

// File Output
void Chip8Debugger::memoryDUMP(){
    std::fstream dump;
    dump.open("debug/memory_dump.bin", std::ios::out | std::ios::binary);
    if(dump){
        for(unsigned int i = 0; i < 4096; ++i){
            dump.write(reinterpret_cast<char *>(&C8S->memory[i]), 1);
        }
        dump.close();
    }
    else{
        printf("I/O ERROR: Memory dump file issue");
    }
}

void Chip8Debugger::graphicsMemoryDUMP(){
    std::fstream dump;
    dump.open("debug/graphics_memory_dump.bin", std::ios::out | std::ios::binary);
    if(dump){
        for(unsigned int i = 0; i < 2048; ++i){
            dump.write(reinterpret_cast<char *>(&C8S->gfx[i]), 1);
        }
        dump.close();
    }
    else{
        printf("I/O ERROR: Graphics memory dump file issue");
    }
}