//
// Created by me on 17-4-15.
//

#include <malloc.h>
#include "cons.h"

list Cons(list car, list cdr) {
    Pcons v = (Pcons) malloc(sizeof(Pcons));
    v->tag = CONS;
    v->car = (Pcons) car;
    v->cdr = (Pcons) cdr;
    return v;
}

int isCons(list v) {
    return TypeTag(v) == CONS;
}

list Car(list cell) {
    if (isCons(cell)) {
        return ((Pcons) cell)->car;
    } else {
        return NULL;
    }
}

list Cdr(list cell) {
    if (isCons(cell)) {
        return ((Pcons) cell)->cdr;
    } else {
        return NULL;
    }
}

list SetCar(list cell, list val) {
    if (isCons(cell)) {
        ((Pcons) cell)->car = val;
        return cell;
    } else {
        return NULL;
    }
}

list SetCdr(list cell, list val) {
    if (isCons(cell)) {
        ((Pcons) cell)->cdr = val;
        return cell;
    } else {
        return NULL;
    }
}