//
// Created by me on 17-4-15.
//

#include <stdlib.h>
#include "integer.h"

list Integer(int i) {
    Pint v = (Pint) malloc(sizeof(Pint));
    v->tag = INTEGER;
    v->val = i;
    return v;
}

int isInteger(list v) {
    return TypeTag(v) == INTEGER;
}

int getInteger(list v) {
    return ((Pint) v)->val;
}