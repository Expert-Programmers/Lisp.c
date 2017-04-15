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

extern list Cons(list car, list cdr);

extern int isCons(list v);

extern list Car(list cell);

extern list Cdr(list cell);

extern list SetCar(list car, list val);

extern list SetCdr(list cdr, list val);

#endif //SIMPLERPEL_CONS_H
