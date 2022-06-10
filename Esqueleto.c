//Definição de Bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tela.h" //Inclusão do monitor que acessa/requisitar a tela do computador
//Inclusão do monitor que ler e disponibiliza valor dos dados para a threads da main 
#include "sensorfluxo.h"
#include "sensornivel.h"
#include "sensortemperatura.h"
#include "sensortemperaturaamb.h"
#include "sensortemperaturaentra.h"
//Inclusão do monitor que acessa o canal de comunicação do simulador
#include "socket.h"
//#include "referenciaT.h"//usar a referencia da temperatura
//#include "bufduplo.h"

//Thread que exibe os valores do Nível e Temperaturas na tela


#define NSEC_PER_SEC (1000000000)
void thread_mostra_status (void){
	double temperatura, nivel;
	while(1){
		temperatura = sensor_get_temperatura();
		nivel = sensor_get_nivel();
		aloca_tela();//Permite acesso exclusivo dos recursos para a tela do computador
		system("tput reset"); //limpa tela
		printf("---------------------------------------\n");
		printf("Temperatura (T)--> %.4lf\n", temperatura);
		printf("Nivel (H)--> %.4lf\n", nivel);
		printf("---------------------------------------\n");
		libera_tela();//Libera os recursos 
		sleep(1); //Executada a cada 1 segundo
	}		
		
}

void thread_le_sensor (void){ //Le Sensores periodicamente a cada 10ms
	char msg_enviada[1000];	
	struct timespec t, t_fim;
	long periodo = 10e6;

	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1){
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		sensor_put_temperatura(msg_socket("st-0"));
		sensor_put_nivel(msg_socket("sh-0"));
		sensor_put_temperatura_ambiente(msg_socket("sta0"));
		sensor_put_fluxo(msg_socket("sno0"));
		sensor_put_temperatura_entrada(msg_socket("sti0"));

		t.tv_nsec += periodo;
		while(t.tv_nsec>= NSEC_PER_SEC){
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_nsec ++;
		}
			
	}		
}

//aciona alarme na temperatura definida 
void thread_alarme (void){
	
	while(1){
		sensor_alarmeT(30); //Definindo a temperatura
		aloca_tela();
		printf("ALARME, TEMPERATURA LIMITE ATINGIDA !!!\N");
		libera_tela();
		sleep(1);
	
	}
		
}

/*
void thread_controle_temperatura (void){
	while(1){	
       	
	}
}

void thread_grava_temp_resp(void){
    
    
    
}

*/

/* Controlar nivel da caldeira */
void thread_controle_nivel(){
	
	
}

void main( int argc, char *argv[]) {
    //ref_putT(29.0);
    cria_socket(argv[1], atoi(argv[2]) ); 
    
	pthread_t t1, t2, t3, t4, t5;
    //serão definidos 5 threads
    pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    //pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    //pthread_create(&t4, NULL, (void *) thread_controle_temperatura, NULL);
    //pthread_create(&t5, NULL, (void *) thread_grava_temp_resp, NULL);
    
	pthread_join( t1, NULL);
	pthread_join( t2, NULL);
	//pthread_join( t3, NULL);
	//pthread_join( t4, NULL);
	//pthread_join( t5, NULL);
	    
}
