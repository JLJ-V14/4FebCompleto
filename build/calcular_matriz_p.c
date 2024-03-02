#include "definiciones_globales.h"
#include "osqp.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdbool.h>

/*Este tipo se utiliza para almacenar informacion relevante de la bateria el punto de inicio, el punto final
  y la variable booleana que sirve para identificar si la carga del vehiculo es */

int cargar_informacion_bateria(informacion_procesada_t* informacion_sistema, OSQPInt terminal_actual,
  int* numero_elementos_terminales, int** puntos_iniciales,
  int** puntos_finales,bool ** carga_rapida) {

  //Se van añadiendo los puntos iniciales y finales al array de puntos iniciales y finales de los
  //diferentes vehículos.
  for (int i = 0; i < informacion_sistema->informacion_vehiculos.numero_vehiculos; i++) {
    if (informacion_sistema->informacion_vehiculos.vehiculos[i].numero_terminal == terminal_actual) {
      (*numero_elementos_terminales)++;
      int* temp = realloc(puntos_iniciales, *numero_elementos_terminales * sizeof(int));
      int* temp2 = realloc(puntos_finales, *numero_elementos_terminales * sizeof(int));
      bool* temp3 = realloc(carga_rapida, *numero_elementos_terminales * sizeof(bool));

      if ((temp == NULL) || (temp2 == NULL) || (temp3 == NULL)) {
        printf("No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n");
        registrar_error("No se No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n", REGISTRO_ERRORES);
        return ERROR;
      }

      *puntos_iniciales = temp;
      (*puntos_iniciales)[*numero_elementos_terminales] = informacion_sistema->informacion_vehiculos.vehiculos[i].punto_inicio;
      *puntos_finales = temp2;
      (*puntos_finales)[*numero_elementos_terminales] = informacion_sistema->informacion_vehiculos.vehiculos[i].punto_final;
      *carga_rapida = temp3;
     
        if (informacion_sistema->informacion_vehiculos.vehiculos[i].modo_carga == "Rapida"){
          (*carga_rapida)[*numero_elementos_terminales] = true;
      }
        else {
          (*carga_rapida)[*numero_elementos_terminales] = false;
        }


    }
  }

  //Se van a añadir los puntos iniciales y finales de las baterias al array de puntos iniciales y final
  //correspondiente a las baterías
  for (int i = 0; i < informacion_sistema->informacion_baterias.numero_baterias; i++) {
    if (informacion_sistema->informacion_baterias.baterias[i].numero_terminal == terminal_actual) {
      (*numero_elementos_terminales)++;
      int* temp = realloc(puntos_iniciales, *numero_elementos_terminales * sizeof(int));
      int* temp2 = realloc(puntos_finales, *numero_elementos_terminales * sizeof(int));
      bool* temp3 = realloc(carga_rapida, *numero_elementos_terminales * sizeof(bool));

      if ((temp == NULL) || (temp2 == NULL) || (temp3 == NULL)) {
        printf("No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n");
        registrar_error("No se No se ha podido reservar memoria para el array de los puntos iniciales o finales al crear la matriz A_x\n", REGISTRO_ERRORES);
        return ERROR;
      }

      *puntos_iniciales = temp;
      (*puntos_iniciales)[*numero_elementos_terminales] = informacion_sistema->informacion_baterias.baterias[i].punto_inicio;
      *puntos_finales = temp2;
      (*puntos_finales)[*numero_elementos_terminales] = informacion_sistema->informacion_baterias.baterias[i].punto_final;
      (*carga_rapida) = temp3;
      (*carga_rapida)[*numero_elementos_terminales] = false;
    }
  }
  return EXITO;
}






int calcular_objetivos_cuadraticos_carga(informacion_procesada_t* informacion_sistema, matriz_p_t* matriz_p,
  OSQPInt numero_terminal,int* index_actual, int* dimension_vector_P_x,int* dimension_vector_P_p, int* dimension_vector_P_i) {

  //Se itera por todos los vehiculos que se tienen, se comprueba si el terminal al que están conectados coincide
  //con el termino que se esta anyadiendo

  int numero_vehiculos = informacion_sistema->informacion_vehiculos.numero_vehiculos;
  int numero_elementos_terminales = 0;
  int* puntos_iniciales_vehiculos = NULL;
  int* puntos_finales_vehiculos = NULL;
  bool* carga_rapida = NULL;

  if (cargar_informacion_bateria(informacion_sistema, numero_terminal, &numero_elementos_terminales,
    &puntos_iniciales_vehiculos, &puntos_finales_vehiculos, &carga_rapida) == ERROR) {
    printf("No se ha podido cargar en la matriz de objetivos cuadraticos la carga de las baterias, por problemas de reserva de memoria\n");
    registrar_error("No se ha podido cargar en la matriz de objetivos cuadraticos la carga de las baterias por problemas de reserva de memoria\n", REGISTRO_ERRORES);
    return ERROR;
  }
 
  return EXITO;
}



int calcular_objetivos_cuadraticos_carga(informacion_procesada_t* informacion_sistema, matriz_p_t* matriz_p) {

  //Se carga el numero de puntos de simulacion
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Se itera por cada uno de los terminales
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {


  }
}

/*Este subprograma se utiliza para calcular la matriz p del problema de optimización */

int calcular_matriz_p(informacion_procesada_t* informacion_sistema, matriz_p_t *matriz_p) {
 

  //Se reserva memoria para la matriz P
  matriz_p->P = malloc(sizeof(OSQPCscMatrix));

  if (matriz_p->P == NULL) {
    printf("No se ha podido reservar memoria para la matriz P\n");
    registrar_error("No se ha podido reservar memoria para la matriz P\n",REGISTRO_ERRORES);
    return ERROR;
  }
  
  return EXITO;
}