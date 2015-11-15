/* 
 * File:   adaemon.c
 * Author: akiel
 *
 * Created on 6 de octubre de 2013, 11:35
 */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <assert.h>



const char *pidfile = "/tmp/mydaemon.pid";
#define BUFFER_SIZE 512
void doStop(void);
void doStart(void);

int getPid(void);
void setRunnning(int);
void daemonize(void);
void process(int i);
void table();
void compile();
MYSQL *conn; /* variable de conexión para MySQL */
MYSQL *conn2;
MYSQL_RES *res; /* variable que contendra el resultado de la consuta */
MYSQL_ROW row; /* variable que contendra los campos por cada registro consultado */
const char *server = "localhost"; /*direccion del servidor 127.0.0.1, localhost o direccion ip */
const char *user = "root"; /*usuario para consultar la base de datos */
const char *password = "root"; /* contraseña para el usuario en cuestion */
const char *database = "b"; /*nombre de la base de datos a consultar */
int main(int argc, char** argv) {
		if (argc != 2) { //comprobar que me pasan solo un parametro
				printf("%s\n", "Parametros insuficientes. use: adaemon start|stop|restart");
				exit(EXIT_FAILURE);
		}

		if (strcmp(argv[1], "start") == 0) {
				doStart();
		} else if (strcmp(argv[1], "stop") == 0) {
				doStop();
		} else if (strcmp(argv[1], "restart") == 0) {
				doStop();
				doStart();
		} else {
				printf("%s\n", "Parametros incorrectos. use: adaemon start|stop|restart");
				exit(EXIT_FAILURE);
		}
		return 0;
}
void doStart() {
		/*
		 * Inicia el demonio
		 */
		int pid = getPid();
		if (pid == 0) { //si no obtengo ningun pid entonces puedo iniciar el demonio
				printf("%s\n", "Iniciando adaemon, revise el LOG, correra en segundo plano...");
				daemonize();
			 // freopen("/tmp/log.txt","w",stdout);
				/* The Big Loop */
				int i=0;
				
				while (1) {
						table();
						sleep(3); /*esperar 10 segundos*/
				}
				exit(EXIT_SUCCESS);
		} else {
				/*si encuentro otro pid entonces existe una ejecucion previa de mi demonio
				 y solo quiero correrlo una vez*/
				printf("El proceso ya esta corriendo. PID: %i\n", pid);
				exit(EXIT_SUCCESS);
		}
}
void compile(){

		//int pid = fork();
	 // printf("PID FORK %d\n",pid );
	 // if (pid == 0) {
		freopen("/tmp/ce.txt", "w", stderr);
		const char * CP_G[] = {"g++", "/tmp/Main.cpp", "-o", "/tmp/compile", "-fno-asm", "-Wall", "-lm", "--static", "-std=c++0x", "-DONLINE_JUDGE", NULL };
		 execvp(CP_G[0], (char * const *) CP_G);

		 /* FILE *fp = fopen("ce.txt", "r");
			char ceinfo[(1 << 16)], *cend;
			cend = ceinfo;
			while (fgets(cend, 1024, fp)) {
					 cend += strlen(cend);    
			}
			if (cend>0)
			{
			}
			*/
			fclose(stderr);
			exit(0);
		//} else {
		//    int status = 0;
		//    waitpid(pid, &status, 0);   
		//}

}

