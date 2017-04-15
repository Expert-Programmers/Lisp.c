//
// Created by me on 17-4-15.
//
#include <stdio.h>
#include "environment.h"
#include "cfunc.h"
#include "print.h"
#include "power.h"
#include "cons.h"
#include "atom.h"

list GlobalEnv = NULL;

list ExtendEnv(list name, list value, list env) {
    return Cons(Cons(name, value), env);
}

list ExtendListEnv(list names, list values, list env) {
    while (!isNULL(names)) {
        env = ExtendEnv(Car(names), Car(values), env);
        names = Cdr(names);
        values = Cdr(values);
    }
    return env;
}

/* return a cons(name, val) */
list LookupVar(list name, list env) {
    if (isNULL(env))
        return nil;
    else {
        if (Car(Car(env)) == name)
            return Car(env);
        else
            return LookupVar(name, Cdr(env));
    }
}

list VarVal(list name) {
    list v = LookupVar(name, GlobalEnv);
    if (isCons(v))
        return Cdr(v);
    else
        return v;
}

list SetQ(list name, list val) {
    if (isAtom(name)) {
        list v = LookupVar(name, GlobalEnv);
        if (isNULL(v)) {
            printf("Extending! ");
            Print(name);
            printf("==");
            Print(val);
            printf("ENV-before: ");
            Print(GlobalEnv);
            GlobalEnv = ExtendEnv(name, val, GlobalEnv);
            printf("ENV-after: ");
            Print(GlobalEnv);
        } else {
            printf("Setting! ");
            Print(name);
            printf("==");
            Print(val);
            printf("ENV-before: ");
            Print(GlobalEnv);
            SetCdr(v, val);
            printf("ENV-after: ");
            Print(GlobalEnv);
        }
        return val;
    } else
        return Atom("SETQ-FAILED");
}

list _SetQ(list name, list val) {
    return SetQ(name, Eval(val));
}

void Defun(void *f, int args, char *name) {
    list str = Atom(name);
    list v = Cfunc(f, args, str);
    SetQ(str, v);
}
