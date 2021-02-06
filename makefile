OBJD = lex.o parser.o pretty.o env.o eval.o interpreter.o
OOPTS = -Wall -std=c99 -Wextra -g -c
LOPTS = -Wall -std=c99 -Wextra -g

all :	idcInterpreter

idcInterpreter :	$(OBJD)
	gcc $(LOPTS) -o idcInterpreter $(OBJD)

interpreter.o :	src/interpreter.c src/parser.h src/pretty.h
	gcc $(OOPTS) src/interpreter.c

eval.o :	src/eval.c src/eval.h src/lex.h src/parser.h src/env.h src/pretty.h
	gcc $(OOPTS) src/eval.c

env.o :	src/env.c src/env.h src/lex.h src/parser.h
	gcc $(OOPTS) src/env.c

parser.o :	src/parser.c src/lex.h
	gcc $(OOPTS) src/parser.c

pretty.o :	src/pretty.c src/pretty.h
	gcc $(OOPTS) src/pretty.c

lex.o :	src/lex.c src/lex.h src/eval.h
	gcc $(OOPTS) src/lex.c

clean :
	rm -f $(OBJD) idcInterpreter *.o
