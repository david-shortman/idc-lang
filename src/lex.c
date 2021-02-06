#include "lex.h"
#include "eval.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int lineNumber = 1;
int utStrLineNumber = 0;

int isWhitespace(char ch)
{
    int isWhite = 0;
    switch (ch)
    {
    case ' ':
        isWhite = 1;
        break;
    case '\n':
        isWhite = 1;
        break;
    case '\t':
        isWhite = 1;
        break;
    }
    return isWhite;
}

char skipComment(FILE *file)
{
    char ch = fgetc(file);
    while (ch != '\n' && ch != EOF)
        ch = fgetc(file);
    lineNumber++;
    return ch;
}

void skipWhiteSpace(FILE *file)
{
    char ch = fgetc(file);
    if (ch == '\n')
        lineNumber++;
    if (ch == '-')
    {
        char temp = fgetc(file);
        if (temp == '-')
            ch = skipComment(file);
        else
            ungetc(temp, file);
    }
    while (isWhitespace(ch))
    {
        ch = fgetc(file);
        if (ch == '\n')
            lineNumber++;
        if (ch == '-')
        {
            char temp = fgetc(file);
            if (temp == '-')
                ch = skipComment(file);
            else
            {
                ungetc(temp, file);
                break;
            }
        }
    }
    ungetc(ch, file);
}

lexeme
    *
    newLexeme(lexemeType type)
{
    lexeme *l = malloc(sizeof(lexeme));
    if (l == 0)
    {
        fprintf(stderr, "out of memory");
        exit(-1);
    }
    l->type = type;
    l->lineNumber = lineNumber;
    l->name = NULL;
    l->func = NULL;
    l->defEnv = NULL;
    l->left = NULL;
    l->right = NULL;
    return l;
}

lexeme
    *
    newIntLexeme(lexemeType type, int value)
{
    lexeme *l = malloc(sizeof(lexeme));
    if (l == 0)
    {
        fprintf(stderr, "out of memory");
        exit(-1);
    }
    l->type = type;
    l->integer = value;
    l->lineNumber = lineNumber;
    l->name = NULL;
    l->func = NULL;
    l->defEnv = NULL;
    l->left = NULL;
    l->right = NULL;
    return l;
}

lexeme
    *
    newStrLexeme(lexemeType type, char *value)
{
    lexeme *l = malloc(sizeof(lexeme));
    if (l == 0)
    {
        fprintf(stderr, "out of memory");
        exit(-1);
    }
    l->type = type;
    l->string = value;
    l->lineNumber = lineNumber;
    l->name = NULL;
    l->func = NULL;
    l->defEnv = NULL;
    l->left = NULL;
    l->right = NULL;
    return l;
}

lexeme
    *
    newVarOrKeyLexeme(lexemeType type, char *value)
{
    lexeme *l = malloc(sizeof(lexeme));
    if (l == 0)
    {
        fprintf(stderr, "out of memory");
        exit(-1);
    }
    if (!strcmp(value, "null"))
        type = NIL;
    else if (!strcmp(value, "declare"))
        type = DECLARE;
    else if (!strcmp(value, "as"))
        type = AS;
    else if (!strcmp(value, "evaluate"))
        type = VALUEOF;
    else if (!strcmp(value, "function"))
        type = FUNCTION;
    else if (!strcmp(value, "give_back"))
        type = GIVEBACK;
    else if (!strcmp(value, "display"))
        type = DISPLAY;
    else if (!strcmp(value, "displayln"))
        type = DISPLAYLN;
    else if (!strcmp(value, "if"))
        type = IF;
    else if (!strcmp(value, "is"))
        type = IS;
    else if (!strcmp(value, "then"))
        type = THENDOTHIS;
    else if (!strcmp(value, "and"))
        type = AND;
    else if (!strcmp(value, "or"))
        type = OR;
    else if (!strcmp(value, "array"))
        type = ARRAY;
    else if (!strcmp(value, "of_size"))
        type = OFSIZE;
    else if (!strcmp(value, "of"))
        type = OF;
    else if (!strcmp(value, "holding"))
        type = HOLDING;
    else if (!strcmp(value, "integers"))
        type = INT;
    else if (!strcmp(value, "strings"))
        type = STR;
    else if (!strcmp(value, "index"))
        type = ITEMATINDEX;
    else if (!strcmp(value, "while"))
        type = WHILE;
    else if (!strcmp(value, "change"))
        type = CHANGE;
    else if (!strcmp(value, "set"))
        type = SET;
    else if (!strcmp(value, "to_be"))
        type = TOBE;
    else if (!strcmp(value, "else"))
        type = ELSE;
    l->type = type;
    l->name = value;
    l->func = NULL;
    l->defEnv = NULL;
    l->lineNumber = lineNumber;
    l->left = NULL;
    l->right = NULL;
    return l;
}

