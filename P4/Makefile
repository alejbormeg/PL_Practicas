.SUFFIXES:

ana8: y.tab.c
	gcc -w -o ana8 y.tab.c

y.tab.c: analizador_semantico.y lex.yy.c
	bison -d -v -o y.tab.c analizador_semantico.y

lex.yy.c: analizador_lexico.lex
	flex analizador_lexico.lex

limpia:
	rm ana8 y.tab.c lex.yy.c y.output y.tab.h

todo:
	#make --no-print-directory limpia
	make --no-print-directory ana8

# Test
test_ana8: todo
	./ana8 < ./programa2.j8

test_sintactico: todo
	./ana8 < ../pruebas/errores_sintacticos.txt
