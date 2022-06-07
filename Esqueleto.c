//Definição de Bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tela.h" //para usar o aloca tela é necessário definir a biblioteca 
#include "sensores.h" //Ler e disponibilza valor dos dados
//#include "socket.h" //acessar o canal de comunicação do simulador
//#include "referenciaT.h"//usar a referencia da temperatura
//#include "bufduplo.h"

/*
void thread_mostra_status (void){
	while(1){
		
	}		
		
}

void thread_le_sensor (void){ //Le Sensores periodicamente a cada 10ms

	while(1){
			
	}		
}

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
    //pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    //pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    //pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    //pthread_create(&t4, NULL, (void *) thread_controle_temperatura, NULL);
    //pthread_create(&t5, NULL, (void *) thread_grava_temp_resp, NULL);
    
	//pthread_join( t1, NULL);
	//pthread_join( t2, NULL);
	//pthread_join( t3, NULL);
	//pthread_join( t4, NULL);
	//pthread_join( t5, NULL);
	    
}
