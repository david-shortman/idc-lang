#include "lex.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexeme *CurrentLexeme = NULL;
lexeme *parameters(FILE *);
lexeme *statement(FILE *);
lexeme *expression(FILE *);
lexeme *optParamList(FILE *);
lexeme *mathExpression(FILE *);
lexeme *value(FILE *);
lexeme *itemAtIndex(FILE *);
int itemAtIndexPending();

lexemeType
type(lexeme *l)
{
    return l->type;
}

int check(lexemeType t)
{
    if (type(CurrentLexeme) == t)
        return 1;
    return 0;
}

lexeme
    *
    advance(FILE *file)
{
    lexeme *temp = CurrentLexeme;
    CurrentLexeme = lex(file);
    return temp;
}

void matchNoAdvance(lexemeType type)
{
    if (!check(type))
    {
        fprintf(stdout, "syntactic error on line %d\n", CurrentLexeme->lineNumber);
        fprintf(stdout, "expected ");
        displayType(stdout, type);
        fprintf(stdout, "\n");
        fprintf(stdout, "got this instead: ");
        displayLexeme(stdout, CurrentLexeme);
        fprintf(stdout, "\n");
        exit(-1);
    }
}

lexeme
    *
    match(FILE *file, lexemeType type)
{
    matchNoAdvance(type);
    return advance(file);
}

int nullPending()
{
    return check(NIL);
}

int integerPending()
{
    return check(INTEGER);
}
int stringPending()
{
    return check(STRING);
}
int variablePending()
{
    return check(VARIABLE);
}

int functionPending()
{
    return check(FUNCTION);
}

int variableDeclarationPending()
{
    return check(DECLARE);
}

int displayPending()
{
    return check(DISPLAY);
}

int displayLnPending()
{
    return check(DISPLAYLN);
}

int moreParamsPending()
{
    return check(COMMA);
}

lexeme
    *
    optMoreParams(FILE *file)
{
    lexeme *t = NULL;
    if (moreParamsPending())
    {
        match(file, COMMA);
        t = optParamList(file);
    }
    return t;
}

lexeme
    *
    parameters(FILE *file)
{
    lexeme *t = newLexeme(NEXT);
    t->left = match(file, VARIABLE);
    if (moreParamsPending())
    {
        match(file, COMMA);
        t->right = parameters(file);
    }
    return t;
}

lexeme
    *
    optParamList(FILE *file)
{
    lexeme *t = NULL;
    if (!check(CPAREN))
    {
        t = newLexeme(PARAMS);
        t->left = parameters(file);
    }
    return t;
}

int statementPending()
{
    return !check(CBRACKET);
}

lexeme
    *
    optFunction(FILE *file)
{
    if (statementPending())
    {
        lexeme *t = newLexeme(NEXT);
        t->left = statement(file);
        t->right = optFunction(file);
        return t;
    }
    return NULL;
}

lexeme
    *
    functionBody(FILE *file)
{
    lexeme *t = newLexeme(NEXT);
    t->left = statement(file);
    t->right = optFunction(file);
    return t;
}

lexeme
    *
    function(FILE *file)
{
    lexeme *t = match(file, FUNCTION);
    match(file, OPAREN);
    t->left = optParamList(file);
    match(file, CPAREN);
    match(file, OBRACKET);
    t->right = functionBody(file);
    match(file, CBRACKET);
    return t;
}

int paramPending()
{
    return check(COMMA);
}

int expressionPending()
{
    return check(NIL) || check(VARIABLE) || check(INTEGER) || check(STRING) || check(VALUEOF) || check(OPAREN) || check(ITEMATINDEX) || check(FUNCTION);
}

lexeme
    *
    optParams(FILE *file)
{
    lexeme *t = NULL;
    if (expressionPending())
    {
        t = newLexeme(NEXT);
        t->left = expression(file);
        if (paramPending())
        {
            match(file, COMMA);
            t->right = optParams(file);
        }
    }
    return t;
}

lexeme
    *
    functionCall(FILE *file)
{
    match(file, VALUEOF);
    lexeme *t = newLexeme(CALL);
    t->left = match(file, VARIABLE);
    match(file, OPAREN);
    t->right = optParams(file);
    match(file, CPAREN);
    return t;
}

int functionCallPending()
{
    return check(VALUEOF);
}

