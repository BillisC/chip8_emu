#ifndef GRAPHICSCORE_H
#define GRAPHICSCORE_H

#include <SDL2/SDL.h>

class GraphicsHandler{
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    
    uint32_t scr_buffer[64 * 32];

    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    SDL_Texture *texture{nullptr};
    
    bool FAILURE{false};

public:
    GraphicsHandler(const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);
    ~GraphicsHandler();

    void updateGFXBuffer(uint8_t *gfx);

    int EventCheck(uint8_t *keypresses);
};

#endif