lexeme
    *
    lexNumber(FILE *file, int negative)
{
    int number;
    fscanf(file, "%d", &number);
    if (negative)
        number *= -1;
    return newIntLexeme(INTEGER, number);
}

char convertToChar(int ch)
{
    switch (ch)
    {
    case 'n':
        return '\n';
    case 't':
        return '\t';
    case '"':
        return '\"';
    case '\\':
        return '\\';
    default:
        return 0;
    }
    return ch;
}

lexeme
    *
    lexString(FILE *file)
{
    int size = 512;
    char *string = malloc(512);
    utStrLineNumber = lineNumber;
    int ch = fgetc(file);
    if (ch == '\n')
        lineNumber++;
    int index = 0;

    while (ch != '\"')
    {
        if (ch == EOF)
        {
            fprintf(stderr, "syntax error on line %d\n", lineNumber);
            fprintf(stderr, "string not terminated starting on line %d\n", utStrLineNumber);
            exit(-1);
        }
        if (index > size - 2)
            string = realloc(string, ++size);
        if (ch == '\\')
        {
            ch = fgetc(file);
            if (ch == '\n')
                lineNumber++;
            string[index] = convertToChar(ch);
        }
        else
            string[index] = ch;
        index++;
        ch = fgetc(file);
        if (ch == '\n')
            lineNumber++;
    }

    string[index] = '\0';

    return newStrLexeme(STRING, string);
}

int illegalCharacter(char character)
{
    switch (character)
    {
    case EOF:
        return 1;
    case '(':
        return 1;
    case ')':
        return 1;
    case ',':
        return 1;
    case '+':
        return 1;
    case '*':
        return 1;
    case '-':
        return 1;
    case '%':
        return 1;
    case '/':
        return 1;
    case '\'':
        return 1;
    case '<':
        return 1;
    case '>':
        return 1;
    case '=':
        return 1;
    case '.':
        return 1;
    case '{':
        return 1;
    case '}':
        return 1;
    case '\"':
        return 1;
    default:
        return 0;
    }
}

lexeme
    *
    lexVariableOrKeyword(FILE *file)
{
    char ch = fgetc(file);
    int size = 64;
    char *name = malloc(size);

    int index = 0;
    while (!isWhitespace(ch) && !illegalCharacter(ch))
    {
        if (index > size - 2)
            name = realloc(name, ++size);
        name[index] = ch;
        index++;
        ch = fgetc(file);
    }

    ungetc(ch, file);

    name[index] = '\0';
    return newVarOrKeyLexeme(VARIABLE, name);
}

lexeme
    *
    lex(FILE *file)
{
    skipWhiteSpace(file);
    char ch = fgetc(file);
    if (ch == EOF)
        return newLexeme(ENDofINPUT);
    switch (ch)
    {
    case '(':
        return newLexeme(OPAREN);
    case ')':
        return newLexeme(CPAREN);
    case ',':
        return newLexeme(COMMA);
    case '+':
        return newLexeme(PLUS);
    case '*':
        return newLexeme(TIMES);
    case '-':
        ch = fgetc(file);
        if (isdigit(ch))
        {
            ungetc(ch, file);
            return lexNumber(file, 1);
        }
        ungetc(ch, file);
        return newLexeme(MINUS);
        break;
    case '/':
        return newLexeme(DIVIDES);
    case '%':
        return newLexeme(MOD);
    case '<':
        return newLexeme(LESSTHAN);
    case '>':
        return newLexeme(GREATERTHAN);
    case '=':
        return newLexeme(EQUALS);
    case '.':
        return newLexeme(PERIOD);
    case '{':
        return newLexeme(OBRACKET);
    case '}':
        return newLexeme(CBRACKET);

    default:
        if (isdigit(ch))
        {
            ungetc(ch, file);
            return lexNumber(file, 0);
        }
        else if (isalpha(ch))
        {
            ungetc(ch, file);
            return lexVariableOrKeyword(file);
        }
        else if (ch == '\"')
        {
            return lexString(file);
        }
        else
            fprintf(stdout, "syntax error on line\n");
        fprintf(stdout, "unknown interpretation of '%c'\n", ch);
        exit(-1);
    }
    return 0;
}

