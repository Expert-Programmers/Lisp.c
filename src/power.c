//
// Created by me on 17-4-15.
//

#include "power.h"
#include "environment.h"
#include "cfunc.h"

list Eval(list a) {
    if (isInteger(a))
        return a;
    else if (isAtom(a))
        return VarVal(a);
    else if (isCons(a))
        return Apply(Eval(Car(a)), Cdr(a));
}

list EvalList(list args) {
    if (isNULL(args))
        return nil;
    else
        return Cons(Eval(Car(args)), EvalList(Cdr(args)));
}

list ApplyCFun(void *fun, int n, list args) {
    list (*f)() = fun;
    switch (n > 0 ? n : -n) {
        case 0:
            return (f)();
            break;
        case 1:
            return (f)(Car(args));
            break;
        case 2:
            return (f)(Car(args), Car(Cdr(args)), Car(Cdr(Cdr(args))));
            break;
        case 3:
            return (f)(Car(args), Car(Cdr(args)), Car(Cdr(Cdr(args))),
                       Car(Cdr(Cdr(Cdr(args)))));
            break;
        case 16:
            return (f)(args);
            break;

        default:
            return Atom("NOGOODFUNC");
    }
}

list ProgN(list seq) {
    list v = nil;
    while (isCons(seq)) {
        v = Eval(Car(seq));
        seq = Cdr(seq);
    }
    return v;
}

list Apply(list fun, list args) {
    if (isCfunc(fun)) {
        void *f = getCfunc(fun);
        int n = getCfuncArgs(fun);
        if (f) {
            list argl = n > 0 ? EvalList(args) : args;
            return ApplyCFun(f, n, argl);
        } else
            return Atom("APPLY:NOPTR-IN-CFUNC");
    } else {
        /* Not a cfunc, look for lambda! */
        list lam = fun; /* VarVal(fun); */
        if (isNULL(lam))
            return Atom("EVAL:NO-SUCH-LAM");
        if (isCons(lam)) {
            list argnam = Car(Cdr(lam));
            list body = Cdr(Cdr(lam));
            list argl = EvalList(args);
            list savedEnv = GlobalEnv;
            list ret = nil;
            GlobalEnv = ExtendListEnv(argnam, argl, GlobalEnv);
            ret = ProgN(body);
            GlobalEnv = savedEnv;
            return ret;
        }
        return Cons(Atom("EVAL:ERROR"), Cons(fun, args));
    }
}
