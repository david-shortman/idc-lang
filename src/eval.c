#include "lex.h"
#include "parser.h"
#include "pretty.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexeme *evaluate(lexeme *, lexeme *);
int functionID = 0;

lexeme
    *
    disp(lexeme *val, int newLine)
{
    switch (val->type)
    {
    case NIL:
        fprintf(stdout, "null");
        break;
    case INTEGER:
        fprintf(stdout, "%d", val->integer);
        break;
    case STRING:
        fprintf(stdout, "%s", val->string);
        break;
    case FUNCTION:
        fprintf(stdout, "function<");
        if (val->func && val->func->left)
            pParams(val->func->left, 0);
        else
            pParams(val->left, 0);
        fprintf(stdout, ">"); //fprintf(stdout, "> id: %d\n", val->func->integer);
        break;
    case PRINTDONE:
        fprintf(stdout, "PRINTDONE");
        break;
    case ARRAY:
        fprintf(stdout, "[");
        for (int i = 0; i < val->arraySize; i++)
        {
            if (i)
                fprintf(stdout, ", ");
            if (val->array[i])
            {
                if (val->array[i]->type == STRING)
                    fprintf(stdout, "\"");
                disp(val->array[i], 0);
                if (val->array[i]->type == STRING)
                    fprintf(stdout, "\"");
            }
            else
                fprintf(stdout, "null");
        }
        fprintf(stdout, "]");
        break;
    default:
        fprintf(stdout, "can't print type ");
        displayType(stdout, val->type);
        fprintf(stdout, "\n");
        exit(-1);
    }
    if (newLine == 1)
        fprintf(stdout, "\n");
    return newLexeme(PRINTDONE);
}

lexeme
    *
    evalArgs(lexeme *args, lexeme *env)
{
    lexeme *eArgs = newLexeme(NEXT);
    lexeme *cur = eArgs;
    while (args)
    {
        cur->left = evaluate(args->left, env);
        cur->right = newLexeme(NEXT);
        cur = cur->right;
        args = args->right;
    }
    return eArgs;
}

lexeme
    *
    bindArgs(lexeme *parameters, lexeme *args, lexeme *call)
{
    if (!parameters)
    {
        if (!args->left)
            return NULL;
        fprintf(stdout, "too many arguments supplied for func call '%s' | line %d\n", call->left->name, call->lineNumber);
        exit(-1);
    }
    lexeme *bArgs = newLexeme(VARIABLE);
    lexeme *cur = bArgs;
    while (parameters)
    {
        if (!args->left)
        {
            fprintf(stdout, "no argument supplied for argument '%s' in function call '%s' | line %d\n", parameters->left->name, call->left->name, call->lineNumber);
            exit(-1);
        }
        cur->name = parameters->left->name;
        cur->integer = args->left->integer;
        cur->string = args->left->string;
        if (args->left->func)
            cur->func = args->left->func;
        else
            cur->func = args->left;
        cur->type = args->left->type;
        cur->array = args->left->array;
        cur->arraySize = args->left->arraySize;
        if (parameters->right)
        {
            cur->left = newLexeme(VARIABLE);
        }
        cur = cur->left;
        parameters = parameters->right;
        args = args->right;
    }
    if (args->left)
    {
        fprintf(stdout, "too many arguments supplied for func call '%s' | line %d\n", call->left->name, call->lineNumber);
        exit(-1);
    }
    return bArgs;
}

