/*
 * cmd.h
 *
 *  Created on: 13/12/2013
 *      Author: mabel
 */

#ifndef CMD_H_
#define CMD_H_

#include "lista.h"
#include "librerias.h"
#include "algoritmos.h"
#include "listaMem.h"

/*PARTE I */
void cmdExitSucess(lista l);
void cmdAutores(void);
void cmdGetPid(char *cadena[], int trozos);
void cmdCdir(char *cadena[], int trozos);
void cmdList(char *cadena[], int trozos);
void cmdDeleteFile(char *archivo);
void cmdRdir(char *cadena[], int trozos);

/*PARTE II */
void cmdPriority(char *cadena[], int trozos);
void cmdFork(char *cadena[], int trozos);

void cmdExec(char *cadena[], int prioridadSinArroba);
void cmdSplano(char *cadena[], int trozos, int prioridadSinArroba,lista listaProcesos);
void cmdPplano(char *cadena[], int trozos,int prioridadSinArroba);

void cmdExecPrio(char *cadena[], int trozos);
void cmdSplaPrio(char *cadena[], int trozos,lista listaProcesos);
void cmdPplaPrio(char *cadena[], int trozos);
void cmdPplaPrio(char *cadena[], int trozos);

void cmdJobs(char *cadena[],int trozos, lista listaProcesos);
void cmdDelJobs(char *cadena[],int trozos, lista listaProcesos);
void cmdFgPid(char *cadena[], int trozos);
void cmdPplano2(char *cadena[], int trozos);


/*PARTE III*/
void cmdMem(char *cadena[], int trozos, listaMem lista);
void Cmd_rmkey(char * key);
void cmdDirecciones();
void cmdMemdup(char *cadena[], int trozos, listaMem lista) ;
void recursiva(int n);
void cmdUid(listaMem lista);
#endif /* CMD_H_ */
