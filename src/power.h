//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_POWER_H
#define SIMPLERPEL_POWER_H

#include "base.h"

list Eval(list a);

list EvalList(list args);

list ApplyCFun(void *fun, int n, list args);

list ProgN(list seq);

list Apply(list fun, list args);

#endif //SIMPLERPEL_POWER_H
