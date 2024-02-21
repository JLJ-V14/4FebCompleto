
/*En este archivo se encuentran los subprogramas reservados
  a liberar la memoria reservada a almacenar la informacion
  leida de los ficheros csv de entrada*/
#include <stdio.h>
#include <stdlib.h>
#include "osqp.h"
#include "tipos_optimizacion.h"

  /*
  Este subprograma se utiliza para liberar la memoria
  reservada a almacenar la informacion de un CSV.
  */
void liberar_memoria_csv_individual(datos_csv_t* datos_csv) {

    if (datos_csv == NULL || datos_csv->datos == NULL) {
        return; //Si el dato de entrada que es un puntero es NULL se sale de la funcion.
    }
    int numero_filas = (datos_csv)->filas;
    int numero_columnas = (datos_csv)->columnas;

    for (int i = 0; i < numero_filas; i++) {
      if (datos_csv->datos[i] != NULL) {
        for (int j = 0; j < numero_columnas; j++) {
          if (datos_csv->datos[i][j] != NULL) {
            free(datos_csv->datos[i][j]);
            datos_csv->datos[i][j] = NULL;
          }
          
        }
       free((datos_csv)->datos[i]);
        datos_csv->datos[i] = NULL;
      }
    }
   
        free(datos_csv->datos);
       datos_csv->datos = NULL;

}

/*
Este subprograma se utiliza para liberar espacio en memoria para
los datos que se leen de los CSVs
*/

void liberar_memoria_csvs(informacion_entrada_t* informacion_sistema) {
    datos_csv_t* datos_vehiculos = &(informacion_sistema->datos_vehiculos.informacion_vehiculos);
    datos_csv_t* datos_algoritmo = &(informacion_sistema->datos_algoritmo.informacion_algoritmo);
    datos_csv_t* datos_baterias = &(informacion_sistema->datos_baterias.informacion_baterias);
    datos_csv_t* datos_precio_compra = &(informacion_sistema->datos_precio_compra.informacion_precio);
    datos_csv_t* datos_precio_venta = &(informacion_sistema->datos_precio_venta.informacion_precio);
    datos_csv_t* datos_terminales = &(informacion_sistema->datos_terminales.informacion_terminales);
    datos_csv_t* datos_restricciones = &(informacion_sistema->datos_restricciones.informacion_restricciones);


    datos_csv_t* datos_csvs[] = {
    datos_vehiculos, datos_algoritmo, datos_baterias,
    datos_precio_compra, datos_precio_venta,
    datos_restricciones, datos_terminales
    };

    int numero_csvs = sizeof(datos_csvs) / sizeof(datos_csvs[0]);

    for (int i = 0; i < numero_csvs; i++) {
        if (datos_csvs[i] != NULL) {
            liberar_memoria_csv_individual(datos_csvs[i]);
            (datos_csvs[i]) = NULL;

        }
    }

}

void liberar_memoria_vehiculo(vehiculos_t* vehiculo) {
 // free(&(vehiculo->fecha_inicio));
  //free(&(vehiculo->fecha_final));
}

void liberar_memoria_bateria(bateria_t* bateria) {
  //free(bateria->fecha_inicio);
  //free(bateria->fecha_final);
  //free(bateria->fecha_objetivo);
}

/*Este subprograma se utiliza para liberar la memoria reservada para almacenar la informacion procesada*/
void liberar_memoria_informacion_procesada(informacion_procesada_t* informacion_procesada) {

  if(informacion_procesada!=NULL){
  if (informacion_procesada->informacion_vehiculos.vehiculos != NULL) {
    /*
    for (int i = 0; i < informacion_procesada->informacion_vehiculos.numero_vehiculos; i++) {
      if (&(informacion_procesada->informacion_vehiculos.vehiculos[i]) != NULL) {
        liberar_memoria_vehiculo(&informacion_procesada->informacion_vehiculos.vehiculos[i]);
      }

      
      // informacion_procesada->informacion_vehiculos.vehiculos = NULL;
    }
    */
    free(informacion_procesada->informacion_vehiculos.vehiculos);
  }
    //Se libera las baterias
  if (informacion_procesada->informacion_baterias.baterias != NULL) {
    /*
    for (int i = 0; i < informacion_procesada->informacion_baterias.numero_baterias; i++) {
      if (&(informacion_procesada->informacion_baterias.baterias[i]) != NULL) {
        liberar_memoria_bateria(&informacion_procesada->informacion_baterias.baterias[i]);
      }
      
      //informacion_procesada->informacion_baterias.baterias = NULL;
    }
    */
    free(informacion_procesada->informacion_baterias.baterias);
  }
    
    // Se libera la informacion de los precios
    if (informacion_procesada->informacion_precio_compra.precios != NULL) {
      free(informacion_procesada->informacion_precio_compra.precios);
      informacion_procesada->informacion_precio_compra.precios = NULL;
    }
    if (informacion_procesada->informacion_precio_venta.precios != NULL) {
      free(informacion_procesada->informacion_precio_venta.precios);
      informacion_procesada->informacion_precio_venta.precios = NULL;
    }
  }
}


void liberar_memoria_problema_optimizacion(problema_optimizacion_t* problema_optimizacion) {
  /*En este subprograma se liberan las posiciones en memoria reservadas a las matrices para la resolucion del
  problema*/


  //Se libera los contenidos de la matriz P
  if (problema_optimizacion->matriz_p.P)free(problema_optimizacion->matriz_p.P);
  if(problema_optimizacion->matriz_p.P_x)free(problema_optimizacion->matriz_p.P_x);
  if(problema_optimizacion->matriz_p.P_i)free(problema_optimizacion->matriz_p.P_i);
  if(problema_optimizacion->matriz_p.P_p)free(problema_optimizacion->matriz_p.P_p);

  //Se libera los contenidos del vector q
  if(problema_optimizacion->vector_q)free(problema_optimizacion->vector_q);

  
  // Se libera los contenidos de la matriz A
  if (problema_optimizacion->matriz_a.A)free(problema_optimizacion->matriz_a.A);
  if(problema_optimizacion->matriz_a.A_i)free(problema_optimizacion->matriz_a.A_i);
  if(problema_optimizacion->matriz_a.A_x)free(problema_optimizacion->matriz_a.A_x);
  if(problema_optimizacion->matriz_a.A_p)free(problema_optimizacion->matriz_a.A_p);

  //Se liberan los contenidos de los vectores l y u
  if(problema_optimizacion->vector_l)free(problema_optimizacion->vector_l);
  if (problema_optimizacion->vector_u)free(problema_optimizacion->vector_u);

  //Se liberan los ajustes 
  if (problema_optimizacion->ajustes)free(problema_optimizacion->ajustes);


  //Se limpia el solver
  if(problema_optimizacion->solver)osqp_cleanup(problema_optimizacion->solver);

}