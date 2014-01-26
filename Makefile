all: cmd.o algoritmos.o lista.o 
	gcc -o shell shell.c  cmd.o lista.o algoritmos.o 
	
algoritmos.o: algoritmos.c algoritmos.h
	gcc -c algoritmos.c
           
cmd.o: cmd.c cmd.h
	gcc -c cmd.c

lista.o: lista.c lista.h
	 gcc -c lista.c

clean:
	rm *.o
