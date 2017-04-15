//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_CORE_H
#define SIMPLERPEL_CORE_H


#include <stdio.h>
#include "atom.h"
#include "environment.h"
#include "power.h"
#include "print.h"
#include "read.h"
#include "if.h"
#include "cons.h"

void InitLisp();

void ReadEvalLoop();

void Lisp();

#endif //SIMPLERPEL_CORE_H
