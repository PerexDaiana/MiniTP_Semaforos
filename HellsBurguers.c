
#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

//creo estructura de semaforos
struct semaforos {
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_armarMedallon;
	sem_t sem_cocinarMedallon;
	sem_t sem_armarHamburguesa;
	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];

};

//creo los parametros de los hilos
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	FILE* procesoDeEquipos = fopen("tareasRealizadas.txt","a+");
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		fprintf(procesoDeEquipos,"\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		fprintf(procesoDeEquipos,"\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							fprintf(procesoDeEquipos,"\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);

				}
			}
		}
	}
}
void *ganador(void *data){
	FILE* procesoDeEquipos = fopen("tareasRealizadas.txt","a+");
	struct parametro *mydata = data;
	printf("\tEquipo %d Gano \n",mydata->equipo_param);
	fprintf(procesoDeEquipos,"\tEquipo %d Gano \n",mydata->equipo_param);
	exit(-1);
}
//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);

    pthread_exit(NULL);
}
void* mezclar(void *data) {
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);

	//creo el nombre de la accion de la funcion
	char *accion = "mezclar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_salar);

    pthread_exit(NULL);
}

void* salar(void *data) {
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_salar);
	pthread_mutex_lock(&salar);

	//creo el nombre de la accion de la funcion
	char *accion = "salar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	sem_post(&mydata->semaforos_param.sem_armarMedallon);
	pthread_mutex_unlock(&salar);

    pthread_exit(NULL);
}

void* armarMedallon(void *data) {
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_armarMedallon);

	//creo el nombre de la accion de la funcion
	char *accion = "armarMedallon";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 50000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_cocinarMedallon);

    pthread_exit(NULL);
}

void* cocinarMedallon(void *data) {
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_cocinarMedallon);
	pthread_mutex_lock(&cocinarMedallon);

	//creo el nombre de la accion de la funcion
	char *accion = "cocinarMedallon";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
	sem_post(&mydata->semaforos_param.sem_armarHamburguesa);
	pthread_mutex_unlock(&cocinarMedallon);

    pthread_exit(NULL);
}

void* cortarExtras(void *data) {

	//creo el nombre de la accion de la funcion
	char *accion = "cortarExtras";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_armarHamburguesa);

    pthread_exit(NULL);
}

void* hornear(void *data) {
	pthread_mutex_lock(&hornear);

	//creo el nombre de la accion de la funcion
	char *accion = "hornear";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 100000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_armarHamburguesa);
	pthread_mutex_unlock(&hornear);

    pthread_exit(NULL);
}

void* armarHamburguesa(void *data) {
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_armarHamburguesa);
	sem_wait(&mydata->semaforos_param.sem_armarHamburguesa);
	sem_wait(&mydata->semaforos_param.sem_armarHamburguesa);

	//creo el nombre de la accion de la funcion
	char *accion = "armarHamburguesa";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	//struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	ganador(mydata);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	printf("\tEquipo %d termino de armar la hamburguesa\n", mydata->equipo_param);
    pthread_exit(NULL);
}



void* ejecutarReceta(void *i) {

	//variables semaforos
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_armarMedallon;
	sem_t sem_cocinarMedallon;
	sem_t sem_armarHamburguesa;


	//crear variables semaforos aqui

	//variables hilos
	pthread_t p1;	//cortar
	pthread_t p2;	//mezclar
	pthread_t p3;	//salar
	pthread_t p4;	//armarMedallon
	pthread_t p5;	//cocinar
	pthread_t p6;	//hornear
	pthread_t p7;	//cortarExtras
	pthread_t p8;	//armarHamburguesa

	//crear variables hilos aqui

	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);

	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct

	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_mezclar = sem_salar;
	pthread_data->semaforos_param.sem_mezclar = sem_armarMedallon;
	pthread_data->semaforos_param.sem_mezclar = sem_cocinarMedallon;
	pthread_data->semaforos_param.sem_mezclar = sem_armarHamburguesa;


	//setear demas semaforos al struct aqui


	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?

	FILE *receta = fopen("instrucciones.txt","rb");
	if(receta == NULL) {
		printf("El archivo esta vacio");
	}
	char separador[] = ":,";
	char salto = '\n';
	char oracion[200];
	char *palabra;
	char caracter;
	int indPalab = 0;
	int acciones = 0;
	int ingredientes = 0;
	int posCaract = 0;

	while(feof(receta) == 0){
		caracter = fgetc(receta);		//tomo el primer caracter
		if(caracter != salto){
			oracion[posCaract] = caracter;
			posCaract++;			//tomo el siguiente caracter
		}
		else{
			palabra=strtok(oracion, separador);
			while(palabra != NULL){
				if(indPalab == 0){
					strcpy(pthread_data->pasos_param[acciones].accion, palabra);
					indPalab++;
				}
				else{
						strcpy(pthread_data->pasos_param[acciones].ingredientes[ingredientes], palabra);
				ingredientes++ ;
				}
			palabra=strtok(NULL, separador);
			}
			memset(oracion,0,200);
			indPalab = 0;
			posCaract = 0;
			acciones++;
			ingredientes = 0;
		}

	}

	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armarMedallon),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinarMedallon),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armarHamburguesa),0,0);

	//inicializar demas semaforos aqui


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

    rc = pthread_create(&p2,                           //identificador unico
                            NULL,                          //atributos del thread
                                mezclar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia


    rc = pthread_create(&p3,                           //identificador unico
                            NULL,                          //atributos del thread
                                salar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia


    rc = pthread_create(&p4,                           //identificador unico
                            NULL,                          //atributos del thread
                                armarMedallon,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia


    rc = pthread_create(&p5,                           //identificador unico
                            NULL,                          //atributos del thread
                                cocinarMedallon,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia


    rc = pthread_create(&p6,                           //identificador unico
                            NULL,                          //atributos del thread
                                hornear,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia


    rc = pthread_create(&p7,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortarExtras,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia


    rc = pthread_create(&p8,                           //identificador unico
                            NULL,                          //atributos del thread
                                armarHamburguesa,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia


	//crear demas hilos aqui


	//join de todos los hilos
	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);


	//crear join de demas hilos


	//valido que el hilo se alla creado bien
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }


	//destruccion de los semaforos
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_armarMedallon);
	sem_destroy(&sem_cocinarMedallon);
	sem_destroy(&sem_armarHamburguesa);

	//destruir demas semaforos

	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
	//creo los nombres de los equipos
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;

	//creo las variables los hilos de los equipos
	pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_mutex_t salero;
	pthread_mutex_t plancha;
	pthread_mutex_t horno;


	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1);

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);

    rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);
		pthread_mutex_init(&salero, NULL);
		pthread_mutex_init (&plancha, NULL);
		pthread_mutex_init (&horno,NULL);
   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);


    pthread_exit(NULL);
}


//Para compilar:   gcc HellsBurgers.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable

