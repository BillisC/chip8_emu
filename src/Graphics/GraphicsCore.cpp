#include "../../include/Graphics/GraphicsCore.h"

GraphicsHandler::GraphicsHandler(const unsigned int WIDTH, const unsigned int HEIGHT): SCR_WIDTH(WIDTH), SCR_HEIGHT(HEIGHT){
    memset(scr_buffer, 0, 64 * 32);

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL INIT FAILURE: %s\n", SDL_GetError());
        FAILURE = true;
    }
    else{
        window = SDL_CreateWindow("segs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == nullptr){
            printf("SDL WINDOW FAILURE: %s\n", SDL_GetError());
        }
        else{
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL){
                printf("SDL RENDERER FAILURE: %s\n", SDL_GetError());
            }
            else{
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
                SDL_RenderSetLogicalSize(renderer, SCR_WIDTH, SCR_HEIGHT);
                texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
            }
        }
    }
}

GraphicsHandler::~GraphicsHandler(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();
}

void GraphicsHandler::updateGFXBuffer(uint8_t *gfx){
    for (int i = 0; i < 2048; ++i){
        scr_buffer[i] = (0x00FFFFFF * gfx[i]) | 0xFF000000;
    }

    SDL_UpdateTexture(texture, NULL, scr_buffer, 64 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int GraphicsHandler::EventCheck(uint8_t *keypresses){
    SDL_Event ev;

    while(SDL_PollEvent(&ev) != 0){
        //User requests quit
        if(ev.type == SDL_QUIT){
            return 1;
        }
        else if(ev.type == SDL_KEYDOWN){
            switch(ev.key.keysym.sym){
                case SDLK_1: keypresses[0x1] = 1; break;
                case SDLK_2: keypresses[0x2] = 1; break;
                case SDLK_3: keypresses[0x3] = 1; break;
                case SDLK_4: keypresses[0xC] = 1; break;
                case SDLK_q: keypresses[0x4] = 1; break;
                case SDLK_w: keypresses[0x5] = 1; break;
                case SDLK_e: keypresses[0x6] = 1; break;
                case SDLK_r: keypresses[0xD] = 1; break;
                case SDLK_a: keypresses[0x7] = 1; break;
                case SDLK_s: keypresses[0x8] = 1; break;
                case SDLK_d: keypresses[0x9] = 1; break;
                case SDLK_f: keypresses[0xE] = 1; break;
                case SDLK_z: keypresses[0xA] = 1; break;
                case SDLK_x: keypresses[0x0] = 1; break;
                case SDLK_c: keypresses[0xB] = 1; break;
                case SDLK_v: keypresses[0xF] = 1; break;
            }
        }
        else if(ev.type == SDL_KEYUP){
            switch(ev.key.keysym.sym){
                case SDLK_1: keypresses[0x1] = 0; break;
                case SDLK_2: keypresses[0x2] = 0; break;
                case SDLK_3: keypresses[0x3] = 0; break;
                case SDLK_4: keypresses[0xC] = 0; break;
                case SDLK_q: keypresses[0x4] = 0; break;
                case SDLK_w: keypresses[0x5] = 0; break;
                case SDLK_e: keypresses[0x6] = 0; break;
                case SDLK_r: keypresses[0xD] = 0; break;
                case SDLK_a: keypresses[0x7] = 0; break;
                case SDLK_s: keypresses[0x8] = 0; break;
                case SDLK_d: keypresses[0x9] = 0; break;
                case SDLK_f: keypresses[0xE] = 0; break;
                case SDLK_z: keypresses[0xA] = 0; break;
                case SDLK_x: keypresses[0x0] = 0; break;
                case SDLK_c: keypresses[0xB] = 0; break;
                case SDLK_v: keypresses[0xF] = 0; break;
            }
        }
    }

    return 0;
}