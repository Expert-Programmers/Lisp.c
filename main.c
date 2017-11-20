#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void *list; // lisp(list process)中,一切都是列表

int TypeTag(list v)
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

list Cons(list car, list cdr)
{
    Pcons v = (Pcons) malloc(sizeof(Tcons));
    v->tag = CONS;
    v->car = car;
    v->cdr = cdr;
    return v;
} // 序偶的构造函数

int IsCons(list cons)
{   // 判断是否是一个Cons
    return TypeTag(cons) == CONS;
}

list Car(list cons)
{   // 获得car元素
    if (IsCons(cons)) {
        return ((Pcons) cons)->car;
    } else {
        return NULL;
    }
}

list Cdr(list cons)
{
    // 获得cdr元素
    if (IsCons(cons)) {
        return ((Pcons) cons)->cdr;
    } else {
        return NULL;
    }
}

list SetCar(list cons, list val)
{
    // 为序偶cons的car元素赋值
    if (IsCons(cons)) {
        ((Pcons) cons)->car = val;
        return cons;
    } else {
        return NULL;
    }
}

list SetCdr(list cons, list val)
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
list AtomList = NULL;
list nil = NULL;
list t = NULL; // true
// nil与t虽然值相同,但是所在的内存地址不同,所以这两个变量是有区别的

list Atom(char *name)
{
    list i = AtomList;
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

char *GetAtomString(list atom)
{
    if (IsCons(atom)) {
        return ((Patom) atom)->name;
    } else {
        return NULL;
    }
}

int IsAtom(list v)
{   // 判断是否为atom
    return TypeTag(v) == ATOM;
}

int IsNULL(list v)
{
    return v == nil;
}

list Bool(list b)
{
    if (b) {
        return t;
    } else {
        return nil;
    }
}

int GetBool(list v)
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

list Integer(int i)
{
    Pinteger v = (Pinteger) malloc(sizeof(Tintegr));
    v->tag = INTEGER;
    v->val = i;
    return v;
}

int IsInteger(list v)
{
    return TypeTag(v) == INTEGER;
}

int GetIneteger(list v)
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
    list name;
    void *cfunc;
} *Pcfunc, Tcfunc; // 函数

list Cfunc(void *f, int args, list name)
{
    Pcfunc v = (Pcfunc) malloc(sizeof(Tcfunc));
    v->tag = CFUNC;
    v->args = args;
    v->name = name;
    v->cfunc = f;
    return v;
}

int IsCfunc(list v)
{
    return TypeTag(v) == CFUNC;
}

void *GetCfunc(list v)
{
    if (IsCfunc(v)) {
        return ((Pcfunc) v)->cfunc;
    } else {
        return NULL;
    }
}

list GetCfuncName(list v)
{
    if (IsCfunc(v)) {
        return ((Pcfunc) v)->name;
    } else {
        return NULL;
    }
}

int GetCfuncArgs(list v)
{
    if (IsCfunc(v)) {
        return ((Pcfunc) v)->args;
    } else {
        return NULL;
    }
}

// --------------------------------------------

int main()
{
    printf("Hello, World!\n");
    return 0;
}