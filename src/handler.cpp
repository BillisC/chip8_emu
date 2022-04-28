#include "../include/handler.h"

#ifndef OP_NIBB
#define OP_NIBB     ((full_opcode & 0xF000u) >> 12)
#define SEC_BYTE    (full_opcode & 0x00FFu)
#define ADDR_BYTE   (full_opcode & 0x0FFFu)
#define SEC_NIBB    ((full_opcode & 0x0F00u) >> 8)
#define TRI_NIBB    ((full_opcode & 0x00F0u) >> 4)
#define FTH_NIBB    (full_opcode & 0x000Fu)
#endif

// _SysInit_
void Chip8Handler::init(){
    C8S->clock_speed = 1000000 / 600; // Default clock speed is 600Hz
    
    C8S->PC = 0x200;
    C8S->I = 0x00;
    C8S->sp = 0x00;

    // Clear registers 
    memset(C8S->V, 0, 16);
    // Clear keyboard 
    memset(C8S->key, 0, 16);    
    // Clear memory 
    resetMemory();
    // Clear graphics buffer
    resetGraphicsMemory();
    // Clear stack
    memset(C8S->stack, 0, 16);

    // Load font
    fontLoader();

    // Initialize function tables
    opTAB0.fill(&Chip8Handler::OP_NULL);
    opTAB0[0x0] = &Chip8Handler::CLR;
    opTAB0[0xE] = &Chip8Handler::RET;

    opTAB8.fill(&Chip8Handler::OP_NULL);
    opTAB8 = { &Chip8Handler::ASGN, &Chip8Handler::OR, &Chip8Handler::AND, &Chip8Handler::XOR,
               &Chip8Handler::ADD, &Chip8Handler::SUB, &Chip8Handler::SHR, &Chip8Handler::SUBR };
    opTAB8[0xE] = &Chip8Handler::SHL;
    
    opTABE.fill(&Chip8Handler::OP_NULL);
    opTABE[0xE] = &Chip8Handler::KPR;
    opTABE[0x1] = &Chip8Handler::NKPR;

    opTABF.fill(&Chip8Handler::OP_NULL);
    opTABF[0x07] = &Chip8Handler::STRT;
    opTABF[0x0A] = &Chip8Handler::WKPR;
    opTABF[0x15] = &Chip8Handler::SETT;
    opTABF[0x18] = &Chip8Handler::SNDT;
    opTABF[0x1E] = &Chip8Handler::ADDI;
    opTABF[0x29] = &Chip8Handler::SPRI;
    opTABF[0x33] = &Chip8Handler::BCD;
    opTABF[0x55] = &Chip8Handler::STRM;
    opTABF[0x65] = &Chip8Handler::STRR;

    opTAB = { &Chip8Handler::fTAB0, &Chip8Handler::JMP, &Chip8Handler::CALL, &Chip8Handler::SEQ, 
              &Chip8Handler::SNEQ, &Chip8Handler::SREQ, &Chip8Handler::LDNN, &Chip8Handler::ADDNN,
              &Chip8Handler::fTAB8, &Chip8Handler::SRNEQ, &Chip8Handler::SETI, &Chip8Handler::JMPO,
              &Chip8Handler::RAND, &Chip8Handler::DRAW, &Chip8Handler::fTABE, &Chip8Handler::fTABF };
}