int valuePending()
{
    return check(VALUEOF) || check(OPAREN);
}

int timesPending()
{
    return check(TIMES);
}

int dividePending()
{
    return check(DIVIDES);
}

int plusPending()
{
    return check(PLUS);
}

int minusPending()
{
    return check(MINUS);
}

int modPending()
{
    return check(MOD);
}

int operatorPending()
{
    return (plusPending() || minusPending() || timesPending() || dividePending() || modPending());
}

lexeme
    *
    operator(FILE *file)
{
    lexeme *t;
    if (timesPending())
        t = match(file, TIMES);
    else if (dividePending())
        t = match(file, DIVIDES);
    else if (plusPending())
        t = match(file, PLUS);
    else if (modPending())
        t = match(file, MOD);
    else
        t = match(file, MINUS);
    return t;
}

lexeme
    *
    mathExpression(FILE *file)
{
    lexeme *t = newLexeme(MATH);
    if (integerPending())
        t->left = match(file, INTEGER);
    else if (variablePending())
        t->left = match(file, VARIABLE);
    else if (itemAtIndexPending())
        t->left = itemAtIndex(file);
    else
        t->left = value(file);
    if (operatorPending())
    {
        t->right = operator(file);
        t->right->left = mathExpression(file);
    }
    return t;
}

lexeme
    *
    value(FILE *file)
{
    lexeme *t;
    if (functionCallPending())
        t = functionCall(file);
    else
    {
        match(file, OPAREN);
        t = mathExpression(file);
        match(file, CPAREN);
    }
    return t;
}

int itemAtIndexPending()
{
    return check(ITEMATINDEX);
}

lexeme
    *
    itemAtIndex(FILE *file)
{
    lexeme *t = match(file, ITEMATINDEX);
    if (expressionPending())
        t->right = expression(file);
    else if (variablePending())
        t->right = match(file, VARIABLE);
    else
        t->right = match(file, INTEGER);
    match(file, OF);
    t->left = match(file, VARIABLE);
    return t;
}

lexeme
    *
    expression(FILE *file)
{
    lexeme *t;
    if (nullPending(file))
        t = match(file, NIL);
    else if (integerPending(file))
        t = match(file, INTEGER);
    else if (stringPending(file))
        t = match(file, STRING);
    else if (variablePending(file))
        t = match(file, VARIABLE);
    else if (functionPending(file))
        t = function(file);
    else if (itemAtIndexPending(file))
        t = itemAtIndex(file);
    else
        t = value(file);
    return t;
}

int arrayDecPending()
{
    return check(ARRAY);
}

lexeme
    *
    arrayDec(FILE *file)
{
    lexeme *t = match(file, ARRAY);
    match(file, OFSIZE);
    t->right = match(file, INTEGER);
    return t;
}

lexeme
    *
    variableDeclaration(FILE *file)
{
    lexeme *t = match(file, DECLARE);
    t->left = match(file, VARIABLE);
    match(file, AS);
    if (arrayDecPending())
        t->right = arrayDec(file);
    else
        t->right = expression(file);
    match(file, PERIOD);
    return t;
}

lexeme
    *
    display(FILE *file)
{
    match(file, DISPLAY);
    lexeme *t = newLexeme(DISPLAY);
    t->left = expression(file);
    match(file, PERIOD);
    return t;
}

lexeme
    *
    displayLn(FILE *file)
{
    match(file, DISPLAYLN);
    lexeme *t = newLexeme(DISPLAYLN);
    t->left = expression(file);
    match(file, PERIOD);
    return t;
}

lexeme
    *
    giveback(FILE *file)
{
    match(file, GIVEBACK);
    lexeme *t;
    if (valuePending())
        t = value(file);
    else if (integerPending())
        t = match(file, INTEGER);
    else if (stringPending())
        t = match(file, STRING);
    else if (variablePending())
        t = match(file, VARIABLE);
    else
        t = functionCall(file);
    match(file, PERIOD);
    return t;
}

int givebackPending()
{
    return check(GIVEBACK);
}

int conditionalPending()
{
    return check(IF);
}

int comparatorPending()
{
    return check(LESSTHAN) || check(GREATERTHAN) || check(EQUALS);
}

