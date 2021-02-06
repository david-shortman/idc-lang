#ifndef __PARSER_INCLUDED__
#define __PARSER_INCLUDED__

#include <stdio.h>
#include <string.h>
#include "lex.h"

extern lexeme *parse(FILE *);
extern lexeme *newLexeme(lexemeType type);

#endif
