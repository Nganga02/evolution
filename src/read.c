#include <stdio.h>

#include "read.h"
#include "xmalloc.h"
#include "fetch_line.h"
#include "linked-list.h"
#include "evolution.h"

#define BUFLEN 1024

static int get_world_dimens(struct world *world,char *str, int lineno) 
{
    if(sscanf(str, "World %d %d", &world->world_h, &world->world_w) == 2)
    {
        return 1;
    }
    else
    {
        fprintf(stderr, "stdin:line %d: expected to find World dimensions here\n", lineno);
        return 0;
    }
}
static int get_eden_dimens(struct world *world, char *str, int lineno)
{
    if(sscanf(str, "Eden %d %d", &world->eden_h, &world->eden_w) == 2)
    {
        return 1;
    }
    else
    {
        fprintf(stderr, "stdin:line %d: expected to find Eden dimensions here\n", lineno);
        return 0;
    }
}
static int get_plant_energy(struct world *world, char *str, int lineno)
{
    if(sscanf(str, "Plant Energy %d", &world->plant_energy) == 1)
    {
        return 1;
    }
    else
    {
        fprintf(stderr, "stdin:line %d: expected to find Plant energy here\n", lineno);
        return 0;
    }
}
static int get_reproduction_threshold(struct world *world, char *str, int lineno)
{
    if(sscanf(str, "Reproduction Threshold %d", &world->reproduction_threshold) == 1)
    {   
        return 1;
    }
    else
    {
        fprintf(stderr, "stdin:line %d: expected to find Reproduction Threshold here\n", lineno);
        return 0;
    }
}
static animal *get_animal_specs(char *str, int lineno)
{
    animal *animal;
    int i, j, d, e, r;
    int genes[8];

    r = sscanf(str, "(%d %d) %d %d | %d %d %d %d %d %d %d %d |",
        &i, &j, &d, &e, &genes[0], &genes[1], &genes[2], &genes[3], 
        &genes[4], &genes[5], &genes[6], &genes[7]);

    if(r == 12)
    {
        animal = xmalloc(sizeof *animal);
        animal->i = i;
        animal->j = j;
        animal->d = d;
        animal->e = e;
        for(int a = 0; a < 8; a++)
        {
            animal->genes[a] = genes[a];
        }
        return animal;
    }
    else
    {
        fprintf(stderr, "stdin:line %d: Expected animal specs on this line\n", 
            lineno);
        return NULL;
    }

}


static char *fetch_line_aux(char *buf, int buflen, FILE *stream, int *lineno)
{
    char *s = fetch_line(buf, buflen, stream, lineno);
    if (s == NULL)
        fprintf(stderr, "stdin:line %d: premature end of input\n", *lineno);
    return s;    
}

//This is the function for reading the wdf file 
int read_wdf(struct world *world) 
{
    printf("We are in the read functions\n");
    int animal_count = 0;
    char buf[BUFLEN];
    animal *animal;
    int lineno = 0;
    char *s;

    int result = 
    (s = fetch_line_aux(buf, BUFLEN, stdin, &lineno)) &&
    get_world_dimens(world, s, lineno)                &&
    (s = fetch_line_aux(buf, BUFLEN, stdin, &lineno)) &&
    get_eden_dimens(world, s, lineno)                 &&
    (s = fetch_line_aux(buf, BUFLEN, stdin, &lineno)) &&
    get_plant_energy(world, s, lineno)                &&
    (s = fetch_line_aux(buf, BUFLEN, stdin, &lineno)) &&
    get_reproduction_threshold(world, s, lineno);     //Remember that lineno is only used for debugging

    if(!result)
        return 0;
    while((s = fetch_line_aux(buf, BUFLEN, stdin, &lineno)) != NULL)
    {
        if((animal = get_animal_specs(s, lineno)) == NULL)
        {
            return 0;
        }
        else
        {
            world->herd = ll_push(world->herd, animal);
            animal_count++;
        }
    }
    fprintf(stderr, "# %d animal%s read from the input\n",
        animal_count, animal_count == 1 ? "" : "s");

    return 1;
}