#include "cmd.h"
#include "lista.h"
#include "listaMem.h"
#define OPCMALLOC "-malloc"
#define OPCMMAP "-mmap"
#define OPCSHARED "-shared"
#define OPCDEASSING "-deassign"

int variableGlobal1 = 0;
char * variableGlobal2 = "Variable Global";
double variableGlobal3 = 6.5;

/******************************************************************************
 *								   COMANDOS									 *
 ******************************************************************************/

/*
 *	EXIST_SUCESS es una macro se expande a una expresión integral dependiente
 *	del sistema que, cuando se usa como argumento de la función termina,
 *	significa que la aplicación se ha realizado correctamente.
 *	la utilizamos cuando hacemos exit,quit,fin de nuestra shell
 */

/*void cmdExitSucess(lista l) {
 eliminarLista(l);
 exit(0);
 }*/

void cmdAutores(void) {
	printf("%8s %s\n", " ", "HEIDY MABEL IZAGUIRRE");
}

/*
 *	getpid determina el ID de proceso invocador.
 *	getppid determina el ID del proceso padre.
 */

void cmdGetPid(char *cadena[], int trozos) {
	if (trozos == 1) {
		printf("My process ID : %d\n", getpid());
	} else if (!strcmp(cadena[1], "-p")) {
		printf("My parent's ID: %d\n", getppid());
	}
}

/*
 *	CDIR FUNCION QUE NOS CAMBIA DE DIRECTORIO,
 *	 si CDIR si no se especifica una ruta
 *    se muestra el directorio de trabajo.
 *   chdir()  función que cambia el directorio de trabajo.
 *
 */

void cmdCdir(char *cadena[], int trozos) {
	if (trozos >= 2) {
		int ret = chdir(cadena[1]);
		if (ret != 0) {
			perror("chdir");
			return;
		}
	} else if (trozos == 1) {
		char *path = pwd();
		printf("%s\n", path);
		free(path);
	}
}

/*
 *  PROCEDIMIENTO LIST es el procedimiento
 *  principal del comando list
 */

void cmdList(char *cadena[], int trozos) {
	int s = 0;
	int r = 0;
	int a = 0;
	char *path = NULL;
	contiene(cadena, trozos, &s, &r, &a);
	int posicion = contienePath(cadena, trozos);
	if (posicion) {
		path = cadena[posicion];
	} else {
		path = pwd();
	}
	procesarLs(path, s, r, a);
	if (!posicion) {
		free(path);
	}
}

/*
 * Eliminamos el archivo unlink elimina un archivo
 * tambien se puede hacer con remove
 */

void cmdDeleteFile(char *archivo) {

	if (unlink(archivo) == -1) {
		perror("delete file");
		return;
	}
}

/*
 * procedimiento para el comando rdir
 * con rdir -f eliminamos carpetas no vacias
 */
void cmdRdir(char *cadena[], int trozos) {
	if (trozos == 0) {
		printf("missing arguments\n");
	} else if (trozos >= 1) {
		if (trozos == 1) {
			int deleteDirectory = rmdir(cadena[1]);
			if (deleteDirectory == 0) {
				return;
			} else {
				perror("rdir");
				return;
			}
		} else if (!(strcmp(cadena[1], "-f"))) {
			recursiveDelete(cadena[2]);
		}
	}
}

/***********************************PARTE II *********************************/

void cmdPriority(char *cadena[], int trozos) {

	switch (trozos) {
	case 0:
		getShellPriority();
		break;
	case 1:
		getPidProcess(cadena);
		break;
	case 2:
		setPriority(cadena);
		break;
	default: //si tiene mas argumentos
		setPriority(cadena);
	}

}
void cmdFork(char *cadena[], int trozos) {
	pid_t pid = 0;
	pid = fork();

	switch (pid) {
	case -1:
		perror("cmdFork"); // When fork() returns -1, an error happened
		break;
	case 0: // When fork() returns 0, we are in the child process
		break;
	default: //When fork() returns a positive number, we are in the parent process
		if (waitpid(pid, NULL, 0) == -1) {
			perror("waitpid error");
		}
		break;
	}
}

void cmdExec(char *cadena[], int prioridadSinArroba) {
	int prio = 0;

	if (cadena[0][0] == '@' || (prioridadSinArroba)) {
		if (!prioridadSinArroba)
			prio = obtenerPrioridadConArroba(cadena);
		else
			prio = atoi(cadena[0]);
		if (setpriority(PRIO_PROCESS, 0, prio) == -1) {
			perror("Error en WithPriority(setpriority):");
			return;
		}
		cadena++;
	}
	execvp(cadena[0], &cadena[0]);
	printf("El prog especificado no existe\n");
}

