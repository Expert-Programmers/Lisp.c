//
// Created by me on 17-4-15.
//

#ifndef SIMPLERPEL_BASE_H
#define SIMPLERPEL_BASE_H


typedef void *list;

extern int TypeTag(list v);

extern list Quote(list a);

extern list Plus(list a, list b);

extern list Negate(list a);

extern list Times(list a, list b);

extern list Eq(list a, list b);

extern list Equal(list a, list b);

extern list Lambda(list all);

#endif //SIMPLERPEL_BASE_H
