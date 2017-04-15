//
// Created by me on 17-4-15.
//

#include <stddef.h>
#include "base.h"

#ifndef SIMPLERPEL_ENVIRONMENT_H
#define SIMPLERPEL_ENVIRONMENT_H

extern list GlobalEnv;

extern list ExtendEnv(list name, list value, list env);

extern list ExtendListEnv(list names, list values, list env);

extern list LookupVar(list name, list env);

extern list VarVal(list name);

extern list SetQ(list name, list val);

extern list _SetQ(list name, list val);

extern void Defun(void *f, int args, char *name);

#endif //SIMPLERPEL_ENVIRONMENT_H
