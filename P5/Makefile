.SUFFIXES:

traductor: y.tab.c
	gcc -w -o traductor y.tab.c

y.tab.c: analizador_semantico.y lex.yy.c
	bison -d -v -o y.tab.c analizador_semantico.y

lex.yy.c: analizador_lexico.lex
	flex analizador_lexico.lex

limpia:
	rm -f traductor y.tab.c lex.yy.c y.output y.tab.h a.out dec_fun.c prog.c

todo:
	make --no-print-directory limpia
	make --no-print-directory traductor

# Test
test: todo ./j8/programa_simple.j8
	./traductor < ./j8/programa_simple.j8      # traducir a c
	gcc prog.c -w -o ./bin/programa_simple.x   # compilar con gcc
	./bin/programa_simple.x                    # ejecutar
	make --no-print-directory limpia           # eliminar ficheros intermedios    
