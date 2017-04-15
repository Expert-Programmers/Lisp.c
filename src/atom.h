//
// Created by me on 17-4-15.
//

#include <stddef.h>
#include "base.h"

#ifndef SIMPLERPEL_ATOM_H
#define SIMPLERPEL_ATOM_H

#define ATOM 2

typedef struct Tatom {
    int tag;
    char *name;
    struct Tatom *next;
} *Patom, Tatom;

extern list AtomList;
extern list t;
extern list nil;

list Atom(char *name);

char *getAtomString(list v);

int isAtom(list v);

int isNULL(list v);

list Bool(int b);

int getBool(list v);

#endif //SIMPLERPEL_ATOM_H
