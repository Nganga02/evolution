#include <stdio.h>

#include "write.h"
#include "linked-list.h"

static void print_animal(animal *animal, FILE *file)
{
    fprintf(file, "(%d %d) %d %d | %d %d %d %d %d %d %d %d|",
            animal->i, animal->j, animal->d, animal->e,
            animal->genes[0], animal->genes[1], animal->genes[2],
            animal->genes[3], animal->genes[4], animal->genes[5],
            animal->genes[6], animal->genes[7]);
            fputc('\n', file);
        }

static void print_herd(Conscell *herd, FILE *file)
{
    if (herd == NULL)
    {   
        printf("the herd is empty");
        fprintf(file, "(0 0) 0 0 | 0 0 0 0 0 0 0 0|");
    }
    else
    {
        for (Conscell *p = herd; p != NULL; p = p->next)
        {
            if (p->data == NULL) {
                fprintf(stderr, "Error: NULL animal in herd.\n");
                continue;
            }
            print_animal(p->data, file);
        }
    }
}

void write_wdf(struct world *world, char *filename)
{
    FILE *file = fopen(filename, "w");
    fprintf(file, "#A World definition for the evolution simulator");
    fputc('\n', file);
    fprintf(file, "World %d %d", world->world_h, world->world_w);
    fputc('\n', file);
    fprintf(file, "Eden %d %d", world->eden_h, world->eden_w);
    fputc('\n', file);
    fprintf(file, "Plant Energy %d", world->plant_energy);
    fputc('\n', file);
    fprintf(file, "Reproduction Threshold %d", world->reproduction_threshold);
    fputc('\n', file);
    fputc('\n', file);
    fprintf(file, "#(i j) d e | g[0] g[1] g[2] g[3] g[4] g[5] g[6] g[7]|");
    fputc('\n', file);
    fprintf(file, "#-------------------|----------------------------------------|");
    fputc('\n', file);
    print_herd(world->herd, file);
    fprintf(file, "# animal count: %d", ll_length(world->herd));
    fclose(file);
}