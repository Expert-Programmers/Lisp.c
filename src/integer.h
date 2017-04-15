//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_INTEGER_H
#define SIMPLERPEL_INTEGER_H

#include "base.h"

#define INTEGER 3

typedef struct Tint {
    int tag;
    int val;
} *Pint, Tint;

list Integer(int i);

int isInteger(list v);

int getInteger(list v);

#endif //SIMPLERPEL_INTEGER_H
