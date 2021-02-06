#ifndef __PRETTY_INCLUDED__
#define __PRETTY_INCLUDED__

#include <stdio.h>
#include <string.h>
#include "lex.h"

extern void pretty(lexeme *); //prints parse tree as source code
void treePrint(lexeme *, int, int);
void pParams(lexeme *, int);

#endif
