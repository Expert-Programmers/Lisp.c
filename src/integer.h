//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_INTEGER_H
#define SIMPLERPEL_INTEGER_H

#include <stdlib.h>

#include "base.h"

#define INTEGER 3

typedef struct Tint {
    int tag;
    int val;
} *Pint, Tint;

extern list Integer(int i);

extern int isInteger(list v);

extern int getInteger(list v);

#endif //SIMPLERPEL_INTEGER_H
