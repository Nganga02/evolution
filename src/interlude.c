// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>

// #include "read.h"
// #include "write.h"
// #include "evolution.h"
// #include "linked-list.h"
// #include "malloc.h"

// int usage()
// {
//     return 0;
// }
// int main(int argc,char **argv)
// {
//     struct world World;
//     struct world *world = &World;
//     world->herd = NULL;
    
    
//     if(argc != 2)
//     {
//         fprintf(stderr, "Usage: \tAccepts 2 arguments %s and outfile ", argv[0]);
//         printf("%d", argc);
//         return EXIT_FAILURE;
//     }
    
//     if(argv[1] != NULL && read_wdf(world))
//     {

//         printf("We are in this function");
//         if(argv[1] != NULL)
//         {
//             write_wdf(world, argv[1]);
//             fprintf(stderr,"printed successfully");
//         }
//         else
//         {
//             goto cleanup;
//             fprintf(stderr,"The out file should not be an empty string");
//         }
//     }
//     else
//     {

//         goto cleanup;
//         fprintf(stderr,"The infile should not be an empty string");
//     }

// cleanup:
//     ll_free(world->herd);
// }



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "read.h"
#include "write.h"
#include "evolution.h"
#include "linked-list.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input-file> <output-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("This is the second argument %s\n", argv[1]);
    struct world World;
    struct world *world = &World;
    world->herd = NULL;

    if (read_wdf(world)) {
        printf("We are in the write function");
        write_wdf(world, argv[1]);
        fprintf(stderr, "Printed successfully\n");
        goto cleanup;
        
    } else {
        fprintf(stderr, "Failed to read from input file: %s\n", argv[1]);
        goto cleanup;
    }

cleanup:
    ll_free(world->herd);
    return EXIT_SUCCESS;
}
