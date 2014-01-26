#include "listaMem.h"
/*
 * CREA LA LISTA DE ASIGNACION DE MEMORIA
 */
void crear_listaMem(listaMem *lista) {
	pnodoMem tmp = (pnodoMem) malloc(sizeof(struct nodoMem));
	if (tmp == NULL) {
		printf("memoria agotada\n");
	}
	tmp->siguiente = NULL;
	*lista = tmp;
}
int lista_vaciaMem(listaMem lista) {
	return (lista->siguiente == NULL);
}

int ultimo_elementoMem(pnodoMem p) {
	return (p->siguiente == NULL);
}

void insertarListaMem(tMemoria memoria, pnodoMem p) {
	pnodoMem tmp = (pnodoMem) malloc(sizeof(struct nodoMem));
	if (tmp == NULL) {
		printf("memoria agotada, al insertar elementos a la lista\n");
	}
	tmp->memoria = memoria;
	tmp->siguiente = p->siguiente;
	p->siguiente = tmp;
}
/*
 * BUSCAR POR MALLOC Y SHARED
 */
pnodoMem buscarMemTam(int tam, char *tipo, listaMem lista) {
	pnodoMem p = lista->siguiente;
	while (p != NULL && p->memoria.tam != tam && p->memoria.tipoAsig != tipo) {
		p = p->siguiente;
	}
	if (p->memoria.tam == tam && p->memoria.tipoAsig == tipo)
		return p;
	else {
		printf("Tamano no encontrado\n");
		return NULL;
	}
}
/*
 *PARA BUSCAR MMAP
 */
pnodoMem buscarMemFichero(char *fichero, listaMem lista) {
	pnodoMem p = lista->siguiente;
	while (p != NULL && strcmp(p->memoria.descriptorFich, fichero)) {
		p->siguiente;
	}
	if (!strcmp(p->memoria.descriptorFich, fichero)) {
		return p->siguiente;
		printf("Fichero no encontrado\n");
		return NULL;
	}
}
/*
 * BUSCAR POR DIRECCION
 */
pnodoMem buscarMemDireccion(void *dir, listaMem lista) {
	pnodoMem p = lista->siguiente;
	while (p != NULL && p->memoria.dir != dir) {
		p->siguiente;
	}
	if (p->memoria.dir == dir)
		return p->siguiente;
	printf("direccion no valida\n");
	return NULL;
}

pnodoMem buscarAnteriorMemFich(char *fich, listaMem lista) {
	pnodoMem p = lista;
	while ((p->siguiente != NULL) && (strcmp(p->memoria.descriptorFich == fich)))
		p = p->siguiente;
	return p;
}

pnodoMem buscarAnteriorMemTam(int tam, char *tipo, listaMem lista) {
	pnodoMem p = lista;
	while ((p->siguiente != NULL) && (p->memoria.tam != tam)
			&& (strcmp(p->memoria.tipoAsig, tipo)))
		p = p->siguiente;
	return p;
}

pnodoMem buscarAnteriorDir(void *dir, listaMem lista) {
	pnodoMem p = lista;
	while ((p->siguiente != NULL) && (p->memoria.dir != dir))
		p = p->siguiente;
	return p;
}
/*
 * ELIMINAR PARA MALLOC
 * ELIMINAR PARA SHARED
 */
void eliminarMemTam(int tam, char *tipo, listaMem lista) {
	pnodoMem tmp, p = buscarAnteriorMemTam(tam, tipo, lista);
	if (!ultimo_elementoMem(p)) {
		tmp = p->siguiente;
		p->siguiente = tmp->siguiente;
		free(tmp);
	}
}
/*
 * ELIMINAR PARA MMAP
 */
void eliminarMemFichero(char *fich, listaMem lista) {
	pnodoMem tmp, p = buscarAnteriorMemFich(fich, lista);
	if (!ultimo_elementoMem(p)) {
		tmp = p->siguiente;
		p->siguiente = tmp->siguiente;
		free(tmp);
	}
}
/*
 * ELIMINAR POR DIRECCION
 */
void eliminarMemDireccion(void *dir, listaMem lista) {
	pnodoMem tmp, p = buscarAnteriorDir(dir, lista);
	if (!ultimo_elementoMem(p)) {
		tmp = p->siguiente;
		p->siguiente = tmp->siguiente;
		free(tmp);
	}
}
/*
 * ELIMINA LA LISTA DE ASIGNACIONES DE MEMORIA
 */

void eliminarListaMem(listaMem listaMem) {
	pnodoMem p, tmp;
	p = listaMem->siguiente;
	while (p != NULL) {
		tmp = p->siguiente;
		p->siguiente = tmp->siguiente;
		free(tmp);
	}
}
