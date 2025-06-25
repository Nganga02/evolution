#include <stdlib.h>

#include "world-animation.h"
#include "evolution.h"

#define WINDOW_HEIGHT(world_ptr) ((world_ptr)->world_h)
#define WINDOW_WIDTH(world_ptr) ((world_ptr)->world_w)


/*
Takes the world struct and the number of iterations the population should go through
*/
int animate_world(struct world *world, unsigned long n)
{
    struct animation World_animation = {
        .world = world,
        .window = NULL,
        .renderer = NULL,
        .texture = NULL,
    };

    struct animation *world_animation = &World_animation;//For easy accessing

    if (sdl_init(world_animation))
    {
        fprintf(stderr, "There was an error at initialization");
        animation_cleanup(world_animation, EXIT_FAILURE);
    }

    SDL_FRect eden;/*Used to render eden*/
    SDL_FPoint animals;/*Used to render animal */
    SDL_FPoint plants;/*Used to render plants*/
    unsigned long i = 0;


    int eden_x = ((world->world_w - 1) - world->eden_w)/2;
    int eden_y = ((world->world_h - 1) - world->eden_h)/2;
    Conscell *a = world->herd;


    while(i < n){
        SDL_RenderClear(world_animation->renderer);
        SDL_SetRenderDrawColor(world_animation->renderer, 255, 255, 255,
             SDL_ALPHA_OPAQUE); //rendering the world 
        
        //Rendering eden of the Eden
        SDL_SetRenderDrawColor(world_animation->renderer, 0, 0, 255, 
            SDL_ALPHA_OPAQUE);
        eden.x = eden_x;
        eden.y = eden_y;
        eden.w = world->eden_w;
        eden.h = world->eden_h;
        SDL_RenderFillRect(world_animation->renderer, &eden);

        //Rendering the animals 
        while (a != NULL) {
            animal *anim = (animal *)a->data;
            SDL_SetRenderDrawColor(world_animation->renderer, 150, 75, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(world_animation->renderer, anim->i, anim->j);
            a = a->next;
        }  


        //Rendering the plants
        for(int i = 0; i < world->world_h; i++)
        {
            for(int j = 0; j < world->world_w; j++)
            {
                if(world->plants > 0)
                {
                    SDL_SetRenderDrawColor(world_animation->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(world_animation->renderer, j, i);
                }
            }
        }

    }
}


/*
We are initializing the sdl video
Return a value of 0 on success and a 1 on failure
*/
static int sdl_init(struct animation *animation)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Error initialising video: %s\n", SDL_GetError());
        return 1;
    }

    animation->window = SDL_CreateWindow("Evolution",
                                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                         WINDOW_WIDTH(animation->world), WORLD_HEIGHT(animation->world), 0);

    if (!animation->window)
    {
        fprintf(stderr, "Error creating animation window: %s\n", SDL_GetError());
        return 1;
    }

    animation->renderer = SDL_CreateRenderer(animation->window, -1, 0);

    if (!animation->renderer)
    {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}


/*
Used to clean up memory on the event of failure or when closing the animation
Accepts to arguments animation and exit_status
*/
void animation_cleanup(struct animation *animation, int exit_status)
{
    SDL_DestroyTexture(animation->texture);
    SDL_DestroyRenderer(animation->renderer);
    SDL_DestroyWindow(animation->window);
    SDL_Quit();
    free(animation->world);
    exit(exit_status);
}
