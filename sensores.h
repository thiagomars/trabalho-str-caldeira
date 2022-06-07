/*Cabeçalho do modulo monitor dos sensores*/
#ifndef SENSORES_H
#define SENSORES_H

void sensor_put_temperatura( double temp);
double sensor_get_temperatura(void);
void sensor_put_nivel( double niv);
double sensor_get_nivel(void);
void sensor_put_fluxo( double flux);
double sensor_get_fluxo(void);
void sensor_put_temperatura_entrada( double temp_entr);
double sensor_get_temperatura_entrada(void);
void sensor_put_temperatura_ambiente( double temp_amb);
double sensor_get_temperatura_ambiente(void);
//void sensor_alarmeT(double limite);

#endif



