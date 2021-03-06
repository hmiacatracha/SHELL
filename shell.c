#include "librerias.h"
#include "algoritmos.h"
#include "cmd.h"


int trocearCadena(char *cadena, char *trozos[]) {
	int i = 1;
	if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
		return 0;
	while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
		i++;
	return i;
}

int opcion(char *cadena[], int trozos) {
	if (trozos >= 1) {
		int i = 0;
		int numComandos = 26;
		char *comando[26] = { "fin", "exit", "quit", "autores", "getpid",
				"cdir", "list", "delete", "rdir", "priority", "fork", "exec",
				"splano", "pplano", "execpri", "splanopri", "pplanopri", "jobs",
				"deljobs", "fg", "mem", "rmkey", "direcciones", "memdup",
				"recursiva", "uid" };
		for (i = 0; i < numComandos; i++) {
			if (strcmp(cadena[0], comando[i]) == 0) {
				return i;;
			}
		}
	}
	return (-1);
}

int inputType(char *cadena[], int trozos, lista listaProcesos, listaMem listaMemoria) {
	int opc = opcion(cadena, trozos);
	int tam = atoi(cadena[1]);
	switch (opc) {
	case 0:
		return 0;
	case 1:
		return 0;
	case 2:
		return 0;
	case 3:
		cmdAutores();
		return 1;
	case 4:
		cmdGetPid(cadena, trozos);
		return 1;
	case 5:
		cmdCdir(cadena, trozos);
		return 1;
	case 6:
		cmdList(cadena, trozos);
		return 1;
	case 7:
		cmdDeleteFile(cadena[1]);
		return 1;
	case 8:
		cmdRdir(cadena, trozos - 1);
		return 1;
	case 9:
		cmdPriority(cadena, trozos - 1);
		return 1;
	case 10:
		cmdFork(cadena, trozos - 1);
		return 1;
	case 11:
		cadena++;
		cmdExec(cadena, 0);
		return 1;
	case 12:
		cadena++;
		cmdSplano(cadena, trozos - 1, 0, listaProcesos);
		return 1;
	case 13:
		cadena++;
		cmdPplano(cadena, trozos - 1, 0);
		return 1;
	case 14:
		cmdExecPrio(cadena, trozos - 1);
		return 1;
	case 15:
		cmdSplaPrio(cadena, trozos - 1, listaProcesos);
		return 1;
	case 16:
		cmdPplaPrio(cadena, trozos - 1);
		return 1;
	case 17:
		cmdJobs(cadena, trozos - 1, listaProcesos);
		return 1;
	case 18:
		cmdDelJobs(cadena, trozos - 1, listaProcesos);
		return 1;
	case 19:
		cmdFgPid(cadena, trozos - 1);
		return 1;
	case 20:
		cadena++;
		cmdMem(cadena, trozos - 1,listaMemoria);
		return 1;
	case 21:
		Cmd_rmkey(cadena[1]);
		return 1;
	case 22:
		cmdDirecciones();
		return 1;
	case 23:
		cadena++;
		cmdMemdup(cadena, trozos,listaMemoria);
		return 1;
	case 24:
		recursiva(tam);
		return 1;
	case 25:
		cmdUid(listaMemoria); return 1;
	default:
		cmdPplano(cadena, trozos, 0);
		//printf("%s : not find the commands\n", cadena[0]);
		return 1;
	}
}

/**
 * GETENV, Busca una "lista de entorno", proporcionado por el entorno local,
 * para una cadena que empareje la cadena apuntada por nombre
 */
/*VERIFICAMOS QUE COMANDO HACE REFERENCIA EL USUARIO
 *
 */

int main(int argc, char * arvg[]) {
	char **c = (char **) malloc(sizeof(char*) * 11);
	c[10] = NULL; //para que la ultima posicion se un null fijo.
	char arguments[MAX_LENGTH];
	int condicion = 1, argumentos = 0;
	lista listaProcesos;
	listaMem listaMemoria;
	crear_listaMem(&listaMemoria);
	crear_lista(&listaProcesos);

	while (condicion) {
		printf("%s@myShell:~$", getenv("LOGNAME"));
		fgets(&arguments[0], MAX_LENGTH, stdin);
		argumentos = trocearCadena(arguments, c);
		condicion = inputType(c, argumentos, listaProcesos, listaMemoria);
	}
	printf("\n");
	eliminarLista(listaProcesos);
	eliminarListaMem(listaMemoria);
	free(c);
	exit(EXIT_SUCCESS);
}
