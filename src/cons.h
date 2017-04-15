//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_CONS_H
#define SIMPLERPEL_CONS_H

#include "base.h"

#define CONS 1

typedef struct Tcons {
    int tag;
    struct Tcons *car;
    struct Tcons *cdr;
} *Pcons, Tcons;

list Cons(list car, list cdr);

int isCons(list v);

list Car(list cell);

list Cdr(list cell);

list SetCar(list car, list val);

list SetCdr(list cdr, list val);

#endif //SIMPLERPEL_CONS_H
