#include "algoritmos.h"
#include "listaMem.h"

/*
 *   FUNCION QUE NOS DEVUELVE EL DIRECTORIO ACTUAL DE TRABAJO.
 *	El getcwd () función coloca una ruta absoluta del
 *   directorio de trabajo actual. char *getcwd(char *buf, size_t size);
 *    
 *    Si buf es un puntero nulo, getcwd () obtiene el tamaño de bytes de
 *      espacio utilizando malloc (3C). El puntero devuelto por getcwd () puede
 *      ser utilizado como el argumento en una llamada posterior a liberar ().
 */
char *pwd(void) {
	char *cwd = "";
	if ((cwd = getcwd(NULL, PATH_MAX)) == NULL) {
		perror("pwd");
		return ""; //return
	}
	return cwd;
}

/*  FUNCION QUE UTILIZAMOS EN LA FUNCION
 *	LIST para verificar el path
 *	devolvemos el indice donde se encuentra lo que consideramos el path.
 */

int contienePath(char *cadena[], int trozos) {
	int i = 0;
	for (i = 1; i < trozos; i++) {
		if (cadena[i][0] != '-') {
			return i;
		}
	}
	return FALSE;
}

/* FUNCION QUE NOS DEVUELVE EL TIPO DE FICHERO 
 *  LA UTILIZAMOS PARA LA FUNCION LIST
 */

char TipoFichero(mode_t m) {
	switch (m & S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
	case S_IFSOCK:
		return 's'; /*socket */
	case S_IFLNK:
		return 'l'; /*symbolic link*/
	case S_IFREG:
		return '-'; /* fichero normal*/
	case S_IFBLK:
		return 'b'; /*block device*/
	case S_IFDIR:
		return 'd'; /*directorio */
	case S_IFCHR:
		return 'c'; /*char device*/
	case S_IFIFO:
		return 'p'; /*pipe*/
	default:
		return '?'; /*desconocido, no deberia aparecer*/
	}
}

/*  
 *  NOS DEVUELVE UN CHAR CON LOS PERMISOS
 *	DE LOS ARCHIVOS O DIRECTORIOS
 */

char * ConvierteModo2(mode_t m) {
	static char permisos[12];
	strcpy(permisos, "---------- ");
	permisos[0] = TipoFichero(m);
	if (m & S_IRUSR)
		permisos[1] = 'r'; /*propietario*/
	if (m & S_IWUSR)
		permisos[2] = 'w';
	if (m & S_IXUSR)
		permisos[3] = 'x';
	if (m & S_IRGRP)
		permisos[4] = 'r'; /*grupo*/
	if (m & S_IWGRP)
		permisos[5] = 'w';
	if (m & S_IXGRP)
		permisos[6] = 'x';
	if (m & S_IROTH)
		permisos[7] = 'r'; /*resto*/
	if (m & S_IWOTH)
		permisos[8] = 'w';
	if (m & S_IXOTH)
		permisos[9] = 'x';
	if (m & S_ISUID)
		permisos[3] = 's'; /*setuid, setgid y stickybit*/
	if (m & S_ISGID)
		permisos[6] = 's';
	if (m & S_ISVTX)
		permisos[9] = 't';
	return (permisos);
}
/*

 *  NOS DEVUELVE EL USUARIO
 *  man getpwuid nos muestra como esta formada la estructura
 *  de passwd
 *    uid_t   pw_uid;    user ID
 */

char *usuario(uid_t user) {
	struct passwd * us;
	if ((us = getpwuid(user)) == NULL)
		return "desconocido";
	return (us->pw_name);
}

/*
 *  NOS DEVUELVE EL GRUPO
 *  man getpwuid nos muestra la estructura de passwd
 *	 id_t   pw_gid;       group ID
 */

char *grupo(gid_t group) {
	struct passwd * us;
	if ((us = getpwuid(group)) == NULL)
		return "desconocido";
	return (us->pw_name);
}

/* PROCEDIMIENTO QUE IMPRIME EL LISTADO CORTO Y LARGO 
 *  utiliza struct stat retorna informacion sobre los archivos
 *  man 2 stat
 */

