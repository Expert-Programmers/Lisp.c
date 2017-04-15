//
// Created by me on 17-4-15.
//

#include "base.h"
#include "integer.h"
#include "atom.h"
#include "cons.h"

int TypeTag(list v) {
    return *((int *) v);
}

list Quote(list a) {
    return a;
}

list Plus(list a, list b) {
    return Integer(getInteger(a) + getInteger(b));
}

list Negate(list a) {
    return Integer(-getInteger(a));
}

list Times(list a, list b) {
    return Integer(getInteger(a) * getInteger(b));
}

list Eq(list a, list b) {
    return Bool(a == b);
}

list Equal(list a, list b) {
    if (a == b)
        return t;
    else if (TypeTag(a) != TypeTag(b))
        return nil;
    else if (isCons(a))
        return Bool(!isNULL(Equal(Car(a), Car(b))) && !isNULL(Equal(Cdr(a), Cdr(b))));
    else if (isInteger(a))
        return Bool(getInteger(a) == getInteger(b));
    else
        return nil;
}

list Lambda(list all) {
    return Cons(Atom("LAMBDA"), all);
}
