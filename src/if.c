//
// Created by me on 17-4-15.
//

#include "if.h"
#include "power.h"

list _If(list exp, list th, list el) {
    if (isNULL(Eval(exp)))
        return Eval(el);
    else
        return Eval(th);
}