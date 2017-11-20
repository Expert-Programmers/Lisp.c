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
    if (IsCons(cons)) {
        return ((Pcons) cons)->car;
    } else {
        return NULL;
    }
}

List Cdr(List cons)
{
    // 获得cdr元素
    if (IsCons(cons)) {
        return ((Pcons) cons)->cdr;
    } else {
        return NULL;
    }
}

List SetCar(List cons, List val)
{
    // 为序偶cons的car元素赋值
    if (IsCons(cons)) {
        ((Pcons) cons)->car = val;
        return cons;
    } else {
        return NULL;
    }
}

List SetCdr(List cons, List val)
{
    // 为序偶cons的cdr元素赋值
    if (IsCons(cons)) {
        ((Pcons) cons)->cdr = val;
        return cons;
    } else {
        return NULL;
    }
}

// --------------------------------------------

#define ATOM 2

typedef struct Tatom {
    int tag;
    char *name;
    struct Tatom *next;
} *Patom, Tatom; // 原子

// 一个简单的链表
List AtomList = NULL;
List nil = NULL;
List t = NULL; // true
// nil与t虽然值相同,但是所在的内存地址不同,所以这两个变量是有区别的

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
    if (IsCons(atom)) {
        return ((Patom) atom)->name;
    } else {
        return NULL;
    }
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

int GetIneteger(List v)
{
    if (IsInteger(v)) {
        return ((Pinteger) v)->val;
    } else {
        return NULL;
    }
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
    if (IsCfunc(v)) {
        return ((Pcfunc) v)->cfunc;
    } else {
        return NULL;
    }
}

List GetCfuncName(List v)
{
    if (IsCfunc(v)) {
        return ((Pcfunc) v)->name;
    } else {
        return NULL;
    }
}

int GetCfuncArgs(List v)
{
    if (IsCfunc(v)) {
        return ((Pcfunc) v)->args;
    } else {
        return NULL;
    }
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
                fprintf(f, "%d ", GetIneteger(v));
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

List Eval(List val);

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


int main()
{
    printf("Hello, World!\n");
    return 0;
}