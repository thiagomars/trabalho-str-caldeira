//Definição de Bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "time.h" // utilizado para medição de tempo
#include "tela.h" //Inclusão do monitor que acessa/requisitar a tela do computador
//Inclusão do monitor que ler e disponibiliza valor dos dados para a threads da main 
#include "sensorfluxo.h"
#include "sensornivel.h"
#include "sensortemperatura.h"
#include "sensortemperaturaamb.h"
#include "sensortemperaturaentra.h"
#include "bufferDuplo.h"
//Inclusão do monitor que acessa o canal de comunicação do simulador
#include "socket.h"
#include "refTemperatura.h"//usar a referencia da temperatura
#include "refNivel.h"
//#include "bufduplo.h"
#include "atuador_aquecedor.h"
#include "atuador_fluxo_aquecida.h"
#include "atuador_fluxo_entrada.h"
#include "atuador_fluxo_saida.h"


//Definindo Variáveis necessárias para as medições de tempo
#define	NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um segundo
#define	N_AMOSTRAS	100		// Numero de amostras (medições) coletadas
long temp_exec[N_AMOSTRAS];		// Medicoes do tempo de execução da tarefa em microsegundos
long valor_nivel[N_AMOSTRAS];
long valor_temp[N_AMOSTRAS];


//Thread que exibe os valores do Nível e Temperaturas na tela
#define NSEC_PER_SEC (1000000000)
void thread_mostra_status (void){
	double temperatura, nivel, fluxo, temp_amb, temp_entrada;
	int i = 0;
	while(1){
		temperatura = sensor_get_temperatura();
		nivel = sensor_get_nivel();
		
		fluxo = sensor_get_fluxo();
		temp_amb = sensor_get_temperatura_ambiente();
		temp_ent = sensor_get_temperatura_entrada();
		
		aloca_tela();//Permite acesso exclusivo dos recursos para a tela do computador
		system("tput reset"); //limpa tela
		printf("---------------------------------------\n");
		printf("Temperatura (T)--> %.4lf\n", temperatura);
		printf("Nivel (H)--> %.4lf\n", nivel);
		
		printf("Fluxo (No)--> %.4lf\n", fluxo);
		printf("Temperatura Ambiente (Ta)--> %.4lf\n", temp_amb);
		printf("Temperatura Entrada (Ti)--> %.4lf\n", temp_ent);
		
		printf("---------------------------------------\n");
		libera_tela();//Libera os recursos 

		//Grava no arquivo a temperatura e o nivel
		valor_nivel[i] = nivel;
		valor_temp[i] = temperatura;

		sleep(1); //Executada a cada 1 segundo

		i++; // incrementa i
	}
		thread_grava_sensor_nivel();
		thread_grava_sensor_temperatura();
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
		printf("ALARME, TEMPERATURA LIMITE ATINGIDA !!!\n");
		libera_tela();
		sleep(1);
	
	}
		
}

/*
void thread_controle_temperatura (void){

	struct timespec t, t_fim;
	long periodo = 50e6;  // 50ms
	long temp_resp; // momento em que inicia até o fim

	// leitura da hora atual
  	clock_gettime(CLOCK_MONOTONIC, &t);

	while(1){	
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

		//-------------->Códigos para controle de Temperatura aqui<------------------

	// leitura da hora atual
    clock_gettime(CLOCK_MONOTONIC, &t_fim);

    // calcula o tempo de resposta observado
    temp_resp = 1000000 * (t_fim.tv_sec - t.tv_sec) + (t_fim.tv_nsec - t.tv_nsec) / 10000;

    bufduplo_insere_leitura(temp_resp);

    // calcula inicio do prox periodo
    t.tv_nsec += periodo;
    while (t.tv_nsec >= NSEC_PER_SEC) {
      t.tv_nsec -= NSEC_PER_SEC;
      t.tv_sec++;
    }	
	}
}

*/

void thread_grava_temp_resp(void){
    FILE* dados_f;
	dados_f = fopen("dados_tempo_exec.txt", "w");
	if(dados_f == NULL){
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}

	int amostras = 0;
	int tambuffer = tamBuf();

	while(amostras++ <= N_AMOSTRAS / tambuffer) {
    //Espera até o buffer encher para descarregar no arquivo
    long* buf = bufduplo_espera_buffer_cheio();

    int n2 = tamBuf();
    int tam = 0;

    while (tam < n2)
      fprintf(dados_f, "%4ld\n", buf[tam++]);

    fflush(dados_f);

    aloca_tela();
    printf("Gravando arquivo...\n");
    libera_tela();
  }

	fclose(dados_f);	
}

void thread_grava_sensor_nivel(void){
    FILE* dados_f;
	dados_f = fopen("dados_nivel.txt", "w");
	if(dados_f == NULL){
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}

	for( int i=0; i<N_AMOSTRAS; i++){
		fprintf(dados_f, "%4ld\n", valor_nivel[i]);
	}
	fclose(dados_f);	
}