void cmdSplano(char *cadena[], int trozos, int prioridadSinArroba,
		lista listaProcesos) {
	nodoProc proceso;
	pid_t pid = 0;
	time_t fechaHora = time(NULL);
	int prioridad = 0;

	switch (pid = fork()) {
	case -1:
		perror("cdmSplano");
		break;
	case 0: //// Se crea el proceso, devuelve 0 al hijo y el pid del hijo al padre
		cmdExec(cadena, prioridadSinArroba);
		exit(255);
		break;
	default: //el padre
		proceso.pid = pid;
		if (cadena[0][0] == '@' || (prioridadSinArroba)) {
			if (prioridadSinArroba)
				prioridad = atoi(cadena[0]);
			else
				prioridad = obtenerPrioridadConArroba(cadena);
		}
		proceso.prioridad = prioridad;
		strcpy(proceso.comando, lineaComando(cadena, trozos));
		strftime(proceso.fechaInicio, 20, "%d-%m-%Y %H:%M:%S",
				localtime(&fechaHora));
		strcpy(proceso.estado, ACT);
		proceso.valorSenal = 0;
		insertarLista(proceso, listaProcesos);
		break;
	}
}
void cmdPplano(char *cadena[], int trozos, int prioridadSinArroba) {

	pid_t pidHijo = 0;
	switch (pidHijo = fork()) {
	case -1:
		perror("cdmPplano");
		break;
	case 0:
		cmdExec(cadena, prioridadSinArroba);
		exit(255);
		break;
	default:
		if ((waitpid(pidHijo, NULL, 0)) == -1) {
			perror("waitpid Pplano");
		}
	}
}
void cmdExecPrio(char *cadena[], int trozos) {
	int priority = atoi(cadena[1]); //da cero si no puede convertir a numeros la palabra cadena[1]

	if ((trozos >= 3) && priority) { //como minimo 3 argumentos pri prog arg
		strncat(cadena[1], "@", 0);
		printf(" %s", cadena[1]);
		cadena++;
		cmdExec(cadena, 1);
		return;
	}
	printf("formato debe ser: execpri prio prog arg1 ...\n");

}
void cmdSplaPrio(char *cadena[], int trozos, lista listaProcesos) {
	int priority = atoi(cadena[1]); //da cero si no es numero.
	if (trozos >= 3 && (priority)) {
		cadena++;
		cmdSplano(cadena, trozos, 1, listaProcesos);
		return;
	}
	printf("formato debe ser: splanopri prio prog arg1 ...\n");
}

void cmdPplaPrio(char *cadena[], int trozos) {
	int priority = atoi(cadena[1]); //da cero si no es numero.
	if (trozos >= 3 && (priority)) {
		cadena++;
		cmdPplano(cadena, trozos, 1);
		return;
	}
	printf("formato debe ser: pplanopri prio prog arg1 ...\n");
}

void cmdJobs(char *cadena[], int trozos, lista listaProcesos) {
	int pid = 0;
	switch (trozos) {
	case 0:
		mostrarProcesos(cadena, listaProcesos, 1);
		break;
	case 1:
		pid = atoi(cadena[1]);
		if (pid != 0) {
			mostrarProcesoPid(pid, listaProcesos);
			return;
		}
		cadena++;
		mostrarProcesos(cadena, listaProcesos, 0);
		return;
	}

}

void cmdDelJobs(char *cadena[], int trozos, lista listaProcesos) {
	switch (trozos) {
	case 0:
		mostrarProcesos(cadena, listaProcesos, 1);
		break;
	case 1:
		cadena++;
		if (strcmp(cadena[0], ALL) == 0)
			eliminarLista(listaProcesos);
		eliminarProcesos(cadena, listaProcesos);
		break;
	}

}
void cmdFgPid(char *cadena[], int trozos) {
	int pid = 0;
	int estado = 0;
	if (trozos >= 1) {
		pid = atoi(cadena[1]);
		if (pid != 0) {
			if ((waitpid(pid, &estado, 0) == -1)) {
				perror("fg");
			}
		}
		printf("formato debe ser: fg pid");
	}
	printf("formato debe ser: fg pid");

}

/*****************************************************************************
 *                     PARTE III
 *
 *****************************************************************************
 */

void cmdMem(char *cadena[], int trozos, listaMem lista) {
	if (trozos == 0) {
		listarDirecciones("", lista, 1);
		return;
	}
	if (!strcmp(cadena[0], OPCDEASSING)) {
		cmdOpcDeassing(cadena++, trozos--, lista);
		return;
	}
	if (!strcmp(cadena[0], OPCMALLOC)) {
		cmdOpcMalloc(cadena++, trozos--, lista);
		return;
	}
	if (!strcmp(cadena[0], OPCMMAP)) {
		cmdOpcMmap(cadena++, trozos--, lista);
		return;
	}
	if (!strcmp(cadena[0], OPCSHARED)) {
		cmdOpcShared(cadena++, trozos--, lista);
		return;
	}
	printf("%s : not find the commands\n", cadena[0]);
}

void Cmd_rmkey(char * key) {
	key_t clave;
	int id;
	if (key == NULL || (clave = (key_t) strtoul(key, NULL, 10)) == IPC_PRIVATE) {
		printf(" rmkey clave_valida\n");
		return;
	}
	if ((id = shmget(clave, 0, 0666)) == -1) {
		perror("shmget: imposible obtener memoria compartida");
		return;
	}
	if (shmctl(id, IPC_RMID, NULL) == -1)
		perror("shmctl: imposible eliminar memoria compartida\n");
}

void cmdDirecciones() {
	int variableLocal1 = 5;
	char *variableLocal2 = "Variable Local";
	double variableLocal3 = 6.5;
	printf("VARIABLES GLOBALES/n");
	printf("Direccion de la primera variable G: %p", &variableGlobal1);
	printf("Direccion de la segunda variable G: %p", &variableGlobal2);
	printf("Direccion de la tercera variable G: %p", &variableGlobal3);
	printf("Direccion de la primera variable L: %p", &variableLocal1);
	printf("Direccion de la segunda variable L: %p", &variableLocal2);
	printf("Direccion de la tercera variable L: %p", &variableLocal3);
	printf("Direccion de la primera funcion %p", &cmdAutores);
	printf("Direccion de la segunda funcion %p", &cmdGetPid);
	printf("Direccion de la tercera funcion %p", &cmdCdir);

}

void cmdMemdup(char *cadena[], int trozos,listaMem lista) {

}
void recursiva(int n) {
	char automatico[TAMANO];
	static char estatico[TAMANO];
	printf("parametro n:%d en %p\n", n, &n);
	printf("array estatico en:%p \n", estatico);
	printf("array automatico en %p\n", automatico);
	if (n > 0)
		recursiva(n - 1);
}

void cmdUid(listaMem lista) {

}
