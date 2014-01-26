#ifndef LIBRERIAS_H
#define LIBRERIAS_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*getcwd, stat,unlik*/
#include <unistd.h>
/*struct dirent*/
#include <dirent.h>
/*struct stat*/
#include <sys/stat.h>
/*struct passwd*/
#include <pwd.h>
/*struct group*/
#include <grp.h>
#include <time.h>
/*getopt parsing arguments*/
#include<getopt.h>
/*parte II*/
//waitpid, pid
#include <sys/types.h>
#include <sys/resource.h>


/*PARTE 3*/
#include  <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>


#include <sys/wait.h>
#define MAX_LENGTH 2048
#define TAMANO 512
#define TRUE 1
#define FALSE 0
#define WHITE    "\x1b[37m"
#define BLUE   "\x1b[34m"
#define GREEN   "\x1b[32m"
#define ACT "act"
#define TERM "term"
#define SIG "sig"
#define STOP "stop"
#define ALL  "all"

#endif