void thread_grava_sensor_temperatura(void){
    FILE* dados_f;
	dados_f = fopen("dados_temperatura.txt", "w");
	if(dados_f == NULL){
		printf("Erro, nao foi possivel abrir o arquivo\n");
		exit(1);
	}

	for( int i=0; i<N_AMOSTRAS; i++){
		fprintf(dados_f, "%4ld\n", valor_temp[i]);
	}
	fclose(dados_f);	
}





/* Controlar nivel da caldeira */
void thread_controle_nivel(){
struct timespec t;
	long int periodo = 70e6; 	// 70ms
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

                // Realiza seu trabalho
                //variaveis de comporta de entrada
                double atu_q, atu_ni, atu_na, atu_nf;
                
                //verificar se o nivel está baixo -> Hmin = 0,1m e Hmax = 3,0m
                if(sensor_get_nivel() < HREF){
                	//Verificar a temperatura
                	if(TEMP_REF < sensor_get_temperatura()){ //temperatura alta
                		atu_q = 0;
                		atu_ni = 100;
                		atu_na = 0;
                		atu_nf = 0;
			} else if(TEMP_REF > sensor_get_temperatura()){ //temperatura baixa
				atu_q = 0;
                		atu_ni = 100;
                		atu_na = 10;
                		atu_nf = 100000;
			} else if(TEMP_REF == sensor_get_temperatura()){ //temperatura ideal
				atu_q = 100;
                		atu_ni = 100;
                		atu_na = 0;
                		atu_nf = 0;
			}
                }
                
                //verificar se o nivel está alto -> Hmin = 0,1m e Hmax = 3,0m
                if(sensor_get_nivel() > HREF){
                	//Verificar a temperatura
                	if(TEMP_REF < sensor_get_temperatura()){ //temperatura alta
                		atu_q = 0;
                		atu_ni = 0;
                		atu_na = 0;
                		atu_nf = 100;
			} else if(TEMP_REF > sensor_get_temperatura()){ //temperatura baixa
				atu_q = 100000;
                		atu_ni = 0;
                		atu_na = 10;
                		atu_nf = 100;
			} else if(TEMP_REF == sensor_get_temperatura()){ //temperatura ideal
				atu_q = 100;
                		atu_ni = 0;
                		atu_na = 0;
                		atu_nf = 100;
			}
                }
                
		
		// enviar os valores para os atuadores
		
		// atuador do aquecedor
		sprintf(msg_enviada, "aq-%lf", atu_q);
		msg_socket(msg_enviada);
                
		// atuador de entrada de agua ambiente
		sprintf(msg_enviada, "ani%lf", atu_ni);
		msg_socket(msg_enviada);
		
		// atuador de entrada de agua aquecida 80 graus
		sprintf(msg_enviada, "ana%lf", atu_na);
		msg_socket(msg_enviada);

		// atuador de saída de agua do esgoto controlado
		sprintf(msg_enviada, "anf%lf", atu_nf);
		msg_socket(msg_enviada);

		printf("Passou um periodo !\n");	

		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
	
}

void main( int argc, char *argv[]) {
	//estruturas e váriaveis para marcar o tempo no relógio
	struct timespec t, t_inicio, t_fim;
	int amostra = 0;		// Amostra corrente
	int periodo = 100000000; 	// 100ms

    //ref_putT(29.0);
    cria_socket(argv[1], atoi(argv[2]) ); 
	
	double temp, nivel;
	
	printf("Digite um valor REFERENCIA para a Temperatura: \n");
	scanf(" ");
	scanf("%lf", &temp);
	
	while(temp > 30.0){
		printf("Digite um valor de referência menor que 30 graus: \n");
		scanf(" ");
		scanf("%lf",&temp);
			
    	}
	
	printf("Digite um valor REFERENCIA para o Nivel de Agua: \n");
	scanf(" ");
	scanf("%lf", &nivel);
	
	put_refTemp(temp);
	put_refNivel(nivel);
	

	pthread_t t1, t2, t3, t4, t5, t6, t7, t8;
    //serão definidos 5 threads
    pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    //pthread_create(&t4, NULL, (void *) thread_controle_temperatura, NULL);
    pthread_create(&t5, NULL, (void *) thread_grava_temp_resp, NULL);
	pthread_create(&t6, NULL, (void *) thread_grava_sensor_nivel, NULL);
	pthread_create(&t7, NULL, (void *) thread_grava_sensor_temperatura, NULL);
	pthread_create(&t8, NULL, (void*) thread_controle_nivel, NULL);
    
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join( t3, NULL);
	//pthread_join( t4, NULL);
	pthread_join(t5, NULL);
	pthread_join(t6, NULL);
	pthread_join(t7, NULL);	    
	pthread_join(t8, NULL);
}
