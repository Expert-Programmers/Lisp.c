#include <stdio.h>
#include <stdlib.h>

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

int main()
{
    printf("Hello, World!\n");
    return 0;
}