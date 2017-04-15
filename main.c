#include <stdio.h>
#include "src/atom.h"
#include "src/environment.h"
#include "src/power.h"
#include "src/print.h"
#include "src/read.h"
#include "src/if.h"

void InitLisp() {
    nil = Atom("NIL");
    t = Atom("T");
    GlobalEnv = nil;
    SetQ(t, t);

    /* Integers */
    Defun(Plus, 2, "PLUS");
    Defun(Negate, 1, "NEGATE");
    Defun(Times, 2, "TIMES");

    /* Cons & Atoms */
    Defun(Cons, 2, "CONS");
    Defun(Car, 1, "CAR");
    Defun(Cdr, 1, "CDR");
    Defun(SetCar, 2, "SETCAR");
    Defun(SetCdr, 2, "SETCDR");

    /* Bindings */
    Defun(SetQ, 2, "SET");
    Defun(_SetQ, -2, "SETQ");

    /* Yang & Yeng */
    Defun(Eval, 1, "EVAL");
    Defun(Apply, 2, "APPLY");

    /* Pred */
    Defun(Eq, 2, "EQ");
    Defun(Equal, 2, "EQUAL");

    /* IO */
    Defun(Prin1, 1, "PRIN1");
    Defun(Print, 1, "PRINT");
    Defun(Terpri, 1, "TERPRI");
    Defun(Read, 0, "READ");

    /* NEVAL */
    Defun(Quote, -1, "QUOTE");
    Defun(_If, -3, "IF");
    Defun(Lambda, -16, "LAMBDA");
}

void ReadEvalLoop() {
    list e = nil;
    while (e != Atom("EOF")) {
        printf(">>");
        e = Read();
        Print(Eval(e));
    }
}

void Lisp() {
    InitLisp();
    {
        list v;
        v = nil;
        Print(v);
        Print(v);
        Print(nil);
        Print(t);
        Print(t);
    }
    ReadEvalLoop();
}

int main() {
    Lisp();
}