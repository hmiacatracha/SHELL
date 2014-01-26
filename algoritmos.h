#ifndef ALG_H
#define ALG_H
#include "librerias.h"
#include "cmd.h"
#include "lista.h"
#include "listaMem.h"

/*******************************************************
 * 							I PARTE
 *******************************************************/
void autores();
char *pwd(void);
int contienePath(char *cadena[], int trozos);
void contiene(char **argv, int argc, int *s, int *r, int *a);
char TipoFichero (mode_t m);
char * ConvierteModo2 (mode_t m);
char *usuario (uid_t user);
char *grupo(gid_t group);
void imprimir(char *path, char *name, int s);
void procesarLs(char *path,int s, int r, int a);
void recursiveDelete(char *cadena);
int path_is_directory(char *path);


/*******************************************************
 * 							II PARTE
 *******************************************************/

void setPriority(char *cadena[]);
void getPidProcess(char *cadena[]);
void getShellPriority();
int getNumPriori(char *cadena[]);
char *lineaComando(char *cadena[], int trozos);
void actualizarEstadoProceso(int status, pnodo elemento);
void mostrarProcesos(char *cadena[], lista listaProcesos, int mostrarTodos);
void eliminarProcesos(char *cadena[], lista listaProcesos);
void mostrarProceso(pnodo elemento);
int obtenerPrioridadConArroba(char *cadena[]);
void mostrarProcesoPid(pid_t pid, lista listaProcesos);
void actualizarLista(lista listaProceso);

/***************************************************************
 *
 *          III-PARTE
 */
void GuardarEnLista(void *dir, int tam, char tipoAsig[], key_t descriptor,
		char descriptorFiche[], listaMem lista);
void *ObtenerMemoriaShmget(key_t clave, off_t tam, listaMem lista);
void listarDirecciones(char *tipo, listaMem lista, int mostrarTodos);
void cmdOpcMalloc(char *cadena[], int trozos, listaMem lista);
void Cmd_Mmap(char *fichero, char *tipomap, char * perm, listaMem lista);
void cmdOpcMmap(char *cadena[], int trozos, listaMem lista);
void cmdOpcShared(char *cadena[], int trozos, listaMem lista);
void opcDeassignMalloc(char *cadena[], int trozos, listaMem lista) ;
void opcDeassignShared(char *cadena[], int trozos, listaMem lista) ;
void opcDeassignMmap(char *cadena[], int trozos, listaMem lista);
void opcDeassignAddr(char *cadena[], listaMem lista);
void cmdOpcDeassing(char *cadena[], int trozos, listaMem lista);



#endif




 