lexeme
    *
    comparator(FILE *file)
{
    lexeme *t;
    if (check(LESSTHAN))
        t = match(file, LESSTHAN);
    else if (check(GREATERTHAN))
        t = match(file, GREATERTHAN);
    else
        t = match(file, EQUALS);
    return t;
}

int boolOpPending()
{
    return check(AND) || check(OR);
}

lexeme
    *
    boolOp(FILE *file)
{
    lexeme *t;
    if (check(AND))
        t = match(file, AND);
    else if (check(OR))
        t = match(file, OR);
    else
        t = match(file, EQUALS);
    return t;
}

lexeme
    *
    optMoreConditional(FILE *file)
{
    lexeme *t = NULL;
    if (boolOpPending())
    {
        t = boolOp(file);
        t->left = expression(file);
        match(file, IS);
        t->right = comparator(file);
        t->right->left = expression(file);
        t->right->right = optMoreConditional(file);
    }
    return t;
}

int elsePending()
{
    return check(ELSE);
}

lexeme
    *
    optElse(FILE *file)
{
    lexeme *t = NULL;
    if (elsePending())
    {
        match(file, ELSE);
        match(file, THENDOTHIS);
        match(file, OBRACKET);
        t = functionBody(file);
        match(file, CBRACKET);
    }
    return t;
}

lexeme
    *
    conditional(FILE *file)
{
    lexeme *t = match(file, IF);
    t->left = newLexeme(COND);
    t->left->left = expression(file);
    match(file, IS);
    t->left->right = comparator(file);
    t->left->right->left = expression(file);
    t->left->right->right = optMoreConditional(file);
    t->right = match(file, THENDOTHIS);
    match(file, OBRACKET);
    t->right->left = functionBody(file);
    match(file, CBRACKET);
    t->right->right = optElse(file);
    match(file, PERIOD);
    return t;
}

int whileLoopPending()
{
    return check(WHILE);
}

lexeme
    *
    whileLoop(FILE *file)
{
    lexeme *t = match(file, WHILE);
    t->left = newLexeme(COND);
    t->left->left = expression(file);
    match(file, IS);
    t->left->right = comparator(file);
    t->left->right->left = expression(file);
    t->left->right->right = optMoreConditional(file);
    t->right = match(file, THENDOTHIS);
    match(file, OBRACKET);
    t->right->left = functionBody(file);
    match(file, CBRACKET);
    match(file, PERIOD);
    return t;
}

int changePending()
{
    return check(CHANGE);
}

lexeme
    *
    change(FILE *file)
{
    lexeme *t = match(file, CHANGE);
    t->left = match(file, VARIABLE);
    match(file, TOBE);
    t->right = expression(file);
    match(file, PERIOD);
    return t;
}

int setIndexPending()
{
    return check(SET);
}

lexeme
    *
    setIndex(FILE *file)
{
    //set index 3 of feet as value.
    lexeme *t = match(file, SET);
    match(file, ITEMATINDEX);
    t->left = newLexeme(NEXT);
    if (variablePending())
        t->left->left = match(file, VARIABLE);
    else if (expressionPending())
        t->left->left = expression(file);
    else
        t->left->left = match(file, INTEGER);
    match(file, OF);
    t->left->right = match(file, VARIABLE);
    match(file, AS);
    t->right = expression(file);
    match(file, PERIOD);
    return t;
}

lexeme
    *
    statement(FILE *file)
{
    lexeme *t;
    if (displayPending(file))
        t = display(file);
    else if (displayLnPending(file))
        t = displayLn(file);
    else if (givebackPending(file))
        t = giveback(file);
    else if (variableDeclarationPending(file))
        t = variableDeclaration(file);
    else if (conditionalPending(file))
        t = conditional(file);
    else if (whileLoopPending(file))
        t = whileLoop(file);
    else if (changePending(file))
        t = change(file);
    else if (setIndexPending())
        t = setIndex(file);
    else
    {
        t = expression(file);
        match(file, PERIOD);
    }
    return t;
}

void program(FILE *file, lexeme *t)
{
    if (check(ENDofINPUT))
        return;
    t->left = statement(file);
    t->right = newLexeme(NEXT);
    return program(file, t->right);
}

lexeme
    *
    parse(FILE *file)
{
    CurrentLexeme = lex(file);
    lexeme *root = newLexeme(PROGRAM);
    program(file, root);
    return root;
}