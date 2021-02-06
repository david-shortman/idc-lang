#include "pretty.h"
#include "lex.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void translate(lexeme *, int);

void pIndent(int ind)
{
    while (ind > 0)
    {
        fprintf(stdout, " ");
        ind--;
    }
}

void pDeclare()
{
    fprintf(stdout, "declare");
}

void pSpace()
{
    fprintf(stdout, " ");
}

void pNull()
{
    fprintf(stdout, "null");
}

void pInteger(int i)
{
    fprintf(stdout, "%d", i);
}

void pString(char *s)
{
    fprintf(stdout, "%s", s);
}

void pStringLit(char *s)
{
    fprintf(stdout, "\"%s\"", s);
}

void pAs()
{
    fprintf(stdout, "as");
}

void pPeriod(int ind)
{
    fprintf(stdout, ".\n");
    pIndent(ind);
}

void pThenDoThis(int ind)
{
    fprintf(stdout, "then\n");
    pIndent(ind);
}

void pObracket()
{
    fprintf(stdout, "{");
}

void pCbracket()
{
    fprintf(stdout, "}");
}

void pFunction()
{
    fprintf(stdout, "function");
}

void pDisplay()
{
    fprintf(stdout, "display");
}

void pOparen()
{
    fprintf(stdout, "(");
}

void pIs()
{
    fprintf(stdout, "is");
}

void pCparen()
{
    fprintf(stdout, ")");
}

void pIf()
{
    fprintf(stdout, "if");
}

void pNewLine(int ind)
{
    fprintf(stdout, "\n");
    pIndent(ind);
}

void pElse()
{
    fprintf(stdout, "else");
}

void pArray()
{
    fprintf(stdout, "array");
}

void pOfSize()
{
    fprintf(stdout, "of_size");
}

void pHolding()
{
    fprintf(stdout, "holding");
}

void pItemAtIndex()
{
    fprintf(stdout, "item_at_index");
}

void pOf()
{
    fprintf(stdout, "of");
}

void pInt()
{
    fprintf(stdout, "integers");
}

void pStr()
{
    fprintf(stdout, "strings");
}

void pWhile()
{
    fprintf(stdout, "while");
}

void pToBe()
{
    fprintf(stdout, "to_be");
}

void pChange()
{
    fprintf(stdout, "change");
}

void pValueOf()
{
    fprintf(stdout, "evaluate");
}

void pParams(lexeme *l, int ind)
{
    if (!l)
        return;
    if (l->type == PARAMS)
    {
        pParams(l->left, ind);
        return;
    }
    translate(l->left, ind);
    l = l->right;
    while (l)
    {
        fprintf(stdout, ", ");
        translate(l->left, ind);
        l = l->right;
        if (l && l->type != NEXT)
            break;
    }
}

void translate(lexeme *t, int ind)
{
    if (!t)
        return;
    switch (t->type)
    {
    case NIL:
        pNull();
        break;
    case INTEGER:
        pInteger(t->integer);
        break;
    case STRING:
        pStringLit(t->string);
        break;
    case FUNCTION:
        pFunction();
        pOparen();
        pParams(t->left, ind);
        pCparen();
        pNewLine(ind + 4);
        pObracket();
        pNewLine(ind + 4);
        translate(t->right, ind + 4);
        pCbracket();
        break;
    case DECLARE:
        pDeclare();
        pSpace();
        translate(t->left, ind);
        pSpace();
        pAs();
        pSpace();
        translate(t->right, ind);
        break;
    case VARIABLE:
        pString(t->name);
        break;
    case DISPLAY:
        pDisplay();
        pSpace();
        translate(t->left, ind);
        break;
    case IF:
        pIf();
        pSpace();
        translate(t->left, ind);
        pSpace();
        translate(t->right, ind);
        break;
    case THENDOTHIS:
        pThenDoThis(ind + 4);
        pObracket();
        pNewLine(ind + 4);
        translate(t->left, ind + 4);
        pCbracket();
        if (t->right != NULL)
        {
            pNewLine(ind);
            pElse();
            pSpace();
            pThenDoThis(ind + 4);
            pObracket();
            pNewLine(ind + 4);
            translate(t->right, ind + 4);
            pCbracket();
        }
        break;
    case ARRAY:
        pArray();
        pSpace();
        pOfSize();
        pSpace();
        pInteger(t->right->integer);
        pSpace();
        pHolding();
        pSpace();
        translate(t->left, ind);
        break;
    case ITEMATINDEX:
        pItemAtIndex();
        pSpace();
        pInteger(t->right->integer);
        pSpace();
        pOf();
        pSpace();
        pString(t->left->name);
        break;
    case INT:
        pInt();
        break;
    case STR:
        pStr();
        break;
    case WHILE:
        pWhile();
        pSpace();
        translate(t->left, ind);
        pSpace();
        translate(t->right, ind);
        break;
    case CHANGE:
        pChange();
        pSpace();
        pString(t->left->name);
        pSpace();
        pToBe();
        pSpace();
        translate(t->right, ind);
        break;
    case NEXT:
        if (t->left)
        {
            translate(t->left, ind);
            pPeriod(ind);
            translate(t->right, ind);
        }
        break;
    case PARAMS:
        pParams(t->left, ind);
        break;
    case COND:
        translate(t->left, ind);
        pSpace();
        pIs();
        pSpace();
        translate(t->right, ind);
        break;
    case CALL:
        pValueOf();
        pSpace();
        translate(t->left, ind);
        pOparen();
        pParams(t->right, ind);
        pCparen();
        break;
    case MATH:
        pValueOf();
        pSpace();
        pOparen();
        translate(t->left, ind);
        translate(t->right, ind);
        pCparen();
        break;
    case PLUS:
        fprintf(stdout, " + ");
        translate(t->left, ind);
        break;
    case MINUS:
        fprintf(stdout, " - ");
        translate(t->left, ind);
        break;
    case TIMES:
        fprintf(stdout, " * ");
        translate(t->left, ind);
        break;
    case DIVIDES:
        fprintf(stdout, " / ");
        translate(t->left, ind);
        break;
    case EQUALS:
        fprintf(stdout, "= ");
        translate(t->left, ind);
        if (t->right)
            translate(t->right, ind);
        break;
    case GREATERTHAN:
        fprintf(stdout, "> ");
        translate(t->left, ind);
        if (t->right)
            translate(t->right, ind);
        break;
    case LESSTHAN:
        fprintf(stdout, "< ");
        translate(t->left, ind);
        if (t->right)
            translate(t->right, ind);
        break;
    case OR:
        fprintf(stdout, " or ");
        translate(t->left, ind);
        pSpace();
        pIs();
        pSpace();
        translate(t->right, ind);
        break;
    case AND:
        fprintf(stdout, " and ");
        translate(t->left, ind);
        pSpace();
        pIs();
        pSpace();
        translate(t->right, ind);
        break;
    default:
        fprintf(stdout, "[NotImplemented]");
    }
}

void pretty(lexeme *t)
{
    if (!t)
        return;
    translate(t->left, 0);
    pPeriod(0);
    translate(t->right, 0);
}

void indent(int ind)
{
    while (ind > 0)
    {
        fprintf(stdout, " ");
        ind--;
    }
}

void treePrint(lexeme *t, int level, int ind)
{
    if (!t)
        return;
    indent(ind);
    displayLexeme(stdout, t);
    ++level;
    indent(ind);
    fprintf(stdout, "%d left: \n", level);
    treePrint(t->left, level, ind + 2);
    indent(ind);
    fprintf(stdout, "%d right: \n", level);
    treePrint(t->right, level, ind + 2);
}