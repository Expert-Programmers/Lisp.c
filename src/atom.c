//
// Created by me on 17-4-15.
//

#include <string.h>
#include <stdlib.h>
#include "atom.h"


list Atom(char *name) {
    list i = AtomList;
    while (i) {
        if (strcmp(((Patom) i)->name, name) == 0) {
            return i;
        }
        i = ((Patom) i)->next;
    }

    {
        Patom v = (Patom) malloc(sizeof(Patom));
        v->tag = ATOM;
        v->name = name;
        v->next = AtomList;
        AtomList = v;
        return v;
    }
}

int isAtom(list v) {
    return TypeTag(v) == ATOM;
}

char *getAtomString(list v) {
    return ((Patom) v)->name;
}

int isAtomNULL(list v) {
    return (v == nil);
}

int AtomNULL(list v) {
    return !isAtomNULL(v);
}

list Bool(int b) {
    if (b) {
        return t;
    } else {
        return nil;
    }
}

int getBool(list v) {
    if (isAtomNULL(v)) {
        return 0;
    } else {
        return 1;
    }
}