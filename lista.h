#ifndef LISTA_H
#define LISTA_H
#include <sys/types.h>
#include <sys/wait.h>

#define ACT "act"
#define TERM "term"
#define SIG "sig"
#define STOP "stop"
#define ALL  "all"
#define ESTADOS |ACT|TERM|SIG|STOP

typedef struct nodoProceso {
	pid_t pid;
	int prioridad;
	char comando[200];
	char fechaInicio[20];
	char estado[10];
	int valorSenal;
} nodoProc;

struct nodo {
	struct nodoProceso proceso;
	struct nodo *siguiente;
};

typedef struct nodo *pnodo;
typedef pnodo lista;

void crear_lista(lista *listaProcesos);
int lista_vacia(lista l);
pnodo buscar(pid_t pid, lista listaProcesos);
int ultimo_elemento(pnodo p);
pnodo buscar_anterior(char *estado[], lista l);
void eliminarLista(lista l);
void eliminarProceso(char *estado[], lista l);
void insertarLista(nodoProc proc, pnodo p);
#endif
