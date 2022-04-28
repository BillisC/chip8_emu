#ifndef CHIP8HANDLER_H
#define CHIP8HANDLER_H

#include <cstring>
#include <fstream>
#include <array>

#include "chip8.h"

class Chip8Handler{
    Chip8System *C8S{nullptr};

public:
    Chip8Handler(Chip8System *C8S){ this->C8S = C8S; init(); }
    ~Chip8Handler(){ delete C8S; }
    
    uint16_t full_opcode{0x0};
    bool drawMode{false}; // Block drawing

    /* _SysInit_ */ int romLoader(const std::string file_name);
    /* Emulation */ void clock();

private:
    /* _SysInit_ */ void init();
    /* _SysInit_ */ void fontLoader();
    
    /* Emulation */ void graphics(uint8_t X, uint8_t Y, uint8_t height);
    /* Emulation */ void timing();

    /* Resetting */ void resetGraphicsMemory();
    /* Resetting */ void resetMemory();

private:
    /* System calls from 0x0NNN have been deprecated except from the display ones */  
    /* Assig */ void ASGN();
    /* BitOp */ void OR(); void XOR(); void AND(); void SHR(); void SHL();
    /* Const */ void LDNN(); void ADDNN();
    /* KeyOp */ void KPR(); void NKPR(); void WKPR();
    /* Timer */ void SNDT(); void SETT(); void STRT();
    /* Cond */  void SEQ(); void SNEQ(); void SREQ(); void SRNEQ();
    /* Disp */  void CLR(); void DRAW();
    /* Flow */  void RET(); void JMP(); void CALL(); void JMPO();
    /* Math */  void ADD(); void SUB(); void SUBR();
    /* Null */  void OP_NULL(){ /* Do Nothing */ }
    /* BCD */   void BCD();
    /* Mem */   void SETI(); void ADDI(); void SPRI(); void STRM(); void STRR();
    /* Rand */  void RAND();
 
private:
    /* Jump tables */
    typedef void (Chip8Handler::*ptrFunc)();
    std::array<ptrFunc, 0x0F> opTAB0;  /* Initialized in constructor */
    std::array<ptrFunc, 0x0F> opTAB8;  /* Initialized in constructor */
    std::array<ptrFunc, 0x0F> opTABE;  /* Initialized in constructor */
    std::array<ptrFunc, 0x66> opTABF;  /* Initialized in constructor */
    std::array<ptrFunc, 0x10> opTAB;   /* Initialized in constructor */

    void fTAB0(){ ((*this).*(opTAB0[full_opcode & 0x000Fu]))(); }
    void fTAB8(){ ((*this).*(opTAB8[full_opcode & 0x000Fu]))(); }
    void fTABE(){ ((*this).*(opTABE[full_opcode & 0x000Fu]))(); }
    void fTABF(){ ((*this).*(opTABF[full_opcode & 0x00FFu]))(); }
};

#endif