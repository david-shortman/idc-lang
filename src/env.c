#include "lex.h"
#include "parser.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num = 0;

int sameVariable(lexeme *cur, lexeme *term)
{
    if (!strcmp(cur->name, term->name))
        return 1;
    return 0;
}

lexeme *car(lexeme *l)
{
    lexeme *new = newLexeme(l->type);
    new->integer = l->integer;
    new->lineNumber = l->lineNumber;
    new->name = l->name;
    new->string = l->string;
    new->array = l->array;
    new->arraySize = l->arraySize;
    new->func = l->func;
    new->defEnv = l->defEnv;
    new->type = l->type;
    new->envNum = l->envNum;
    return new;
}

lexeme *createEnv()
{
    lexeme *env = newLexeme(ENV);
    lexeme *table = newLexeme(TABLE);
    env->right = table;
    env->envNum = num++;
    return env;
}

lexeme *lookupEnv(lexeme *variable, lexeme *env)
{
    while (env)
    {
        lexeme *vars = env->right->left;
        while (vars)
        {
            if (sameVariable(variable, vars))
            {
                return car(vars);
            }
            vars = vars->left;
        }
        env = env->left;
    }
    fprintf(stdout, "reference '%s' is not defined | line %d\n", variable->name, variable->lineNumber);
    exit(-1);
    return NULL;
}

lexeme *insertEnv(lexeme *variable, lexeme *value, lexeme *env)
{
    lexeme *var = newLexeme(VARIABLE);
    var->name = variable->name;
    var->integer = value->integer;
    var->string = value->string;
    var->array = value->array;
    var->arraySize = value->arraySize;
    lexeme *v = NULL;
    switch (value->type)
    {
    case NIL:
        var->type = NIL;
        break;
    case INTEGER:
        var->type = INTEGER;
        break;
    case STRING:
        var->type = STRING;
        break;
    case ARRAY:
        var->type = ARRAY;
        break;
    case VARIABLE:
        v = lookupEnv(value, env);
        var->integer = v->integer;
        var->string = v->string;
        var->func = v->func;
        var->array = v->array;
        var->arraySize = v->arraySize;
        var->defEnv = v->defEnv;
        if (var->string)
            var->type = STRING;
        else if (var->func)
            var->type = FUNCTION;
        else if (var->array)
            var->type = ARRAY;
        else
            var->type = INTEGER;
        break;
    case FUNCTION:
        var->func = value;
        var->type = FUNCTION;
        var->defEnv = env;
        break;
    default:
        fprintf(stdout, "don't know how to store ");
        displayType(stdout, value->type);
        fprintf(stdout, "\n");
        exit(-1);
    }
    var->lineNumber = variable->lineNumber;
    var->left = env->right->left;
    env->right->left = var;
    return var;
}

lexeme *extendEnv(lexeme *variables, lexeme *env)
{
    lexeme *e = createEnv();
    e->right->left = variables;
    e->left = env;
    if (env)
        e->envNum = env->envNum + 1;
    return e;
}

lexeme *updateEnv(lexeme *variable, lexeme *value, lexeme *env)
{
    while (env)
    {
        lexeme *vars = env->right->left;
        while (vars)
        {
            if (sameVariable(variable, vars))
            {
                vars->integer = value->integer;
                vars->string = value->string;
                vars->array = value->array;
                vars->arraySize = value->arraySize;
                lexeme *v = NULL;
                switch (value->type)
                {
                case NIL:
                    vars->type = NIL;
                    break;
                case INTEGER:
                    vars->type = INTEGER;
                    break;
                case STRING:
                    vars->type = STRING;
                    break;
                case ARRAY:
                    vars->type = ARRAY;
                    break;
                case VARIABLE:
                    v = lookupEnv(value, env);
                    vars->integer = v->integer;
                    vars->string = v->string;
                    vars->func = v->func;
                    vars->array = v->array;
                    vars->arraySize = v->arraySize;
                    vars->defEnv = v->defEnv;
                    vars->type = v->type;
                    break;
                case FUNCTION:
                    vars->func = value;
                    vars->type = FUNCTION;
                    vars->defEnv = env;
                    break;
                default:
                    fprintf(stdout, "don't know how to store ");
                    displayType(stdout, value->type);
                    fprintf(stdout, "\n");
                    exit(-1);
                }
                return car(vars);
            }
            vars = vars->left;
        }
        env = env->left;
    }
    fprintf(stdout, "cannot update variable '%s'- undefined\n", variable->name);
    exit(-1);
}
