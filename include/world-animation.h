#ifndef H_WORLD_ANIMATION_H
#define H_WORLD_ANIMATION_H
#include <SDL2/SDL.h>
#include "evolution.h"

int animate_world(struct world *world);

struct animation{
    struct world *const world;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};


#endif /*H_WORLD_ANIMATION_H*/