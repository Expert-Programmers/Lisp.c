#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef void *List; // lisp(List process)中,一切都是列表

int TypeTag(List v)
{
    // 获得元素的类型
    // 从后面的基础类型的结构体可以得知,结构体的第一个元素是int型(用来表示类型)
    // 所以这里通过强制类型转换,截取第一个int长度的字节.
    return *((int *) v);
}

// --------------------------------------------

#define CONS 1

typedef struct Tcons {
    int tag;
    struct Tcons *car;
    struct Tcons *cdr;
} *Pcons, Tcons; // 序偶

List Cons(List car, List cdr)
{
    Pcons v = (Pcons) malloc(sizeof(Tcons));
    v->tag = CONS;
    v->car = car;
    v->cdr = cdr;
    return v;
} // 序偶的构造函数

int IsCons(List cons)
{   // 判断是否是一个Cons
    return TypeTag(cons) == CONS;
}

List Car(List cons)
{   // 获得car元素
    return ((Pcons) cons)->car;
}

List Cdr(List cons)
{
    // 获得cdr元素
    return ((Pcons) cons)->cdr;
}

List SetCar(List cons, List val)
{
    // 为序偶cons的car元素赋值
    ((Pcons) cons)->car = val;
    return cons;
}

List SetCdr(List cons, List val)
{
    // 为序偶cons的cdr元素赋值
    ((Pcons) cons)->cdr = val;
    return cons;
}

// --------------------------------------------

#define ATOM 2

typedef struct Tatom {
    int tag;
    char *name;
    struct Tatom *next;
} *Patom, Tatom; // 原子

List Atom(char *name);

// 一个简单的链表
List AtomList = NULL;
List nil = NULL;
List t = NULL; // true

List Atom(char *name)
{
    List i = AtomList;
//    所有的原子存放在一个列表里面, 在构建新的Atom的时候, 先去表中查是否存在相同的, 如果不存在相同的, 才创建atom.
//    享元模式, 节省空间
    while (i) {
        if (strcmp(((Patom) i)->name, name) == 0) {
            return i;
        }
        i = ((Patom) i)->next;
    }

//    创建新的atom
    {
        Patom v = (Patom) malloc(sizeof(Tatom));
        v->tag = ATOM;
        v->name = name;
        v->next = AtomList;
        AtomList = v;
        return v;
    }
}

char *GetAtomString(List atom)
{
    return ((Patom) atom)->name;
}

int IsAtom(List v)
{   // 判断是否为atom
    return TypeTag(v) == ATOM;
}

int IsNULL(List v)
{
    return v == nil;
}

List Bool(List b)
{
    if (b) {
        return t;
    } else {
        return nil;
    }
}

int GetBool(List v)
{
    if (IsNULL(v)) {
        return 0;
    } else {
        return 1;
    }
}

// --------------------------------------------

#define INTEGER 3

typedef struct Tinteger {
    int tag;
    int val;
} *Pinteger, Tintegr; // 数子

List Integer(int i)
{
    Pinteger v = (Pinteger) malloc(sizeof(Tintegr));
    v->tag = INTEGER;
    v->val = i;
    return v;
}

int IsInteger(List v)
{
    return TypeTag(v) == INTEGER;
}

int GetInteger(List v)
{
    return ((Pinteger) v)->val;
}

// --------------------------------------------

#define CFUNC 4

typedef struct Tcfunc {
    int tag;
    int args;
    List name;
    void *cfunc;
} *Pcfunc, Tcfunc; // 函数

List Cfunc(void *f, int args, List name)
{
    Pcfunc v = (Pcfunc) malloc(sizeof(Tcfunc));
    v->tag = CFUNC;
    v->args = args;
    v->name = name;
    v->cfunc = f;
    return v;
}

int IsCfunc(List v)
{
    return TypeTag(v) == CFUNC;
}

void *GetCfunc(List v)
{
    return ((Pcfunc) v)->cfunc;
}

List GetCfuncName(List v)
{
    return ((Pcfunc) v)->name;
}

int GetCfuncArgs(List v)
{
    return ((Pcfunc) v)->args;
}

// --------------------------------------------

void Print_Cons(FILE *f, List v);

void Print_Internal(FILE *f, List v);

void Print_Internal(FILE *f, List v)
{
    if (IsNULL(v)) {
        fprintf(f, "NIL ");
    } else {
        switch (TypeTag(v)) {
            case CONS:
                fprintf(f, "(");
                Print_Cons(f, v);
                fprintf(f, ")");
                break;
            case ATOM:
                fprintf(f, "%s ", GetAtomString(v));
                break;
            case INTEGER:
                fprintf(f, "%d ", GetInteger(v));
                break;
            case CFUNC:
                fprintf(f, "<CFUNC %d %s>", GetCfuncArgs(v), GetAtomString(GetCfuncName(v)));
                break;
            default:
                fprintf(f, "<UNPRINTABLE %p>", v);
                break;
        }
    }
}