void displayEnv(FILE *out, lexeme *env)
{
    fprintf(out, "E%d\n", env->envNum);
    lexeme *values = env->right->left;
    while (values)
    {
        if (values->type == INTEGER)
            fprintf(out, "    %s: %d\n", values->name, values->integer);
        else if (values->type == STRING)
            fprintf(out, "    %s: %s\n", values->name, values->string);
        else if (values->type == FUNCTION)
        {
            fprintf(out, "    %s: ", values->name);
            disp(values);
        }
        else
        {
            fprintf(out, "    %s: ", values->name);
            disp(values);
        }
        values = values->left;
    }
}

void displayLexeme(FILE *out, lexeme *l)
{
    if (!l)
    {
        fprintf(out, "null\n");
        return;
    }
    char *type;
    switch (l->type)
    {
    case ENDofINPUT:
        type = "ENDofINPUT";
        break;
    case NIL:
        type = "NULL";
        break;
    case INTEGER:
        type = "INTEGER";
        break;
    case STRING:
        type = "STRING";
        break;
    case OPAREN:
        type = "OPAREN";
        break;
    case CPAREN:
        type = "CPAREN";
        break;
    case COMMA:
        type = "COMMA";
        break;
    case PLUS:
        type = "PLUS";
        break;
    case MINUS:
        type = "MINUS";
        break;
    case TIMES:
        type = "TIMES";
        break;
    case DIVIDES:
        type = "DIVIDES";
        break;
    case MOD:
        type = "MOD";
        break;
    case PERIOD:
        type = "PERIOD";
        break;
    case OBRACKET:
        type = "OBRACKET";
        break;
    case CBRACKET:
        type = "CBRACKET";
        break;
    case LESSTHAN:
        type = "LESSTHAN";
        break;
    case GREATERTHAN:
        type = "GREATERTHAN";
        break;
    case EQUALS:
        type = "EQUALS";
        break;
    case AS:
        type = "AS";
        break;
    case VALUEOF:
        type = "VALUEOF";
        break;
    case GIVEBACK:
        type = "GIVEBACK";
        break;
    case FUNCTION:
        type = "FUNCTION";
        break;
    case DECLARE:
        type = "DECLARE";
        break;
    case VARIABLE:
        type = "VARIABLE";
        break;
    case DISPLAY:
        type = "DISPLAY";
        break;
    case IF:
        type = "IF";
        break;
    case IS:
        type = "IS";
        break;
    case AND:
        type = "AND";
        break;
    case OR:
        type = "OR";
        break;
    case THENDOTHIS:
        type = "THENDOTHIS";
        break;
    case ARRAY:
        type = "ARRAY";
        break;
    case OFSIZE:
        type = "OFSIZE";
        break;
    case HOLDING:
        type = "HOLDING";
        break;
    case ITEMATINDEX:
        type = "ITEMATINDEX";
        break;
    case OF:
        type = "OF";
        break;
    case INT:
        type = "INT";
        break;
    case STR:
        type = "STR";
        break;
    case WHILE:
        type = "WHILE";
        break;
    case CHANGE:
        type = "CHANGE";
        break;
    case SET:
        type = "SET";
        break;
    case TOBE:
        type = "TOBE";
        break;
    case ELSE:
        type = "ELSE";
        break;
    case PROGRAM:
        type = "PROGRAM";
        break;
    case NEXT:
        type = "NEXT";
        break;
    case BODY:
        type = "BODY";
        break;
    case PARAMS:
        type = "PARAMS";
        break;
    case LOGIC:
        type = "LOGIC";
        break;
    case COND:
        type = "COND";
        break;
    case CALL:
        type = "CALL";
        break;
    case MATH:
        type = "MATH";
        break;
    case ENV:
        type = "ENV";
        displayEnv(out, l);
        break;
    case PRINTDONE:
        type = "PRINTDONE";
        break;
    case T:
        type = "TRUE";
        break;
    case F:
        type = "FALSE";
        break;
    case DISPLAYLN:
        type = "DISPLAYLN";
        break;
    default:
        break;
    }
    fprintf(out, "%s", type);
    if (strcmp(type, "INTEGER") == 0)
        fprintf(out, ": %d\n", l->integer);
    else if (strcmp(type, "STRING") == 0)
        fprintf(out, ": \"%s\"\n", l->string);
    else if (strcmp(type, "VARIABLE") == 0)
        fprintf(out, ": %s\n", l->name);
    else
        fprintf(out, "\n");
}

