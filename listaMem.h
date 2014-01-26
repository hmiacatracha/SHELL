#ifndef LISTAMEM_H
#define LISTAMEM_H
#include <stdlib.h>
#include <stdio.h>
#define MALLOC "malloc"
#define MMAP "mmap"
#define SHARED "shared"
//#define ALLDIR MALLOC |MMAP|SHARED

typedef struct nodoMemoria{
	void * dir;
	int   tam;
	char fechaAsig[18];
	char  tipoAsig[10];
	int  descriptor; //clave si es shared
	char descriptorFich[100]; //descriptor del fichero si es mmap, ejemplo nombre del fichero.
} tMemoria;

struct nodoMem {
	struct nodoMemoria memoria;
	struct nodoMem *siguiente;
};
typedef struct nodoMem *pnodoMem ;
typedef pnodoMem listaMem;


void crear_listaMem(listaMem *lista);
int lista_vaciaMem(listaMem lista);
int ultimo_elementoMem(pnodoMem p) ;
void insertarListaMem(tMemoria memoria, pnodoMem p);
pnodoMem buscarMemTam(int tam, char *tipo, listaMem lista);
pnodoMem buscarMemFichero(char *fichero, listaMem lista);
pnodoMem buscarMemDireccion(void *dir, listaMem lista);
pnodoMem buscarAnteriorMemFich(char *fich, listaMem lista);
pnodoMem buscarAnteriorMemTam(int tam, char *tipo, listaMem lista);
pnodoMem buscarAnteriorDir(void *dir, listaMem lista);
void eliminarMemTam(int tam, char *tipo, listaMem lista);
void eliminarMemFichero(char *fich, listaMem lista);
void eliminarMemDireccion(void *dir, listaMem lista);
void eliminarListaMem(listaMem listaMem);


#endif