void Print_Cons(FILE *f, List v)
{
    Print_Internal(f, Car(v));
    if (IsCons(Cdr(v))) {
        Print_Internal(f, Cdr(v));
    } else if (IsNULL(Cdr(v))) { ;
    } else {
        fprintf(f, " . ");
        Print_Internal(f, Cdr(v));
    }
}

List NewLine()
{
    fprintf(stdout, "\n");
    return nil;
}

List Print(List v)
{
    Print_Internal(stdout, v);
    return v;
}

List Println(List v)
{
    Print_Internal(stdout, v);
    NewLine();
}

// --------------------------------------------

char IgnoreSpace(char c)
{
    while (isspace(c)) {
        c = (char) getchar();
    }
    return c;
}

List ReadInteger(char c)
{
    char arr[128];
    char *p = arr;
    while (isdigit(c)) {
        *p = c;
        p++;
        c = (char) getchar();
    }
    *p = 0;
    ungetc(c, stdin);
    return Integer(atoi(arr));
}

List ReadAtom(char c)
{
    char arr[128];
    char *p = arr;
    while (isalnum(c)) {
        *p = (char) toupper(c);
        p++;
        c = (char) getchar();
    }
    *p = 0;
    ungetc(c, stdin);
    return Atom(arr);
}

List ReadCons(char c);

List ReadExp(char c)
{
    c = IgnoreSpace(c);
    if (isdigit(c)) {
        return ReadInteger(c);
    } else if (isalpha(c)) {
        return ReadAtom(c);
    } else if (c == '(') {
        return ReadCons((char) getchar());
    } else {
        printf("ReadExp: unrecognized character = '%c'\n", c);
        return Atom("READEXP: ERROR");
    }
}

List ReadCons(char c)
{
    List car, cdr;

    car = ReadExp(c);
    c = IgnoreSpace((char) getchar());
    if (c == '.') {
        cdr = ReadExp((char) getchar());
        c = IgnoreSpace((char) getchar());
        if (c != ')') {
            return Atom("READCONS:ERROR");
        } else {
            return Cons(car, cdr);
        }
    } else if (c == ')') {
        return Cons(car, nil);
    } else {
        return Cons(car, ReadCons(c));
    }
}

List Read()
{
    return ReadExp(' ');
}

// --------------------------------------------
// 环境

List GlobalEnvironment = NULL;

List ExtendEnvironment(List var, List val, List env)
{
    return Cons(Cons(var, val), env);
}

List ExtendListEnvironment(List vars, List vals, List env)
{
    while (!IsNULL(vars)) {
        env = ExtendEnvironment(Car(vars), Car(vals), env);
        vars = Cdr(vars);
        vals = Cdr(vals);
    }
    return env;
}

/* return a cons(var, val) */
List LookupVar(List var, List env)
{
    if (IsNULL(env)) {
        return nil;
    } else {
        if (Car(Car(env)) == var) {
            return Car(env);
        } else {
            LookupVar(var, Cdr(env));
        }
    }
}

List VarVal(List var)
{
    List v = LookupVar(var, GlobalEnvironment);
    if (IsCons(v)) {
        return Car(v);
    } else {
        return v;
    }
}

List SetQ(List var, List val)
{
    if (IsAtom(var)) {
        List v = LookupVar(var, GlobalEnvironment);
        if (IsNULL(v)) {
            fprintf(stdout, "Extending: ");
            Print(var);
            fprintf(stdout, " == ");
            Print(val);
            fprintf(stdout, "ENV-before: ");
            Print(GlobalEnvironment);
            GlobalEnvironment = ExtendEnvironment(var, val, GlobalEnvironment);
            printf("ENV-after: ");
            Print(GlobalEnvironment);
        } else {
            printf("Setting! ");
            Print(var);
            fprintf(stdout, " == ");
            Print(val);
            fprintf(stdout, "ENV-before: ");
            Print(GlobalEnvironment);
            SetCdr(v, val);
            fprintf(stdout, "ENV-after: ");
            Print(GlobalEnvironment);
        }
        fprintf(stdout, "\n");
        return val;
    } else {
        return Atom("SETQ-FAILED");
    }
}

List Eval(List exp);

List _SetQ(List var, List val)
{
    return SetQ(var, Eval(val));
}

void Defun(void *f, int args, char *name)
{
    List str = Atom(name);
    List v = Cfunc(f, args, str);
    SetQ(str, v);
}

