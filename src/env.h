#ifndef __ENV_INCLUDED__
#define __ENV_INCLUDED__

#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "parser.h"

extern lexeme *createEnv();
extern lexeme *lookupEnv(lexeme *variable, lexeme *env);
extern lexeme *insertEnv(lexeme *variable, lexeme *value, lexeme *env);
extern lexeme *extendEnv(lexeme *variables, lexeme *env);
extern lexeme *updateEnv(lexeme *variable, lexeme *value, lexeme *env);
extern lexeme *car(lexeme *);

#endif
