#ifndef H_WRITE_H
#define H_WRITE_H
#include "evolution.h"

void write_wdf(struct world *world, char *filename);
//We are using  a pointer since world is continuously changing

#endif /*H_WRITE_H*/    