void imprimir(char *path, char *name, int s) {
	char *permisos = "", *time = "", *user = "", *group = "";
	struct stat inodo;
	char newpath[512];
	sprintf(newpath, "%s/%s", path, name);
	if (s) {
		printf(WHITE"%s ", name);
		printf("\n");
		return;
	}
	if (lstat(newpath, &inodo)) {
		return;
	}
	permisos = ConvierteModo2(inodo.st_mode);
	user = usuario(inodo.st_uid);
	group = grupo(inodo.st_gid);
	time = strtok(ctime(&inodo.st_mtime), "\n");
	printf(WHITE "%lu %s %2ld %s %s %5lld %12s %s\n", inodo.st_ino, permisos,
			(long) inodo.st_nlink, user, group, (long long) inodo.st_size, time,
			name);
}

/*
 *  procesarLs usamos aqui vemos si list es r (recursiv0) o a (all)
 *  para luego mandar imprimir
 *  man readdir
 *  devuelve un puntero a una estructura dirent representa la
 *  próxima entrada de directorio en la secuencia de directorio apuntado por
 *  dirp. Devuelve NULL al llegar al final de la secuencia de directorio o
 *  si se ha producido un error.
 */
void procesarLs(char *path, int s, int r, int a) {
	DIR *dirp = opendir(path);
	struct dirent *direntp;
	char newDir[512];
	if (dirp == NULL) {
		perror("list");
		return;
	}
	while ((direntp = readdir(dirp)) != NULL) {
		if ((direntp->d_name[0] == '.') && !a)
			continue;
		sprintf(newDir, "%s/%s", path, direntp->d_name);
		if (path_is_directory(newDir) && r
				&& !(!strcmp(direntp->d_name, ".")
						|| !strcmp(direntp->d_name, ".."))) {
			printf(BLUE"%s\r\n", path);
			printf(WHITE"");
			;
			procesarLs(newDir, s, r, a);
		} else
			imprimir(path, direntp->d_name, s);
	}
	closedir(dirp);

}

/*PROCEDIMIENTO CONTIENE
 * verificamos que el comando list [opciones] tiene opciones
 * y de que tipo de opciones s(short) r (recursive) a (all)
 * utilizamos getopt:
 * getopt se utiliza para romper opciones de líneas de comandos
 * para facilitar el análisis de los procedimientos de shell
 * y para comprobar si hay opciones legales. utiliza
 * los getopt (3) rutinas de GNU para hacer esto.
 * optind=0 cuando se ejecuta opt varias veces
 * opterr=0 una opción inválida no mande el error de getopt()
 */

void contiene(char **argv, int argc, int *s, int *r, int *a) {
	int opt = 0;
	extern int opterr, optind;
	extern char *optarg;
	opterr = 0, optind = 0; //Esto es obligatorio, sino no nos ecuentra todas las opciones le basta con encontrar una.
	while ((opt = getopt(argc, argv, "sra")) != EOF) { //EOF -1
		switch (opt) {
		case 's':
			*s = 1;
			break;
		case 'a':
			*a = 1;
			break;
		case 'r':
			*r = 1;
			break;
		case '?':
			break;
		}
	}
}

/*
 * aqui eliminamos recursivamente
 * utilizamos este procedimiento para rmdir
 * para ir vaciando las carpetas y luego eliminarlas.
 */

void recursiveDelete(char *cadena) {
	DIR *dirp = opendir(cadena);
	struct dirent *myFile; //aqui se va a guardar la informacion de de readdir
	char p_buf[512] = { 0 };
	if (dirp == NULL) {
		perror("delete directory");
		return;
	}
	while ((myFile = readdir(dirp)) != NULL) {
		sprintf(p_buf, "%s/%s", cadena, myFile->d_name); //Concatena la direccion path + el nombre del directorio
		if (!strcmp(myFile->d_name, ".") || !strcmp(myFile->d_name, ".."))
			continue; //En caso que sea el directorio actual o el directorio anterior pues continua.
		if (path_is_directory(p_buf))
			recursiveDelete(p_buf);   //en caso que si sea un directorio hacemos una eliminacion recursiva.
		else
			cmdDeleteFile(p_buf); //Sino elimina el archivo.
	}
	closedir(dirp);
	rmdir(cadena);

}

/*funcion que nos devuelve TRUE si es una carpeta y FALSE en caso que no.
 *stat() nos da información de un fichero sin abrirlo,
 * pero si es un link simbólico lo sigue hasta el fichero real.
 * Con esta función no podemos obtener información de links simbólicos.
 */

