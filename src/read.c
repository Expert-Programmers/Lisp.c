//
// Created by me on 17-4-15.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "read.h"
#include "cons.h"
#include "atom.h"
#include "integer.h"

char SkipSpace(char c) {
    while (isspace(c)) {
        c = getchar();
    }
    return c;
}

list ReadInt(char c) {
    char arr[20];
    char *p = arr;
    while (isdigit(c)) {
        *p = c;
        p++;
        c = getchar();
    }
    *p = 0;
    ungetc(c, stdin);
    return Integer(atoi(arr));
}

list ReadAtom(char c) {
    char arr[20];
    char *p = arr;
    while (isalnum(c)) {
        *p = toupper(c);
        p++;
        c = getchar();
    }
    *p = 0;
    ungetc(c, stdin);
    return Atom(arr);
}


list ReadCons(char c) {
    list car, cdr;

    car = ReadExp(c);
    c = SkipSpace(getchar());
    if (c == '.') {
        cdr = ReadExp(getchar());
        c = SkipSpace(getchar());
        if (c != ')')
            return Atom("READCONS:ERROR");
        else
            return Cons(car, cdr);
    } else if (c == ')')
        return Cons(car, nil);
    else
        return Cons(car, ReadCons(c));
}

list ReadExp(char c) {
    c = SkipSpace(c);
    if (isdigit(c))
        return ReadInt(c);
    else if (isalpha(c))
        return ReadAtom(c);
    else if (c == '(')
        return ReadCons(getchar());
    else {
        printf("ReadExp: unrecognized char = '%c'\n", c);
        return Atom("READEXP: ERROR");
    }
}

list Read(void) {
    return ReadExp(' ');
}
