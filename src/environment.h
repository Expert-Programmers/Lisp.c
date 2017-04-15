//
// Created by me on 17-4-15.
//

#include <stddef.h>
#include "base.h"

#ifndef SIMPLERPEL_ENVIRONMENT_H
#define SIMPLERPEL_ENVIRONMENT_H

list GlobalEnv = NULL;

list ExtendEnv(list name, list value, list env);

list ExtendListEnv(list names, list values, list env);

list LookupVar(list name, list env);

list VarVal(list name);

list SetQ(list name, list val);

list _SetQ(list name, list val);

void Defun(void *f, int args, char *name);

#endif //SIMPLERPEL_ENVIRONMENT_H