int path_is_directory(char *path) {
	struct stat inodo;
	if (stat(path, &inodo) == 0) {
		if (inodo.st_mode & S_IFDIR)
			return TRUE;
		else
			return FALSE;
	} else {
		perror("path_is_directory");
		return FALSE;
	}
}

/**************************************************************************************************
 * 										PARTE II	                                              *
 **************************************************************************************************/
/*
 * Coge la prioridad que le mandan ya sea con @prioridad o con prioridad
 */
void setPriority(char *cadena[]) {
	int pid = 0, priority = 0;
	pid = atoi(cadena[1]);
	priority = atoi(cadena[2]);

	if ((pid != 0) && (priority != 0)) {
		if (setpriority(PRIO_PROCESS, pid, priority) == -1) {
			perror("setPriority");
		}
	}
}
/**
 * Puedes enviar la prioridad con @prioridad o solo prioridad cuando la envias
 * con el @ esta funcion te devuelve la prioridad sin arroba.
 */
int obtenerPrioridadConArroba(char *cadena[]) {
	int prioridad = 0;
	if (strchr(cadena[0], '@') != NULL) {
		prioridad = atoi(strtok(cadena[0], "@"));
	}
	return prioridad;
}
/*
 * Te devuelve la prioridad que tiene segun el pid.
 */
void getPidProcess(char *cadena[]) {
	int pid = 0, priority = 0;
	pid = atoi(cadena[1]);

	if (pid != 0) {
		if (((priority = getpriority(PRIO_PROCESS, pid)) == -1)) {
			perror("getPidProcess");
			return;
		}
		printf("El prioridad del %s es:%d", cadena[1], priority);
	} else
		perror("getPidProcess");
}
/*
 * procediento que sino se le especifica a obtener prioridad un pid pues entonces
 * te manda la del hijo pid=0
 */
void getShellPriority() {
	int priority = 0;
	if ((priority = getpriority(PRIO_PROCESS, 0)) == -1) {
		perror("getShellPriority");
		return;
	}
	printf("La prioridad del shell es:%d \n", priority);

}
/*
 * devuelve la cadena de texto que has mandando desde la linea de comando en caso
 * que le envias alguna prioridad con arroba pues ignora la @ y asi hasta que termine todos los trozos
 */

char *lineaComando(char *cadena[], int trozos) {
	static char comando[200];
	comando[0] = '\0';
	int i = 0;

	for (i = 0; i < trozos; i++) {
		if (cadena[i][0] == '@')
			continue;
		strcat(comando, cadena[i]);
		strcat(comando, " ");

	}
	return comando;
}
/*
 * Para los procesos de segundo plano, necesitamos actualizar los procesos
 * para ver si continuan, si estan parados, etc.
 */
void actualizarEstadoProceso(int status, pnodo elemento) {

	if (WIFEXITED(status)) {
		strcpy(elemento->proceso.estado, TERM);
		elemento->proceso.valorSenal = WEXITSTATUS(status);
	}
	if (WIFSIGNALED(status)) {
		strcpy(elemento->proceso.estado, SIG);
		elemento->proceso.valorSenal = WTERMSIG(status);
	}

	if (WIFSTOPPED(status)) {
		strcpy(elemento->proceso.estado, STOP);
		elemento->proceso.valorSenal = WSTOPSIG(status);
	}

	if (WIFCONTINUED(status)) {
		strcpy(elemento->proceso.estado, ACT);
		elemento->proceso.valorSenal = 0;
	}
}
/*
 * Muestra todos los procesos en segundo plano. En waitpid mira si hay hijo
 * que haya terminado no espera por ellos sino que pone como opciones
 * WNOHANG no espera a que el hijo termine, WUNTRACED se informa si el hijo para,
 * WCONTINUED se informa si el hijo parado continua
 * solo se informa para mostrar el estado del proceso.
 */
