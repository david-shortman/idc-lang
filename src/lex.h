#ifndef __LEX_INCLUDED__
#define __LEX_INCLUDED__

#include <stdio.h>
#include <string.h>

typedef enum
{
    ENDofINPUT,
    INTEGER,
    STRING,
    OPAREN,
    CPAREN,
    COMMA,
    PLUS,
    MINUS,
    TIMES,
    MOD,
    DIVIDES,
    PERIOD,
    OBRACKET,
    CBRACKET,
    LESSTHAN,
    AS,
    VALUEOF,
    GIVEBACK,
    FUNCTION,
    DECLARE,
    GREATERTHAN,
    EQUALS,
    VARIABLE,
    DISPLAY,
    IF,
    THENDOTHIS,
    IS,
    AND,
    OR,
    ARRAY,
    OFSIZE,
    OF,
    ITEMATINDEX,
    HOLDING,
    INT,
    STR,
    WHILE,
    CHANGE,
    SET,
    TOBE,
    ELSE,
    PROGRAM,
    NEXT,
    BODY,
    PARAMS,
    LOGIC,
    COND,
    CALL,
    MATH,
    ENV,
    TABLE,
    PRINTDONE,
    T,
    F,
    DISPLAYLN,
    NIL
} lexemeType;

typedef struct lexeme
{
    lexemeType type;
    char *string;
    int integer;
    char *name;
    int lineNumber;
    int envNum;
    struct lexeme *func;
    struct lexeme *defEnv;
    struct lexeme *left;
    struct lexeme *right;
    struct lexeme **array;
    int arraySize;
} lexeme;

extern lexeme *lex(FILE *);                  //returns a lexeme
extern void displayLexeme(FILE *, lexeme *); //prints a lexeme
extern void displayType(FILE *, lexemeType); //prints a type name
extern lexeme *newIntLexeme(lexemeType, int);
extern lexeme *newStrLexeme(lexemeType, char *);
extern void displayEnv(FILE *, lexeme *);

#endif
