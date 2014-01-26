#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"

void crear_lista(lista *listaProcesos) {
	pnodo tmp = (pnodo) malloc(sizeof(struct nodo));
	if (tmp == NULL) {
		printf("memoria agotada\n");
	}
	tmp->siguiente = NULL;
	*listaProcesos = tmp;
}

void insertarLista(nodoProc proc, pnodo p) {
	pnodo tmp = (pnodo) malloc(sizeof(struct nodo));
	if (tmp == NULL) {
		printf("memoria agotada\n");
	}
	tmp->proceso = proc;
	tmp->siguiente = p->siguiente;
	p->siguiente = tmp;
}

int lista_vacia(lista l) {
	return (l->siguiente == NULL);
}

pnodo buscar(pid_t pid, lista listaProcesos) {
	pnodo p = listaProcesos->siguiente;

	while (p != NULL && p->proceso.pid!= pid)
		p = p->siguiente;
	return p;
}

int ultimo_elemento(pnodo p) {
	return (p->siguiente == NULL);
}

pnodo buscar_anterior(char *estado[], lista l) {
	pnodo p = l;
	while ((p->siguiente != NULL) && (strcmp(p->proceso.estado, estado[0]) == 0))
		p = p->siguiente;
	return p;
}

void eliminarProceso(char *estado[], lista l) {

	pnodo tmp, p = buscar_anterior(estado, l);
	if (ultimo_elemento(p)) {
		printf("no encontrado: %s\n", estado[0]);
	}
	tmp = p->siguiente;
	p->siguiente = tmp->siguiente;
	free(tmp);
}

void eliminarLista(lista l) {
	pnodo p,tmp;
	p= l->siguiente;
	while (p!=NULL){
		tmp = p->siguiente;
		p->siguiente = tmp -> siguiente;
		free(tmp);
	}
}