// --------------------------------------------
// condition

List _If(List exp, List consequent, List alternative)
{
    if (!IsNULL(Eval(exp))) {
        return Eval(consequent);
    } else {
        return Eval(alternative);
    }
}

// --------------------------------------------
// apply and eval  -- magic

List Apply(List func, List args);

List Eval(List exp)
{
    if (IsInteger(exp)) {
        return exp;
    } else if (IsAtom(exp)) {
        return VarVal(exp);
    } else if (IsCons(exp)) {
        return Apply(Eval(Car(exp)), Cdr(exp));
    }
}

List EvalList(List args)
{
    if (IsNULL(args)) {
        return nil;
    } else {
        return Cons(Eval(Car(args)), EvalList(Cdr(args)));
    }
}

List ApplyCFun(void *func, int n, List args)
{
    List (*f)() = func;
    switch (n > 0 ? n : -n) {
        case 0:
            return (f)();
        case 1:
            return (f)(Car(args));
        case 2:
            return (f)(Car(args), Car(Cdr(args)), Car(Cdr(Cdr(args))));
        case 3:
            return (f)(Car(args), Car(Cdr(args)), Car(Cdr(Cdr(args))), Car(Cdr(Cdr(Cdr(args)))));
        case 16:
            return (f)(args);
        default:
            return Atom("NOGOODFUNC");
    }
}


List ProgN(List seq)
{
    List v = nil;
    while (IsCons(seq)) {
        v = Eval(Car(seq));
        seq = Cdr(seq);
    }
    return v;
}

List Apply(List func, List args)
{
    if (IsCfunc(func)) {
        void *f = GetCfunc(func);
        int n = GetCfuncArgs(func);
        if (f) {
            List argl = n > 0 ? EvalList(args) : args;
            return ApplyCFun(f, n, argl);
        } else
            return Atom("APPLY:NOPTR-IN-CFUNC");
    } else {
        /* Not a cfunc, look for lambda! */
        List lam = func; /* VarVal(fun); */
        if (IsNULL(lam))
            return Atom("EVAL:NO-SUCH-LAM");
        if (IsCons(lam)) {
            List argnam = Car(Cdr(lam));
            List body = Cdr(Cdr(lam));
            List argl = EvalList(args);
            List savedEnv = GlobalEnvironment;
            List ret = nil;
            GlobalEnvironment = ExtendListEnvironment(argnam, argl, GlobalEnvironment);
            ret = ProgN(body);
            GlobalEnvironment = savedEnv;
            return ret;
        }
        return Cons(Atom("EVAL:ERROR"), Cons(func, args));
    }
}

List Quote(List a)
{
    return a;
}

List Plus(List a, List b)
{
    return Integer(GetInteger(a) + GetInteger(b));
}

List Negate(List a)
{
    return Integer(-GetInteger(a));
}

List Times(List a, List b)
{
    return Integer(GetInteger(a) * GetInteger(b));
}

List Eq(List a, List b)
{
    return Bool((a == b));
}

List Equal(List a, List b)
{
    if (a == b)
        return t;
    else if (TypeTag(a) != TypeTag(b))
        return nil;
    else if (IsCons(a))
        return Bool((!IsNULL(Equal(Car(a), Car(b))) && !IsNULL(Equal(Cdr(a), Cdr(b)))));
    else if (IsInteger(a))
        return Bool((GetInteger(a) == GetInteger(b)));
    else
        return nil;
}

List Lambda(List all)
{
    return Cons(Atom("LAMBDA"), all);
}


void Init()
{
    nil = Atom("NIL");
    t = Atom("T");
    GlobalEnvironment = nil;
    SetQ(t, t);

    // Integers
    Defun(Plus, 2, "PLUS");
    Defun(Negate, 1, "NEGATE");
    Defun(Times, 2, "TIMES");

    // Cons & Atoms
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
    Defun(Print, 1, "PRINT");
    Defun(Print, 1, "PRINTLN");
    Defun(NewLine(), 1, "NEWLINE");
    Defun(Read, 0, "READ");

    /* NEVAL */
    Defun(Quote, -1, "QUOTE");
    Defun(_If, -3, "IF");
    Defun(Lambda, -16, "LAMBDA");
}

void ReadEvalPrintLoop()
{
    List exp = nil;
    while (exp != Atom("EOF")) {
        printf(">>");
        exp = Read();
        Print(Eval(exp));
    }
}

void Lisp()
{
    Init();
    {
        List v;
        v = nil;
        Print(v);
        Print(v);
        Print(nil);
        Print(t);
        Print(t);
    }
    ReadEvalPrintLoop();
}

int main(int args, char *argv[])
{
    Lisp();
    return 0;
}