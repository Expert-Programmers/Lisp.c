//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_POWER_H
#define SIMPLERPEL_POWER_H

#include "base.h"

extern list Eval(list a);

extern list EvalList(list args);

extern list ApplyCFun(void *fun, int n, list args);

extern list ProgN(list seq);

extern list Apply(list fun, list args);

#endif //SIMPLERPEL_POWER_H