void displayType(FILE *out, lexemeType t)
{
    char *type;
    switch (t)
    {
    case ENDofINPUT:
        type = "ENDofINPUT";
        break;
    case NIL:
        type = "NULL";
        break;
    case INTEGER:
        type = "INTEGER";
        break;
    case STRING:
        type = "STRING";
        break;
    case OPAREN:
        type = "OPAREN";
        break;
    case CPAREN:
        type = "CPAREN";
        break;
    case COMMA:
        type = "COMMA";
        break;
    case PLUS:
        type = "PLUS";
        break;
    case MINUS:
        type = "MINUS";
        break;
    case TIMES:
        type = "TIMES";
        break;
    case DIVIDES:
        type = "DIVIDES";
        break;
    case MOD:
        type = "MOD";
        break;
    case PERIOD:
        type = "PERIOD";
        break;
    case OBRACKET:
        type = "OBRACKET";
        break;
    case CBRACKET:
        type = "CBRACKET";
        break;
    case LESSTHAN:
        type = "LESSTHAN";
        break;
    case GREATERTHAN:
        type = "GREATERTHAN";
        break;
    case EQUALS:
        type = "EQUALS";
        break;
    case AS:
        type = "AS";
        break;
    case VALUEOF:
        type = "VALUEOF";
        break;
    case GIVEBACK:
        type = "GIVEBACK";
        break;
    case FUNCTION:
        type = "FUNCTION";
        break;
    case DECLARE:
        type = "DECLARE";
        break;
    case VARIABLE:
        type = "VARIABLE";
        break;
    case DISPLAY:
        type = "DISPLAY";
        break;
    case IF:
        type = "IF";
        break;
    case IS:
        type = "IS";
        break;
    case AND:
        type = "AND";
        break;
    case OR:
        type = "OR";
        break;
    case THENDOTHIS:
        type = "THENDOTHIS";
        break;
    case ARRAY:
        type = "ARRAY";
        break;
    case OFSIZE:
        type = "OFSIZE";
        break;
    case HOLDING:
        type = "HOLDING";
        break;
    case ITEMATINDEX:
        type = "ITEMATINDEX";
        break;
    case OF:
        type = "OF";
        break;
    case INT:
        type = "INT";
        break;
    case STR:
        type = "STR";
        break;
    case WHILE:
        type = "WHILE";
        break;
    case CHANGE:
        type = "CHANGE";
        break;
    case SET:
        type = "SET";
        break;
    case TOBE:
        type = "TOBE";
        break;
    case ELSE:
        type = "ELSE";
        break;
    case PROGRAM:
        type = "PROGRAM";
        break;
    case NEXT:
        type = "NEXT";
        break;
    case BODY:
        type = "BODY";
        break;
    case PARAMS:
        type = "PARAMS";
        break;
    case LOGIC:
        type = "LOGIC";
        break;
    case COND:
        type = "COND";
        break;
    case CALL:
        type = "CALL";
        break;
    case MATH:
        type = "MATH";
        break;
    case ENV:
        type = "ENV";
        break;
    case TABLE:
        type = "TABLE";
        break;
    case PRINTDONE:
        type = "PRINTDONE";
        break;
    case T:
        type = "TRUE";
        break;
    case F:
        type = "FALSE";
        break;
    case DISPLAYLN:
        type = "DISPLAYLN";
        break;
    }
    fprintf(out, "%s", type);
}