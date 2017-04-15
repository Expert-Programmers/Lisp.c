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

extern list Atom(char *name);

extern char *getAtomString(list v);

extern int isAtom(list v);

extern int isNULL(list v);

extern list Bool(int b);

extern int getBool(list v);

#endif //SIMPLERPEL_ATOM_H
