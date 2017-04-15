//
// Created by me on 17-4-15.
//

#include <stdio.h>
#include "print.h"
#include "cfunc.h"

void Prin1_Cons(FILE *f, list v) {
    Prin1_Internal(f, Car(v));
    if (isCons(Cdr(v)))
        Prin1_Cons(f, Cdr(v));
    else if (isNULL(Cdr(v)));
    else {
        fprintf(f, " . ");
        Prin1_Internal(f, Cdr(v));
    }
}

void Prin1_Internal(FILE *f, list v) {
    if (isNULL(v))
        fprintf(f, "NIL ");
    else
        switch (TypeTag(v)) {
            case CONS:
                fprintf(f, "(");
                Prin1_Cons(f, v);
                fprintf(f, ") ");
                break;
            case ATOM:
                fprintf(f, "%s ", getAtomString(v));
                break;
            case INTEGER:
                fprintf(f, "%i ", getInteger(v));
                break;
            case CFUNC:
                fprintf(f, "<CFUNC%i %s> ", getCfuncArgs(v),
                        getAtomString(getCfuncName(v)));
                break;
//            default:
//                fprintf(f, "<UNPRINTABLE %i> ", v);
//                break;
        }
}

list Terpri(void) {
    printf("\n");
    return nil;
}

list Prin1(list v) {
    Prin1_Internal(stdout, v);
    return v;
}

list Print(list v) {
    Prin1(v);
    Terpri();
    return v;
}
