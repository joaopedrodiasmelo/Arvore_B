all:
	gcc -c registro.c -o registro.o
	gcc -c arvore_b.c -o arvore_b.o 
	gcc trabalho03.c registro.o  arvore_b.o  -o trabalho03
run:
	./trabalho03  

clear:	
	rm ./trabalho03