void mostrarProceso(pnodo elemento) {
	int estado = 0;
	int prioridad = 0;

	if ((strcmp(elemento->proceso.estado, TERM) != 0)
			&& (strcmp(elemento->proceso.estado, SIG) != 0)) {

		if (waitpid(elemento->proceso.pid, &estado,
		WUNTRACED | WCONTINUED | WNOHANG) > 0)
			actualizarEstadoProceso(estado, elemento);
		prioridad = getpriority(PRIO_PROCESS, elemento->proceso.pid);
	}

	printf(" %5d %3d %16s %10s %7d  %s\n", (int) elemento->proceso.pid,
			prioridad, elemento->proceso.fechaInicio, elemento->proceso.estado,
			elemento->proceso.valorSenal, elemento->proceso.comando);
}
/*
 * Muestra el proceso haciendo filtro en el pid que se envia, estos
 * procesos estan en segundo plano.
 */

void mostrarProcesoPid(pid_t pid, lista listaProcesos) {
	int estado = 0;
	int prioridad = 0;
	pnodo elemento = buscar(pid, listaProcesos);
	if (elemento == NULL) {
		if ((strcmp(elemento->proceso.estado, TERM) != 0)
				&& (strcmp(elemento->proceso.estado, SIG) != 0)) {
			if (waitpid(elemento->proceso.pid, &estado,
			WUNTRACED | WCONTINUED | WNOHANG) > 0)
				actualizarEstadoProceso(estado, elemento);
			prioridad = getpriority(PRIO_PROCESS, elemento->proceso.pid);
		}

		printf(" %5s %7s %8s %10s %5s  comando\n", " PID", " PRIO", "Fecha",
				"  Estado", "    señal");
		printf(" %5d %3d %16s %10s %7d  %s\n", (int) elemento->proceso.pid,
				prioridad, elemento->proceso.fechaInicio,
				elemento->proceso.estado, elemento->proceso.valorSenal,
				elemento->proceso.comando);
	} else
		printf("pid inexistente");

}
/*
 * Muestra todos los procesos que estan en segundo plano
 */
void mostrarProcesos(char *cadena[], lista listaProcesos, int mostrarTodos) {
	int imprimir = 0;
	pnodo elemento = listaProcesos->siguiente;
	while (elemento != NULL) {
		if ((strcmp(elemento->proceso.estado, cadena[0]) == 0)
				|| (mostrarTodos)) {
			if (!imprimir) {
				imprimir = 1;
				printf(" %5s %4s %20s %10s %7s  comando\n", "PID", "PRIO",
						"Fecha", "Estado", "señal");
			}
			mostrarProceso(elemento);
		}
		elemento = elemento->siguiente;
	}
}
/*
 * Elimina TODA la lista de procesos de segundo plano.
 * esto se hace cuando haces eliminar ALL
 */
void eliminarProcesos(char *cadena[], lista listaProcesos) {
	pnodo elemento = listaProcesos->siguiente;

	while (elemento != NULL) {
		if ((strcmp(elemento->proceso.estado, cadena[0]) == 0)) {
			eliminarProceso(cadena, listaProcesos);
		}
		elemento = elemento->siguiente;
	}
}

/*****************************************************************************
 *                     PARTE III
 *
 *****************************************************************************
 */
void GuardarEnLista(void *dir, int tam, char tipoAsig[], key_t descriptor,
		char descriptorFiche[], listaMem lista) {
	tMemoria memoria;
	memoria.dir = dir;
	memoria.tam = tam;
	time_t fechaHora = time(NULL);
	strftime(memoria.fechaAsig, 20, "%d-%m-%Y %H:%M:%S", localtime(&fechaHora));
	strcpy(memoria.tipoAsig, tipoAsig);
	memoria.descriptor = (int) descriptor;
	strcpy(memoria.descriptorFich, descriptorFiche);
	insertarListaMem(memoria, lista);

}

void *ObtenerMemoriaShmget(key_t clave, off_t tam, listaMem lista) {
	void * p;
	int aux, id, flags = 0777;
	int tamano = 0;
	struct shmid_ds s;

	if (tam)
		flags = flags | IPC_CREAT | IPC_EXCL;
	if (clave == IPC_PRIVATE) /*no nos vale*/
	{
		errno = EINVAL;
		return NULL;
	}
	if ((id = shmget(clave, tam, flags)) == -1)
		return (NULL);
	if ((p = shmat(id, NULL, 0)) == (void*) -1) {
		aux = errno;
		if (tam)
			shmctl(id, IPC_RMID, NULL);
		errno = aux;
		return (NULL);
	}
	shmctl(id, IPC_STAT, &s);
	tamano = (int) s.shm_segsz;
	GuardarEnLista(p, tamano, SHARED, clave, NULL, lista);
	return (p);
}

