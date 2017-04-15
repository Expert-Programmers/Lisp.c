lisp GlobalEnv = NULL;

lisp ExtendEnv(lisp name, lisp value, lisp env) {
    return Cons(Cons(name, value), env);
}

lisp ExtendListEnv(lisp names, lisp values, lisp env) {
    while (!NullP(names)) {
        env = ExtendEnv(Car(names), Car(values), env);
        names = Cdr(names);
        values = Cdr(values);
    }
    return env;
}

lisp LookupVar(lisp name, lisp env)
/* return a cons(name, val) */
{
    if (NullP(env))
        return nil;
    else {
        if (Car(Car(env)) == name)
            return Car(env);
        else
            return LookupVar(name, Cdr(env));
    }
}

lisp VarVal(lisp name) {
    lisp v = LookupVar(name, GlobalEnv);
    if (ConsP(v))
        return Cdr(v);
    else
        return v;
}

lisp SetQ(lisp name, lisp val) {
    if (AtomP(name)) {
        lisp v = LookupVar(name, GlobalEnv);
        if (NullP(v)) {
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

lisp _SetQ(lisp name, lisp val) {
    return SetQ(name, Eval(val));
}

void Defun(void *f, int args, char *name) {
    lisp str = Atom(name);
    lisp v = Cfunc(f, args, str);
    SetQ(str, v);
}

/* ================ */

lisp Quote(lisp a) {
    return a;
}

lisp Plus(lisp a, lisp b) {
    return Integer(getInteger(a) + getInteger(b));
}

lisp Negate(lisp a) {
    return Integer(-getInteger(a));
}

lisp Times(lisp a, lisp b) {
    return Integer(getInteger(a) * getInteger(b));
}

lisp Eq(lisp a, lisp b) {
    return Bool(a == b);
}

lisp Equal(lisp a, lisp b) {
    if (a == b)
        return t;
    else if (TypeTag(a) != TypeTag(b))
        return nil;
    else if (ConsP(a))
        return Bool(TrueP(Equal(Car(a), Car(b))) && TrueP(Equal(Cdr(a), Cdr(b))));
    else if (IntegerP(a))
        return Bool(getInteger(a) == getInteger(b));
    else
        return nil;
}

lisp Lambda(lisp all) {
    return Cons(Atom("LAMBDA"), all);
}

/* ================ */

lisp _If(lisp exp, lisp th, lisp el) {
    if (NullP(Eval(exp)))
        return Eval(el);
    else
        return Eval(th);
}

/* ================ */


lisp Eval(lisp a) {
    if (IntegerP(a))
        return a;
    else if (AtomP(a))
        return VarVal(a);
    else if (ConsP(a))
        return Apply(Eval(Car(a)), Cdr(a));
}

lisp EvalList(lisp args) {
    if (NullP(args))
        return nil;
    else
        return Cons(Eval(Car(args)), EvalList(Cdr(args)));
}

lisp ApplyCFun(void *fun, int n, lisp args) {
    lisp (*f)() = fun;
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


lisp ProgN(lisp seq) {
    lisp v = nil;
    while (ConsP(seq)) {
        v = Eval(Car(seq));
        seq = Cdr(seq);
    }
    return v;
}

lisp Apply(lisp fun, lisp args) {
    if (CfuncP(fun)) {
        void *f = getCfunc(fun);
        int n = getCfuncArgs(fun);
        if (f) {
            lisp argl = n > 0 ? EvalList(args) : args;
            return ApplyCFun(f, n, argl);
        } else
            return Atom("APPLY:NOPTR-IN-CFUNC");
    } else {
        /* Not a cfunc, look for lambda! */
        lisp lam = fun; /* VarVal(fun); */
        if (NullP(lam))
            return Atom("EVAL:NO-SUCH-LAM");
        if (ConsP(lam)) {
            lisp argnam = Car(Cdr(lam));
            lisp body = Cdr(Cdr(lam));
            lisp argl = EvalList(args);
            lisp savedEnv = GlobalEnv;
            lisp ret = nil;
            GlobalEnv = ExtendListEnv(argnam, argl, GlobalEnv);
            ret = ProgN(body);
            GlobalEnv = savedEnv;
            return ret;
        }
        return Cons(Atom("EVAL:ERROR"), Cons(fun, args));
    }
}

/* ================ */

lisp ReadExp(char c);

char SkipSpace(char c) {
    while (isspace(c)) {
        c = getchar();
    }
    return c;
}

lisp ReadInt(char c) {
    char arr[20];
    char *p = arr;
    while (isdigit(c)) {
        *p = c;
        p++;
        c = getchar();
    }
    *p = 0;
    ungetc(c, stdin);
    return Integer(atoi(arr));
}

lisp ReadAtom(char c) {
    char arr[20];
    char *p = arr;
    while (isalnum(c)) {
        *p = toupper(c);
        p++;
        c = getchar();
    }
    *p = 0;
    ungetc(c, stdin);
    return Atom(arr);
}

lisp Print(lisp a);

lisp ReadCons(char c) {
    lisp car, cdr;

    car = ReadExp(c);
    c = SkipSpace(getchar());
    if (c == '.') {
        cdr = ReadExp(getchar());
        c = SkipSpace(getchar());
        if (c != ')')
            return Atom("READCONS:ERROR");
        else
            return Cons(car, cdr);
    } else if (c == ')')
        return Cons(car, nil);
    else
        return Cons(car, ReadCons(c));
}

lisp ReadExp(char c) {
    c = SkipSpace(c);
    if (isdigit(c))
        return ReadInt(c);
    else if (isalpha(c))
        return ReadAtom(c);
    else if (c == '(')
        return ReadCons(getchar());
    else {
        printf("ReadExp: unrecognized char = '%c'\n", c);
        return Atom("READEXP: ERROR");
    }
}

lisp Read(void) {
    return ReadExp(' ');
}

/* ================ */

void Prin1_Internal(FILE *f, lisp v);

void Prin1_Cons(FILE *f, lisp v) {
    Prin1_Internal(f, Car(v));
    if (ConsP(Cdr(v)))
        Prin1_Cons(f, Cdr(v));
    else if (NullP(Cdr(v)));
    else {
        fprintf(f, " . ");
        Prin1_Internal(f, Cdr(v));
    }
}

void Prin1_Internal(FILE *f, lisp v) {
    if (NullP(v))
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
                // default: fprintf(f, "<UNPRINTABLE %i> ", v); break;
        }
}

lisp Terpri(void) {
    printf("\n");
    return nil;
}

lisp Prin1(lisp v) {
    Prin1_Internal(stdout, v);
    return v;
}

lisp Print(lisp v) {
    Prin1(v);
    Terpri();
    return v;
}

/* ================ */

void InitLisp(void) {
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

void ReadEvalLoop(void) {
    lisp e = nil;
    while (e != Atom("EOF")) {
        printf(">>");
        e = Read();
        /* Print(e); */
        Print(Eval(e));
    }
}

void Lisp(void) {
    InitLisp();
    {
        lisp v;
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