lexeme
    *
    evaluate(lexeme *t, lexeme *env)
{
    if (!t)
        return NULL;
    lexeme *closure = NULL;
    lexeme *args = NULL;
    lexeme *params = NULL;
    lexeme *body = NULL;
    lexeme *senv = NULL;
    lexeme *eargs = NULL;
    lexeme *xenv = NULL;
    lexeme *op1 = NULL;
    lexeme *op2 = NULL;
    lexeme *f = NULL;
    lexeme *result = NULL;
    lexeme *index = NULL;
    int arrSize = 0;
    int temp = 0;
    if (t->type == ARRAY)
        arrSize = t->right->integer;
    //fprintf(stdout,"making array of size %d\n", arrSize);
    lexeme **arr = malloc(arrSize * sizeof(lexeme *) + 1);
    //fprintf(stdout, "size of array is %d\n", sizeof(arr) / sizeof(lexeme*));
    switch (t->type)
    {
    case PROGRAM:
        (void)evaluate(t->left, env);
        return evaluate(t->right, env);
        break;
    case NEXT:
        if (t->right)
        {
            (void)evaluate(t->left, env);
            return evaluate(t->right, env);
        }
        return evaluate(t->left, env);
        break;
    case DISPLAY:
        return disp(evaluate(t->left, env), 0);
        break;
    case DISPLAYLN:
        return disp(evaluate(t->left, env), 1);
        break;
    case DECLARE:
        f = evaluate(t->right, env);
        if (f->type == FUNCTION && f->func)
            return insertEnv(t->left, f->func, env);
        return insertEnv(t->left, evaluate(t->right, env), env);
        break;
    case SET:
        f = evaluate(t->right, env);
        index = evaluate(t->left->left, env);
        result = lookupEnv(t->left->right, env);
        if (index->integer > result->arraySize - 1)
        {
            fprintf(stdout, "index %d too big for array %s | line %d\n", index->integer, t->left->right->name, t->lineNumber);
            exit(-1);
        }
        if (f->type == FUNCTION && f->func)
            lookupEnv(t->left->right, env)->array[index->integer] = f->func;
        else
            lookupEnv(t->left->right, env)->array[index->integer] = f;
        return f;
        break;
    case NIL:
        return t;
        break;
    case INTEGER:
        return t;
        break;
    case STRING:
        return t;
        break;
    case VARIABLE:
        return lookupEnv(t, env);
        break;
    case CHANGE:
        //fprintf(stdout, "************calling change\n");
        //displayLexeme(stdout, t->left);
        //fprintf(stdout, "***up\n");
        f = evaluate(t->right, env);
        //fprintf(stdout, "************finished evaluating\n");
        if (f->type == FUNCTION && f->func)
            return updateEnv(t->left, f->func, env);
        return updateEnv(t->left, evaluate(t->right, env), env);
        break;
    case ITEMATINDEX:
        temp = evaluate(t->right, env)->integer;
        f = evaluate(lookupEnv(t->left, env)->array[temp], env);
        if (!f)
        {
            fprintf(stdout, "nothing stored at index %d of array %s\n", t->right->integer, t->left->name);
            exit(-1);
        }
        return f;
        break;
    case FUNCTION:
        f = car(t);
        f->left = t->left;
        f->right = t->right;
        f->defEnv = env;
        f->integer = functionID++;
        return f;
        break;
    case CALL:
        f = lookupEnv(t->left, env);
        //displayLexeme(stdout, f);
        if (f->type != FUNCTION)
        {
            fprintf(stdout, "tried calling ");
            displayType(stdout, f->type);
            fprintf(stdout, " as a function :( | line %d\n", f->lineNumber);
            exit(-1);
        }
        closure = lookupEnv(t->left, env)->func;
        args = t->right;
        if (closure->left)
            params = closure->left->left;
        body = closure->right;
        senv = closure->defEnv;
        eargs = evalArgs(args, env);
        xenv = extendEnv(bindArgs(params, eargs, t), senv);
        return evaluate(body, xenv);
        break;
    case MATH:
        if (!t->right)
            return evaluate(t->left, env);
        switch (t->right->type)
        {
        case PLUS:
            return newIntLexeme(INTEGER, evaluate(t->left, env)->integer + evaluate(t->right->left, env)->integer);
            break;
        case MINUS:
            return newIntLexeme(INTEGER, evaluate(t->left, env)->integer - evaluate(t->right->left, env)->integer);
            break;
        case TIMES:
            return newIntLexeme(INTEGER, evaluate(t->left, env)->integer * evaluate(t->right->left, env)->integer);
            break;
        case DIVIDES:
            if (!evaluate(t->right->left, env)->integer)
            {
                fprintf(stdout, "fatal: divide by zero on line %d\n", t->lineNumber);
                exit(-1);
            }
            return newIntLexeme(INTEGER, evaluate(t->left, env)->integer / evaluate(t->right->left, env)->integer);
            break;
        case MOD:
            if (!evaluate(t->right->left, env)->integer)
            {
                fprintf(stdout, "fatal: divide by zero on line %d\n", t->lineNumber);
                exit(-1);
            }
            return newIntLexeme(INTEGER, evaluate(t->left, env)->integer % evaluate(t->right->left, env)->integer);
            break;
        default:
            fprintf(stdout, "fatal: invalid operator ");
            displayType(stdout, t->type);
            fprintf(stdout, "\n");
            exit(-1);
        }
        break;
    case ARRAY:
        t->array = arr;
        t->arraySize = arrSize;
        t->defEnv = env;
        for (int i = 0; i < t->right->integer; i++)
        {
            t->array[i] = NULL;
        }
        return t;
        break;
    case IF:
        if (evaluate(t->left, env)->type == T)
            return evaluate(t->right->left, env);
        else if (t->right->right)
            return evaluate(t->right->right, env);
        return NULL;
        break;
    case WHILE:
        while (evaluate(t->left, env)->type == T)
            result = evaluate(t->right->left, env);
        return result;
        break;
    case COND:
        op1 = evaluate(t->left, env);
        op2 = evaluate(t->right->left, env);
        switch (t->right->type)
        {
        case GREATERTHAN:
            if (op1->type != op2->type)
            {
                fprintf(stdout, "can't use '<' or '>' to compare unlike things | line %d\n", t->lineNumber);
                exit(-1);
            }
            if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) > 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) > 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) > 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer > op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer > op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer > op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        case LESSTHAN:
            if (op1->type != op2->type)
            {
                fprintf(stdout, "can't use '<' or '>' to compare unlike things | line %d\n", t->lineNumber);
                exit(-1);
            }
            if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) < 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) < 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) < 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer < op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer < op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer < op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        case EQUALS:
            if (op1->type != op2->type)
            {
                if (!t->right->right)
                {
                    return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        return newLexeme(F);
                    }
                }
            }
            else if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) == 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) == 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) == 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer == op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer == op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer == op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        default:
            fprintf(stdout, "you beat the parser, but you lost the game\n");
            exit(-1);
            break;
        }
        break;
    case OR:
        op1 = evaluate(t->left, env);
        op2 = evaluate(t->right->left, env);
        if (op1->type != op2->type)
        {
            return newLexeme(F);
        }
        switch (t->right->type)
        {
        case GREATERTHAN:
            if (op1->type != op2->type)
            {
                fprintf(stdout, "can't use '<' or '>' to compare unlike things | line %d\n", t->lineNumber);
                exit(-1);
            }
            if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) > 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) > 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) > 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer > op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer > op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer > op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        case LESSTHAN:
            if (op1->type != op2->type)
            {
                fprintf(stdout, "can't use '<' or '>' to compare unlike things | line %d\n", t->lineNumber);
                exit(-1);
            }
            if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) < 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) < 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) < 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer < op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer < op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer < op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        case EQUALS:
            if (op1->type != op2->type)
            {
                if (!t->right->right)
                {
                    return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        return newLexeme(F);
                    }
                }
            }
            else if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) == 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    fprintf(stdout, "\n");
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) == 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) == 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer == op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer == op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer == op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        default:
            fprintf(stdout, "you beat the parser, but you lost the game\n");
            exit(-1);
            break;
        }
        break;
    case AND:
        op1 = evaluate(t->left, env);
        op2 = evaluate(t->right->left, env);
        if (op1->type != op2->type)
        {
            return newLexeme(F);
        }
        switch (t->right->type)
        {
        case GREATERTHAN:
            if (op1->type != op2->type)
            {
                fprintf(stdout, "can't use '<' or '>' to compare unlike things | line %d\n", t->lineNumber);
                exit(-1);
            }
            if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) > 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) > 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) > 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer > op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer > op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer > op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        case LESSTHAN:
            if (op1->type != op2->type)
            {
                fprintf(stdout, "can't use '<' or '>' to compare unlike things | line %d\n", t->lineNumber);
                exit(-1);
            }
            if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) < 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) < 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) < 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer < op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer < op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer < op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        case EQUALS:
            if (op1->type != op2->type)
            {
                if (!t->right->right)
                {
                    return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        return newLexeme(F);
                    }
                }
            }
            else if (op1->type == STRING)
            {
                if (!t->right->right)
                {
                    if (strcmp(op1->string, op2->string) == 0)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    displayType(stdout, t->right->right->type);
                    fprintf(stdout, "\n");
                    if (t->right->right->type == OR)
                    {
                        if (strcmp(op1->string, op2->string) == 0 || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (strcmp(op1->string, op2->string) == 0 && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            else
            {
                if (!t->right->right)
                {
                    if (op1->integer == op2->integer)
                        return newLexeme(T);
                    else
                        return newLexeme(F);
                }
                else
                {
                    if (t->right->right->type == OR)
                    {
                        if (op1->integer == op2->integer || evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                    else
                    {
                        if (op1->integer == op2->integer && evaluate(t->right->right, env)->type == T)
                            return newLexeme(T);
                        else
                            return newLexeme(F);
                    }
                }
            }
            break;
        default:
            fprintf(stdout, "you beat the parser, but you lost the game\n");
            exit(-1);
            break;
        }
        break;
    default:
        fprintf(stdout, "fatal: can't evaluate ");
        displayType(stdout, t->type);
        fprintf(stdout, "\n");
        exit(-1);
        break;
    }
    return NULL;
}