/*
 * imprimir lista por tipo de asignacion o mostrando todos
 */
void listarDirecciones(char *tipo, listaMem lista, int mostrarTodos) {
	int imprimir = 0;
	pnodoMem elemento = lista->siguiente;
	tMemoria memoria;
	while (elemento != NULL) {
		if ((strcmp(elemento->memoria.tipoAsig, tipo) == 0) || mostrarTodos) {
			if (!imprimir) {
				imprimir = 1;
				printf("%20s  %10s %20s %20s %10s %20s \n", "Direccion",
						"Tamano", "Fecha", "Tipo de asignacion", "Descriptor",
						"Descriptor del fichero");
			}
			printf("%15x", *(unsigned int*) memoria.dir);
			printf("%10d", memoria.tam);
			printf("%20s", memoria.fechaAsig);
			printf("%20s", memoria.tipoAsig);
			printf("%10d", memoria.descriptor);
			printf("%30s", memoria.descriptorFich);
		}
		elemento = elemento->siguiente;
	}
}


void cmdOpcMalloc(char *cadena[], int trozos, listaMem lista) {
	int tam = 0;
	void *p;

	if (trozos == 0) {
		listarDirecciones(MALLOC, lista, 0);
	}
	if (trozos > 0) {
		tam = atoi(cadena[0]);
		if (tam == 0) {
			printf("El tamano especificado no es valido, intentelo de nuevo\n");
			return;
		}
		if ((p = malloc(tam)) == NULL) {
			printf("Error al reservar memoria (malloc)\n");
			return;
		}
		GuardarEnLista(p, tam, MALLOC, (-1), NULL, lista);
	}

}
void Cmd_Mmap(char *fichero, char *tipomap, char * perm, listaMem lista) {
	void *p;
	struct stat s;
	int df, protection = 0, map = MAP_PRIVATE, modo = O_RDONLY, tam = 0;
	if (fichero == NULL) {
		listarDirecciones(MMAP, lista, 0);
		return;
	}
	if (tipomap != NULL) {
		if (!strcmp(tipomap, "-s"))
			map = MAP_SHARED;
		if (perm != NULL && strlen(perm) < 4) {
			if (strchr(perm, 'r') != NULL)
				protection |= PROT_READ;
			if (strchr(perm, 'w') != NULL)
				protection |= PROT_WRITE;
			if (strchr(perm, 'x') != NULL)
				protection |= PROT_EXEC;
		}
	}
	if (protection & PROT_WRITE)
		modo = O_RDWR;
	if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1) {
		perror("imposible acceder al fichero");
		return;
	}
	if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED) {
		perror("error mmap");
		close(df);
		return;
	}
	tam = (int) s.st_size;
	GuardarEnLista(p, tam, MMAP, NULL, fichero, lista);
	close(df);
	printf("fichero %s mapeado en %p\n", fichero, p);
}
void cmdOpcMmap(char *cadena[], int trozos, listaMem lista) {
	if (trozos == 0) {
		Cmd_Mmap(NULL, NULL, NULL, lista);
	}
	if (trozos == 1) {
		Cmd_Mmap(cadena[0], NULL, NULL, lista);
	}
	if (trozos == 2) {
		if (!strcmp(cadena[1], "-s"))
			Cmd_Mmap(cadena[0], cadena[1], NULL, lista);
		else
			Cmd_Mmap(cadena[0], NULL, cadena[1], lista);
	}
	if (trozos >= 3) {
		Cmd_Mmap(cadena[0], cadena[1], cadena[2], lista);
	}
}

void cmdOpcShared(char *cadena[], int trozos, listaMem lista) {
	int tam = 0;
	key_t key;

	if (trozos == 0)
		listarDirecciones(SHARED, lista, 0);
	else {

		key = (key_t) strtoul(cadena[0], NULL, 10); //strtoul convert a string to an integer, atoi is other function that do the same thing.
		if (trozos == 2) {
			tam = strtoul(cadena[1], NULL, 10);
		}
		if (ObtenerMemoriaShmget(key, tam, lista) == NULL) {
			perror("Asignar memoria Shared");
		}

	}
}


