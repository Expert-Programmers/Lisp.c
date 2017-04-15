//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_BASE_H
#define SIMPLERPEL_BASE_H


typedef void *list;

int TypeTag(list v);

list Quote(list a);

list Plus(list a, list b);

list Negate(list a);

list Times(list a, list b);

list Eq(list a, list b);

list Equal(list a, list b);

list Lambda(list all);

#endif //SIMPLERPEL_BASE_H
