#ifndef WINDOWCORE_H
#define WINDOWCORE_H

#include <SDL2/SDL.h>

class WindowHandler{
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    bool FAILURE{false};

    uint32_t scr_buffer[64 * 32];

    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    SDL_Texture *texture{nullptr};
    SDL_Event ev;

public:
    WindowHandler(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);
    ~WindowHandler();

    void updateGFXBuffer(uint8_t *gfx);

    int EventCheck(uint8_t *keypresses);

    bool failCheck(){ return FAILURE; }
};

#endif