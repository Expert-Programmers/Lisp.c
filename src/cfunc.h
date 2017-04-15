//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_CFUNC_H
#define SIMPLERPEL_CFUNC_H

#include "base.h"

#define CFUNC 4

typedef struct Tcfunc {
    int tag;
    int args;
    list name;
    void *func;
} *Pcfunc, Tcfunc;

extern list Cfunc(void *f, int args, list name);

extern int isCfunc(list v);

extern void *getCfunc(list v);

extern list getCfuncName(list v);

extern int getCfuncArgs(list v);

#endif //SIMPLERPEL_CFUNC_H
