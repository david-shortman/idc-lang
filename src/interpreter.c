#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "pretty.h"
#include "env.h"
#include "eval.h"

int main(int argc, char **argv)
{

    FILE *file = fopen(argv[1], "r");
    lexeme *t = parse(file);
    fclose(file);
    if (argc == 3)
    {
        pretty(t);
        fprintf(stdout, "\n");
        treePrint(t, 0, 0);
        fprintf(stdout, "\n");
    }
    lexeme *global = createEnv();
    (void)evaluate(t, global);
    return 0;
}