/*
 *  Desasigna (con free) en el shell el primer bloque de tamano tam aisgnado mediante el comando
 mem -malloc y lo elimina de la lista. Si no se especifica tamano
 nos dara una lista de las direcciones de memoria asignadas con el
 comando malloc

 */
void opcDeassignMalloc(char *cadena[], int trozos, listaMem lista) {
	pnodoMem p;
	int tam = 0;
	if (trozos ==0) {
		listarDirecciones(MALLOC, lista, 0);
		return;
	}
	if (trozos > 1) {
		tam = atoi(cadena[1]);
		p = buscarMemTam(tam, MALLOC, lista);
		if (p != NULL) {
			free(p);
			eliminarMemTam(tam, MALLOC, lista);
		}
	}
}
/*
 * Desmapea el fichero  mapeado en memoria y elimina dicha direccion de la lista de direcciones.
 *  Si no se especifica fichero nos informa de las direcciones de memoria
 *  donde hay mapeados ficheros, indicandonos la direccion, el tamano
 *  del mapeo, el fichero que hay mapeado en ella, el instante en que
 *  se mapeo y el descriptor de fichero.
 */

void opcDeassignShared(char *cadena[], int trozos, listaMem lista) {
	pnodoMem p;
	int tam = 0;
	tam = atoi(cadena[1]);
	if (trozos == 0) {
		listarDirecciones(SHARED, lista, 0);
		return;
	}
	if (trozos > 1) {
		if ((p = buscarMemTam(tam, SHARED, lista)) != NULL) {
			if (shmdt(p->memoria.dir) == 1) {
				perror("deassign shared: ");
				return;
			}
			eliminarMemTam(tam, SHARED, lista); //Eliminar ese tam de la lista con el tipo SHARED
		}
	}
}
/*
 * Desasigna (con shmdt) en el shell la zona de memoria compartida con
 * tam y la elimina de la lista de direcciones de memoria compartida.
 * Si no se especifica tam nos informa de las direcciones de memoria,
 * indicándonos la direccion, el tam del mapeo en el instante en que se mapeo.
 */
void opcDeassignMmap(char *cadena[], int trozos, listaMem lista) {
	pnodoMem p;
	if (trozos == 0) {
		listarDirecciones(MMAP, lista, 0);
		return;
	}
	if (trozos > 1) {
		if ((p = buscarMemFichero(cadena[1], lista)) != NULL) {
			if ((munmap(p->memoria.dir, p->memoria.tam)) == -1) {
				perror("munmap");
				return;
			}
			eliminarMemFichero(cadena[1], lista); //eliminar del fichero de la lista
		}
	}
}
/*
 * desasigna la direccion de addr dependiendo el tipo de asignacion,desasigna
 * con free munmap, etc. es decir opcDeassignMap
 */
void opcDeassignAddr(char *cadena[], listaMem lista) {
	pnodoMem p;
	void *dir = (void *) cadena[0];
	p = buscarMemDireccion(dir, lista);
	if (p != NULL) {
		if (!strcmp(p->memoria.tipoAsig, MALLOC)) {
			free(p);
			eliminarMemDireccion(dir, lista);
		}
		if (!strcmp(p->memoria.tipoAsig, SHARED)) {
			if (shmdt(p->memoria.dir) == -1) {
				perror("deassign shared: ");
				return;
			}
			eliminarMemDireccion(dir, lista);
		}
		if (!strcmp(p->memoria.tipoAsig, MMAP)) {
			if ((munmap(p->memoria.dir, p->memoria.tam)) == MAP_FAILED) {
				perror("munmap");
				return;
			}
			eliminarMemDireccion(dir, lista);
		}
	}
}

/*
 * Desasigna memoria con MALLOC, SHARED, MMAP o por direccion
 */

void cmdOpcDeassing(char *cadena[], int trozos, listaMem lista) {

	if (!strcmp(cadena[0], MALLOC)) {
		opcDeassignMalloc(cadena++, trozos--, lista);
		return;
	}
	if (!strcmp(cadena[0], SHARED)) {
		opcDeassignShared(cadena++, trozos--, lista);
		return;
	}
	if (!strcmp(cadena[0], MMAP)) {
		opcDeassignMmap(cadena++, trozos--, lista);
		return;
	}
	opcDeassignAddr(cadena,lista);
}
