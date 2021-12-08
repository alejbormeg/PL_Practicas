.SUFFIXES:

sema8: y.tab.c
	gcc -w -o sema8 y.tab.c

y.tab.c: analizador_semantico.y lex.yy.c
	bison -d -v -o y.tab.c analizador_semantico.y

lex.yy.c: analizador_lexico.lex
	flex analizador_lexico.lex

limpia:
	rm -f sema8 y.tab.c lex.yy.c y.output y.tab.h

todo:
	make --no-print-directory limpia
	make --no-print-directory sema8

# Test
test_programita: todo
	./sema8 < ../pruebas/programa_correcto.prog

test_sintactico: todo
	./sema8 < ../pruebas/errores_sintacticos.prog