void delete_and_update_table(){
		char sql[BUFFER_SIZE];
		//revizar esta parte
		sprintf(sql,"update solucion set resp='Error de compilacion'");
		printf("%s\n",sql );
		if (mysql_real_query(conn, sql, strlen(sql))) {
				printf("..update failed! %s\n",mysql_error(conn));
		}
}
void table(){
		conn = mysql_init(NULL); /*inicializacion a nula la conexión */
		if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
		{ /* definir los parámetros de la conexión antes establecidos */
				fprintf(stderr, "%s\n", mysql_error(conn)); /* si hay un error definir cual fue dicho error */
				exit(1);
		}

		if (mysql_query(conn, "select * from run"))
		{ /* definicion de la consulta y el origen de la conexion */
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
		}
		res = mysql_use_result(conn);

		freopen("/tmp/log.txt","w",stdout);

		while ((row = mysql_fetch_row(res)) != NULL){

					 /* FILE * fp;
						fp=fopen("/home/brayan/laravel/demon/Main.cpp","w+");
						fprintf(fp, "%s\n", row[2]);
						fclose(fp);
						compile();

						sleep(1);*/
					//sprintf(sql,"UPDATE `solution` SET  FROM `solution` WHERE `user_id`=\'%s\' AND `result`=\'4\') WHERE `user_id`=\'%s\'",user_id, user_id);
					 // delete_and_update_table(row[0]);
						//char cadena[1000];
						//strcpy(cadena,);
						//strcat(cadena,row[0]);



						conn2 = mysql_init(NULL);

						if (!mysql_real_connect(conn2, server, user, password, database, 0, NULL, 0)){ 
								fprintf(stderr, "%s\n", mysql_error(conn2)); 
								exit(1);
						}

						char sql[(1 << 16)]; 
						snprintf(sql, (1 << 16) - 1, "DELETE FROM run WHERE id_run=%d",atoi(row[0]));
						mysql_real_query(conn2, sql, (unsigned int)strlen(sql));
						
						printf("%s\n",mysql_error(conn2) );
						printf("La consulta es %s\n",sql);
						printf("El codigo Fuente es : \n %s\n",row[2] );
						printf("Actualizando %d\n",atoi(row[0]));

						mysql_free_result(res);
						mysql_close(conn);
		} 
		fclose(stdout);

		
	 // delete_and_update_table();

		//mysql_query(conn, "update solucion set resp='Error XD121' where id_sol=2");


	/*  if (mysql_query(conn, "delete from run"))
		{ 
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
		}*/

		mysql_free_result(res);
		mysql_close(conn);
}


void process(int i){
						char cadena[1000];
						strcpy(cadena,"archivo");
						char nro[1000];
						sprintf(nro,"%d",i);
						strcat(cadena,nro);
						char men[100];
						strcpy(men,"touch /tmp/demon/");
						strcat(men,cadena);          
						system(men);
}
void doStop() {
		/*
		 * Para el demonio
		 */
		printf("%s\n", "Parando adaemon");
		int pid = getPid();
		if (pid != 0) { //si encuentro un pid le mando a ese proceso un SIGTERM y elimino el archivo de PID
				kill(pid, SIGTERM);
				remove(pidfile);
		} else {
				printf("%s\n", "El demonio no se encuentra en ejecucion");
				//exit(EXIT_FAILURE);
		}
}

int getPid() {
		/*
		 * Permite saber si el demonio esta ejecutandose y que PID tiene el proceso
		 */
		FILE *pfile = fopen(pidfile, "r");

		if (!pfile) {
				return 0;
		} else {
				int pid;
				fscanf(pfile, "%i", &pid);
				fclose(pfile);
				return pid;
		}
}

void setRunnning(int pid) {
		/*
		 * Escribe el archivo de PID
		 */
		FILE *pfile = fopen(pidfile, "w");
		fprintf(pfile, "%i", pid);
		fclose(pfile);
}

void daemonize() {
		/*
		 * Convierte nuestro programa en un proceso en 2do plano
		 */
		pid_t pid, sid; // nuestro process ID y Session ID 

		pid = fork(); //le hacemos fork a nuestro proceso
		if (pid < 0) {
				exit(EXIT_FAILURE);
		}

		if (pid > 0) { //si obtenemos pid salimos del proceso padre
				exit(EXIT_SUCCESS);
		}

		umask(0); //cambiamos a umask 0

		/*
		 * Conviene en cualquier caso abrir archivos de log por aqui
		 */

		sid = setsid(); //creamos un SID para el nuevo proceso

		if (sid < 0) {
				exit(EXIT_FAILURE);
		}

		setRunnning(sid); //escribimos el archivo de PID

		if ((chdir("/tmp/")) < 0) { //cambiamos de directorio para evitar errores
				exit(EXIT_FAILURE);
		}

		/* Cerramos las entradas y salidas estandar*/
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

}