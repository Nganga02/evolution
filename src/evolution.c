#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "evolution.h"
#include "xmalloc.h"
#include "read.h"
#include "write.h"
#include "linked-list.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct point
{
    int i;
    int j;
};

static inline int random_gen(int n)
{
    return (rand() / (RAND_MAX / n + 1));
}

static void free_herd(Conscell *herd)
{
    for (Conscell *p = herd; p != NULL; p = p->next)
        free(p->data);
    ll_free(herd);
}

static int dead_or_alive(const void *aa)
{
    animal *animal_ptr = (animal *)aa;

    return animal_ptr->e <= 0 ? 0 : 1;
}

static Conscell *remove_the_dead(Conscell *herd)
{
    Conscell *dead = NULL;
    herd = ll_filter(herd, dead_or_alive, &dead);
    free_herd(dead);

    return herd;
}

static int nearer_the_eden(const void *aa, const void *bb, void *params)
{
    struct point *p1 = (struct point *)aa;
    struct point *p2 = (struct point *)bb;
    struct point *p = (struct pooint *)params;

    int dx = p1->i - p->i;
    int dy = p1->j - p->j;
    unsigned int d1 = dx * dx + dy * dy;
    dx = p2->i - p->i;
    dy = p2->j - p->j;
    unsigned int d2 = dx * dx + dy * dy;

    if (d1 > d2)
        return 1;
    else if (d1 < d2)
        return -1;
    else
        return 0;
}

static void initialize_plants(struct world *world)
{
    make_matrix(world->plants, world->world_h, world->world_w);
}

static void add_plants(struct world *world)
{
    int i, j;
    for (int k = 0; k < 0.1 * world->world_h; k++) // Trying to add 10 plants in the world
    {
        i = random_gen(world->world_h);
        j = random_gen(world->world_w);
        world->plants[i][j]++;
    }

    if (world->eden_h > 0 && world->eden_w > 0)
    {
        i = random_gen(world->eden_h) + (world->world_h - world->eden_h) / 2;
        j = random_gen(world->eden_w) + (world->world_w - world->eden_w) / 2;
        world->plants[i][j]++;
    }
}

// This uses random number generation to choose the gene to activate
static int gene_to_activate(int genes[8])
{
    int genes_cf = 0;
    int k = 0;
    for (int i = 0; i < 8; i++)
        genes_cf += genes[i];

    if (genes_cf == 0)
    {
        fprintf(stderr, "There are no gene variation for the animal");
        exit(EXIT_FAILURE);
    } // or some other error code

    genes_cf = random_gen(genes_cf);

    for (int j = 0; j < 8; j++)
    {
        if (genes_cf <= (k += genes[j]))
        {
            return genes[j];
        }
    }

    // Fallback — should never reach here
    return genes[7];
}

// This is the function to turn an animal in the cell
static void turn(animal *animal)
{
    int k = gene_to_activate(animal->genes);

    animal->d = (animal->d + k) % 8;
}

// This is the function to move an animal in the world cell
static void move(struct world *world, animal *animal)
{
    struct point move_vectors[8] = {
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {-1, -1},
        {0, -1},
        {1, -1}};

    int i = animal->i + move_vectors[animal->d].i;
    int j = animal->j + move_vectors[animal->d].j;

    // This is to ensure a continuous world i.e. the animal does not reach teh end
    if (i < 0)
        i += world->world_h;
    else if (i >= world->world_h)
        i -= world->world_h;

    if (j < 0)
        j += world->world_w;
    else if (j >= world->world_w)
        j -= world->world_w;

    animal->i = i;
    animal->j = j;
}


// This is the feeding algorithm
static void feed(struct world *world, animal *animal)
{
    int i = animal->i;
    int j = animal->j;

    if (world->plants[i][j] >= 1)
    {
        world->plants[i][j]--;
        animal->e += world->plant_energy;
    }
}


// Remember to free the newAnimal pointer in your function
static animal *clone(animal *old)
{
    animal *newAnimal = xmalloc(sizeof *newAnimal);
    newAnimal = old;
    return newAnimal;
}

static void mutate(int genes[8])
{
    int mutator = random_gen(3) - 1;
    int gene_ind = random_gen(8);

    genes[gene_ind] += mutator;

    if (genes[gene_ind] < 1)
        genes[gene_ind] = 1;
    else if (genes[gene_ind] > 7)
        genes[gene_ind] = 1;
}

// This function reproduces a new offspring from the parent animal
static void reproduce(struct world *world, animal *parent)
{
    parent->e /= 2;
    animal *offspring = clone(parent);
    mutate(offspring->genes);
    world->herd = ll_push(world->herd, offspring);
}

// Used to update the world after every time step
static void update_world(struct world *world)
{
    world->herd = remove_the_dead(world->herd);
    for (Conscell *p = world->herd; p != NULL; p = p->next)
    {
        animal *a = p->data;
        turn(a);
        move(world, a);
        feed(world, a);
        a->e--;
        if (a->e >= world->reproduction_threshold)
            reproduce(world, a);
    }
    add_plants(world);
}

static void evolve(struct world *world, unsigned int n)
{
    int i = 1;
    while (i != n)
    {
        update_world(world);
        fprintf(stdout, "\r Progress: %d%% ******************* ", (i / n));
        fflush(stdout);
        i++;
    }
}

static void evolve_with_figs(struct world *world, unsigned int n)
{
    char buf[16];
    // an overkill – only 12 chars are needed
    for (unsigned long int i = 0; i < n; i++)
    {
        sprintf(buf, "fig%04lu.eps", i);
        world_to_eps(world, buf);
        if (i < n - 1)
            update_world(world);
    }
}

static void show_usage(char *progname)
{
    fprintf(stderr, "Usage: %s n [f] <infile >outfile \ 
                        n ≥ 0 : (required) number of updates \
                        f ≥ 0 : (optional) number of snapshots after n updates \
                        Reads World Definition from infile, performs \
                        n updates, and writes result to outfile",
            progname);
}

int main(int argc, char **argv)
{
    unsigned long int n;
    unsigned long int f = 0; // Tentatively to understand how the program flows
    struct world World;
    struct world *world = &World;
    int exit_status = EXIT_FAILURE;
    world->herd = NULL;
    world->plants = NULL;

    if (argc < 2 || argc > 3)
    {
        show_usage(argv[0]);
    }

    if (sscanf(argv[1], "%lu", &n) >= 1)
    {
        show_usage(argv[0]);
        goto cleanup;
    }

    if (argc > 2 && sscanf(argv[2], "%lu", &f) != 1)
    {
        show_usage(argv[0]);
        goto cleanup;
    }

    if (!read_wdf(world))
        goto cleanup;

    initialize_plants(world);   

    evolve(world, n);

    if (f > 0)
        evolve_with_figs(world, f);

    struct point eden_center;
    eden_center.i = (world->world_h - 1) / 2;
    eden_center.j = (world->world_w - 1) / 2;
    world->herd = ll_sort(world->herd, nearer_the_eden, &eden_center);
    write_wdf(world, "");
    exit_status = EXIT_SUCCESS;

cleanup:
    free_matrix(world->plants);
    free_herd(world->herd);
    return exit_status;
}
