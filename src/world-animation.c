#include <stdlib.h>

#include "world-animation.h"
#include "evolution.h"

#define WINDOW_HEIGHT(world_ptr) ((world_ptr)->world_h)
#define WINDOW_WIDTH(world_ptr) ((world_ptr)->world_w)


int sdl_initialize(struct animation *animation);
void animation_cleanup(struct animation *animation, int exit_status);


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

    struct animation *world_animation = &World_animation; // For easy accessing

    if (sdl_initialize(world_animation))
    {
        fprintf(stderr, "There was an error at initialization");
        animation_cleanup(world_animation, EXIT_FAILURE);
    }

    SDL_FRect eden;     /*Used to render eden*/
    // SDL_FPoint animals; /*Used to render animal */
    // SDL_FPoint plants;  /*Used to render plants*/
    unsigned long i = 0;

    int eden_x = ((world->world_w - 1) - world->eden_w) / 2;
    int eden_y = ((world->world_h - 1) - world->eden_h) / 2;
    Conscell *a = world->herd;

    eden.x = eden_x;
        eden.y = eden_y;
        eden.w = world->eden_w;
        eden.h = world->eden_h;

    const SDL_FRect *eden_ptr = &eden;


    while (i < n)
    {

        printf("This is the value of i: %lu \n", i);
        SDL_Event event;
        while(SDL_PollEvent(&event))//The sdl_pollevent function expects a pointer to an event
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    animation_cleanup(world_animation, EXIT_SUCCESS);
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            animation_cleanup(world_animation, EXIT_SUCCESS);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        update_world(world);

        SDL_SetRenderDrawColor(world_animation->renderer, 0, 0, 0,
                               SDL_ALPHA_OPAQUE); // rendering the world
        SDL_RenderClear(world_animation->renderer);

        // Rendering eden of the Eden
        SDL_SetRenderDrawColor(world_animation->renderer, 0, 0, 255,
                               SDL_ALPHA_OPAQUE);
        
        SDL_RenderFillRectF(world_animation->renderer, eden_ptr);

        // Rendering the animals
        SDL_SetRenderDrawColor(world_animation->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        while (a != NULL)
        {
            animal *anim = (animal *)a->data;
            SDL_RenderDrawPoint(world_animation->renderer, anim->i, anim->j);
            a = a->next;
        }

        // Rendering the plants
        SDL_SetRenderDrawColor(world_animation->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < world->world_h; i++)
        {
            for (int j = 0; j < world->world_w; j++)
            {
                if (world->plants[i][j] > 0)
                {
                    SDL_RenderDrawPoint(world_animation->renderer, j, i);
                }
            }
        }

        SDL_RenderPresent(world_animation->renderer);

        SDL_Delay(50);

        i++;
    }
    animation_cleanup(world_animation, EXIT_SUCCESS);
    
    return 0; //
}

/*
We are initializing the sdl video
Return a value of 0 on success and a 1 on failure
*/
int sdl_initialize(struct animation *animation)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Error initialising video: %s\n", SDL_GetError());
        return 1;
    }

    animation->window = SDL_CreateWindow("Evolution",
                                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                         WINDOW_WIDTH(animation->world), WINDOW_HEIGHT(animation->world), 0);

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
    exit(exit_status);
}