int Chip8Handler::romLoader(const std::string file_name){
    std::fstream rom;
    
    rom.open(file_name, std::ios::in | std::ios::binary);
    if(!rom.is_open()){
        printf("\nFailed to load file"); 
        return -1; 
    }

    // Get the file's size
    rom.seekg(0, std::ios::end);
    size_t file_size = rom.tellg();
    rom.seekg(0, std::ios::beg);

    // Load the file in the memory
    rom.read(reinterpret_cast<char *>(C8S->memory + 0x200), file_size);
    printf("\nROM Size: %d\n", file_size);
    
    rom.close();
    return 0;
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

// Emulation
void Chip8Handler::clock(){
    // Fetch and execute
    full_opcode = (C8S->memory[C8S->PC] << 8) | (C8S->memory[C8S->PC + 1]);
    ((*this).*(opTAB[OP_NIBB]))();
    
    timing();
}
void Chip8Handler::graphics(uint8_t X, uint8_t Y, uint8_t height){
    C8S->V[15] = 0;
    uint8_t pixel;

    for(unsigned int off_y = 0; off_y < height; ++off_y){
        pixel = C8S->memory[C8S->I + off_y];
        for(unsigned int off_x = 0; off_x < 8; ++off_x){
            if((pixel & (0x80 >> off_x)) != 0){
                if(C8S->gfx[((Y+off_y)) * 64 + ((X+off_x))] == 1) C8S->V[15] = 1;
                C8S->gfx[((Y+off_y)) * 64 + ((X+off_x))] ^= 1;
            }
        }
    }

    drawMode = true;
}
void Chip8Handler::timing(){
    if(C8S->delay_timer > 0){ --C8S->delay_timer; }
    if(C8S->sound_timer > 0){ --C8S->sound_timer; }  // Normally plays a sound
}

// Instruction Set (sorted by address)
void Chip8Handler::CLR(){ 
    resetGraphicsMemory(); 
    C8S->PC += 2; 
}
void Chip8Handler::RET(){ 
    C8S->PC = C8S->stack[--C8S->sp]; 
}
void Chip8Handler::JMP(){ 
    C8S->PC = ADDR_BYTE; 
}
void Chip8Handler::CALL(){ 
    C8S->stack[C8S->sp++] = C8S->PC + 2; 
    C8S->PC = ADDR_BYTE;
}
void Chip8Handler::SEQ(){ 
    if(C8S->V[SEC_NIBB] == SEC_BYTE) C8S->PC += 2;
    C8S->PC += 2; 
}
void Chip8Handler::SNEQ(){
    if(C8S->V[SEC_NIBB] != SEC_BYTE) C8S->PC += 2; 
    C8S->PC += 2; 
}
void Chip8Handler::SREQ(){ 
    if(C8S->V[SEC_NIBB] == C8S->V[TRI_NIBB]) C8S->PC += 2; 
    C8S->PC += 2; 
}
void Chip8Handler::LDNN(){ 
    C8S->V[SEC_NIBB] = SEC_BYTE; 
    C8S->PC += 2; 
}
void Chip8Handler::ADDNN(){ 
    C8S->V[SEC_NIBB] += SEC_BYTE; 
    C8S->PC += 2; 
}
void Chip8Handler::ASGN(){ 
    C8S->V[SEC_NIBB] = C8S->V[TRI_NIBB]; 
    C8S->PC += 2; 
}
void Chip8Handler::OR(){ 
    C8S->V[SEC_NIBB] |= C8S->V[TRI_NIBB]; 
    C8S->PC += 2;
}
void Chip8Handler::AND(){ 
    C8S->V[SEC_NIBB] &= C8S->V[TRI_NIBB]; 
    C8S->PC += 2; 
}
void Chip8Handler::XOR(){ 
    C8S->V[SEC_NIBB] ^= C8S->V[TRI_NIBB]; 
    C8S->PC += 2; 
}
void Chip8Handler::ADD(){ 
    uint8_t res = C8S->V[SEC_NIBB] + C8S->V[TRI_NIBB];
    C8S->V[SEC_NIBB] = res;
    C8S->V[15] = res > C8S->V[SEC_NIBB];
    C8S->PC += 2; 
}
void Chip8Handler::SUB(){ 
    C8S->V[15] = C8S->V[SEC_NIBB] > C8S->V[TRI_NIBB];
    C8S->V[SEC_NIBB] -= C8S->V[TRI_NIBB];
    C8S->PC += 2; 
}
void Chip8Handler::SHR(){ 
    C8S->V[15] = (C8S->V[SEC_NIBB] & 0x1u);
    C8S->V[SEC_NIBB] >>= 1;
    C8S->PC += 2; 
}
void Chip8Handler::SUBR(){ 
    C8S->V[SEC_NIBB] = C8S->V[TRI_NIBB] - C8S->V[SEC_NIBB];
    C8S->V[15] = (C8S->V[TRI_NIBB] > C8S->V[SEC_NIBB]);
    C8S->PC += 2; 
}
void Chip8Handler::SHL(){ 
    C8S->V[15] = (C8S->V[SEC_NIBB] & 0x80u) >> 7u;
    C8S->V[SEC_NIBB] <<= 1;
    C8S->PC += 2; 
}
void Chip8Handler::SRNEQ(){ 
    if(C8S->V[SEC_NIBB] != C8S->V[TRI_NIBB]) C8S->PC += 2;
    C8S->PC += 2; 
}
void Chip8Handler::SETI(){
    C8S->I = ADDR_BYTE; 
    C8S->PC += 2;
}
void Chip8Handler::JMPO(){ 
    C8S->PC = ADDR_BYTE + C8S->V[0]; 
}
void Chip8Handler::RAND(){ 
    C8S->V[SEC_NIBB] = (uint8_t)(rand() % 256) & SEC_BYTE; 
    C8S->PC += 2; 
}
void Chip8Handler::DRAW(){ 
    graphics(C8S->V[SEC_NIBB], C8S->V[TRI_NIBB], FTH_NIBB); 
    C8S->PC += 2; 
}
void Chip8Handler::KPR(){ 
    if(C8S->key[C8S->V[SEC_NIBB]] != 0) C8S->PC += 2;
    C8S->PC += 2; 
}
void Chip8Handler::NKPR(){ 
    if(C8S->key[C8S->V[SEC_NIBB]] == 0) C8S->PC += 2;
    C8S->PC += 2; 
}
void Chip8Handler::STRT(){ 
    C8S->V[SEC_NIBB] = C8S->delay_timer; 
    C8S->PC += 2; 
}
void Chip8Handler::WKPR(){ 
    for(unsigned int i = 0; i < 0xF; ++i){
        if(C8S->key[i]){
            C8S->V[SEC_NIBB] = C8S->key[i];
            C8S->PC += 2;
        }
    }
}
void Chip8Handler::SETT(){ 
    C8S->delay_timer = C8S->V[SEC_NIBB]; 
    C8S->PC += 2; 
}
void Chip8Handler::SNDT(){ 
    C8S->sound_timer = C8S->V[SEC_NIBB];
    C8S->PC += 2;
}
void Chip8Handler::ADDI(){ 
    C8S->I += C8S->V[SEC_NIBB]; 
    C8S->PC += 2; 
}
void Chip8Handler::SPRI(){ 
    C8S->I = 0x50 + (5 * C8S->V[SEC_NIBB]); 
    C8S->PC += 2; 
}
void Chip8Handler::BCD(){ 
    C8S->memory[C8S->I] = C8S->V[SEC_NIBB] / 100;
    C8S->memory[C8S->I + 1] = (C8S->V[SEC_NIBB] / 10) % 10;
    C8S->memory[C8S->I + 2] = (C8S->V[SEC_NIBB] % 10);
    C8S->PC += 2; 
}
void Chip8Handler::STRM(){ 
    for(unsigned int i = 0; i <= SEC_NIBB; ++i){
        C8S->memory[C8S->I + i] = C8S->V[i];
    }
    C8S->PC += 2;
}
void Chip8Handler::STRR(){
    for(unsigned int i = 0; i <= SEC_NIBB; ++i){
        C8S->V[i] = C8S->memory[C8S->I + i];
    }
    C8S->PC += 2;
}

// Resetting
void Chip8Handler::resetGraphicsMemory(){
    memset(C8S->gfx, 0, 64 * 32);
    drawMode = true;
}
void Chip8Handler::resetMemory(){
    memset(C8S->memory, 0, 4096);
}