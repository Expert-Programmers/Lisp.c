//
// Created by me on 17-4-15.
//

#include <stdlib.h>
#include "cfunc.h"

list Cfunc(void *f, int args, list name) {
    Pcfunc v = (Pcfunc) malloc(sizeof(Pcfunc));
    v->tag = CFUNC;
    v->args = args;
    v->name = name;
    v->func = f;
    return v;
}

int isCfunc(list v) {
    return TypeTag(v) == CFUNC;
}

void *getCfunc(list v) {
    if (isCfunc(v)) {
        return ((Pcfunc) v)->func;
    } else {
        return NULL;
    }
}

list getCfuncName(list v) {
    if (isCfunc(v)) {
        return ((Pcfunc) v)->name;
    } else {
        return NULL;
    }
}

int getCfuncArgs(list v) {
    if (isCfunc(v)) {
        return ((Pcfunc) v)->args;
    } else {
        return NULL;
    }
}