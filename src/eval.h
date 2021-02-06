#ifndef __EVAL_INCLUDED__
#define __EVAL_INCLUDED__

#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "env.h"

extern lexeme *evaluate(lexeme *t, lexeme *env);
extern lexeme *disp(lexeme *val);

#endif
