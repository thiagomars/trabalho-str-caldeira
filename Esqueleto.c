//Definição de Bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tela.h" //Inclusão do monitor que acessa/requisitar a tela do computador 
#include "sensores.h" //Inclusão do monitor que ler e disponibiliza valor dos dados para a threads da main
#include "socket.h" //Inclusão do monitor que acessa o canal de comunicação do simulador
//#include "referenciaT.h"//usar a referencia da temperatura
//#include "bufduplo.h"

//Thread que exibe os valores do Nível e Temperaturas na tela
void thread_mostra_status (void){
	double temperatura, nivel;
	while(1){
		temperatura = sensor_get_temperatura()
		nivel = sensor_get_nivel;
		aloca_tela();//Permite acesso exclusivo dos recursos para a tela do computador
		system("tput reset"); //limpa tela
		printf("---------------------------------------\n");
		printf("Temperatura (T)--> %.2lf\n", temperatura);
		printf("Nivel (H)--> %.2lf\n", nivel);
		printf("---------------------------------------\n");
		libera_tela();//Libera os recursos 
		sleep(1); //Executada a cada 1 segundo
	}		
		
}

void thread_le_sensor (void){ //Le Sensores periodicamente a cada 10ms
	struct timespec t, t_fim;
	while(1){
			
	}		
}
/*
void thread_alarme (void){//aciona alamer
	
	while(1){
		
	}
		
}

void thread_controle_temperatura (void){
	while(1){	
       	
	}
}

void thread_grava_temp_resp(void){
    
    
    
}

*/

void main( int argc, char *argv[]) {
    //ref_putT(29.0);
    //cria_socket(argv[1], atoi(argv[2]) ); 
    
	pthread_t t1, t2, t3, t4, t5;
    //serão definidos 5 threads
    pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    //pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    //pthread_create(&t4, NULL, (void *) thread_controle_temperatura, NULL);
    //pthread_create(&t5, NULL, (void *) thread_grava_temp_resp, NULL);
    
	//pthread_join( t1, NULL);
	//pthread_join( t2, NULL);
	//pthread_join( t3, NULL);
	//pthread_join( t4, NULL);
	//pthread_join( t5, NULL);
	    
}
