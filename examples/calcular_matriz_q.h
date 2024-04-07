
#ifndef CALCULAR_MATRIZ_Q_H
#define CALCULAR_MATRIZ_Q_H

#ifdef __cplusplus
extern "C" {
#endif
	int calcular_vector_q(informacion_procesada_t* informacion_sistema, OSQPFloat** q,
		informacion_carga_terminales_t* elementos_programados_terminales);


#ifdef __cplusplus
}
#endif

#endif
