//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_PRINT_H
#define SIMPLERPEL_PRINT_H

#include "base.h"

extern void Prin1_Cons(FILE *f, list v);

extern void Prin1_Internal(FILE *f, list v);

extern list Terpri(void);

extern list Prin1(list v);

extern list Print(list v);

#endif //SIMPLERPEL_PRINT_H
