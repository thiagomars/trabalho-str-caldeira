/* Modulo do monitor responsável por ler e disponibilza os dados dos sensores p*/ 
#include <math.h>
#include <pthread.h>
#include <string.h>
#include "sensor.h"

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER; //Variável do tipo mutex e estática (acessível apenas a esse monitor), inicializada. Responsável pela exclusão mutua da seção crítica
static pthread_cond_t alarme = PTHREAD_COND_INITIALIZER; 
//Variáveis globas para amezanar os valores dos sensores, só são acessado por esse modulo do monitor sensor
static double s_temp = 0;
static double s_nivel = 0; 
static double s_fluxo = 0;
static double s_temp_entrada = 0; 
static double s_temp_ambiente = 0;
//static double limite_atual = HUGE_VAL;

/* Chamado pela thread que le o sensor e disponibiliza aqui o valor lido */
void sensor_put_temperatura( double temp) {
	//Seção crítica
	pthread_mutex_lock( &exclusao_mutua); //Exclusão mútua dos recursos
	s_temp = temp;//Aloca o valor de temperatura na variável global
	pthread_mutex_unlock( &exclusao_mutua);//Libera os recursos exclusos

}

/* Chamado por qualquer thread que precisa do valor lido do sensor */ 
double sensor_get_temperatura(void) {
	double aux; 
	pthread_mutex_lock(&exclusao_mutua); 
	aux = s_temp; //Armazena localmente o valor da variável global temperatura
	pthread_mutex_unlock(&exclusao_mutua); 
	return aux;//Retorna o valor da temperatura
}

void sensor_put_nivel( double niv) {
	//Seção crítica
	pthread_mutex_lock( &exclusao_mutua); //Exclusão mútua dos recursos
	s_nivel = niv;//Aloca o valor de nível na variável global
	pthread_mutex_unlock( &exclusao_mutua);//Libera os recursos exclusos
}

double sensor_get_nivel(void) {
	double aux; 
	pthread_mutex_lock(&exclusao_mutua); 
	aux = s_nivel; //Armazena localmente o valor da variável global nível
	pthread_mutex_unlock(&exclusao_mutua); 
	return aux; //Retorna o valor do nível
}

void sensor_put_fluxo( double flux) {
	//Seção crítica
	pthread_mutex_lock( &exclusao_mutua); //Exclusão mútua dos recursos
	s_fluxo = flux;//Aloca o valor de temperatura na variável global
	pthread_mutex_unlock( &exclusao_mutua);//Libera os recursos exclusos

}

/* Chamado por qualquer thread que precisa do valor lido do sensor */ 
double sensor_get_fluxo(void) {
	double aux; 
	pthread_mutex_lock(&exclusao_mutua); 
	aux = s_fluxo; //Armazena localmente o valor da variável global temperatura
	pthread_mutex_unlock(&exclusao_mutua); 
	return aux;//Retorna o valor da temperatura
}

/* Chamado pela thread que le o sensor e disponibiliza aqui o valor lido */
void sensor_put_temperatura_entrada( double temp_entr) {
	//Seção crítica
	pthread_mutex_lock( &exclusao_mutua); //Exclusão mútua dos recursos
	s_temp_entrada = temp_entr;//Aloca o valor de temperatura na variável global
	pthread_mutex_unlock( &exclusao_mutua);//Libera os recursos exclusos

}

/* Chamado por qualquer thread que precisa do valor lido do sensor */ 
double sensor_get_temperatura_entrada(void) {
	double aux; 
	pthread_mutex_lock(&exclusao_mutua); 
	aux = s_temp_entrada; //Armazena localmente o valor da variável global temperatura
	pthread_mutex_unlock(&exclusao_mutua); 
	return aux;//Retorna o valor da temperatura
}

/* Chamado pela thread que le o sensor e disponibiliza aqui o valor lido */
void sensor_put_temperatura_ambiente( double temp_amb) {
	//Seção crítica
	pthread_mutex_lock( &exclusao_mutua); //Exclusão mútua dos recursos
	s_temp_ambiente = temp_amb;//Aloca o valor de temperatura na variável global
	pthread_mutex_unlock( &exclusao_mutua);//Libera os recursos exclusos

}

/* Chamado por qualquer thread que precisa do valor lido do sensor */ 
double sensor_get_temperatura_ambiente(void) {
	double aux; 
	pthread_mutex_lock(&exclusao_mutua); 
	aux = s_temp_ambiente; //Armazena localmente o valor da variável global temperatura
	pthread_mutex_unlock(&exclusao_mutua); 
	return aux;//Retorna o valor da temperatura
}




/* Thread fica bloqueada até o valor do sensor chegar em limite */ 
void sensor_alarmeT( double limite) {
	pthread_mutex_lock( &exclusao_mutua); 
	limite_atual = limite; 
	while( s_temp < limite_atual){ 
		pthread_cond_wait( &alarme, &exclusao_mutua); 
	}
	limite_atual = HUGE_VAL; 
	pthread_mutex_unlock( &exclusao_mutua); 
}
