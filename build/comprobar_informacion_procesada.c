#include "definiciones_globales.h"
#include "operaciones_string.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*Funcion para imprimir la informacion procesada de los vehiculos*/

int imprimir_informacion_procesada_vehiculos(const informacion_vehiculos_t* informacion_vehiculos,
                                              const char* nombre_archivo) {
  //Se abre el archivo 
  FILE* file = fopen(nombre_archivo, "w");
  if (!file) {
    perror("Error al abrir el archivo para comprobar la informacion procesada de los vehiculos");
    registrar_error("Fallo en la comprobacion de la informacion de entrada", REGISTRO_ERRORES);
    return ERROR;
  }
  //Imprimir los encabezados del CSV
  fprintf(file, "NumeroTerminal,PuntoInicio,PuntoFinal,FechaInicio,FechaFinal,BateriaInicial,BateriaFinal,CapacidadBateria,ModoCarga\n");
  for (int i = 0; i < informacion_vehiculos->numero_vehiculos; i++) {
    const vehiculos_t* v = &informacion_vehiculos->vehiculos[i];
    fprintf(nombre_archivo, "%d,%d,%d,%s,%s,%.2f,%.2f,%.2f,%s\n",
      v->numero_terminal,
      v->punto_inicio,
      v->punto_final,
      tm_to_string(v->fecha_inicio),
      tm_to_string(v->fecha_final),
      v->bateria_inicial,
      v->bateria_final,
      v->capacidad_bateria,
      v->modo_carga);
  }
  fclose(nombre_archivo);
  return EXITO;
}

/*Funcion para imprimir la informacion de las baterias*/
int imprimir_informacion_procesada_bateria(const informacion_baterias_t * informacion_baterias,
                                            const char* nombre_archivo) {
  /*Se abre el archivo para escribir*/
  FILE* file = fopen(nombre_archivo, "w");
  if (!file) {
    perror("Error al abrir la informacion para comprobar la informacion procesada de las baterias");
    registrar_error("Error al abrir la informacion para comprobar la informacion procesada de las baterias", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se imprimir los encabezados de los CSV
  fprintf(file, "NumeroTerminal,PuntoInicio,PuntoFinal,PuntoObjetivo,CapacidadBateria,BateriaInicial,BateriaObjetivo,MaximaPotencia,FechaInicio,FechaFinal,FechaObjetivo\n");
  //Se itera por cada bateria y se imprime la infomracion procesada de cada bateria
  for (int i = 0; i < informacion_baterias->numero_baterias; i++) {
    const bateria_t* b = &informacion_baterias->baterias[i];
    fprintf(file, "%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%s,%s,%s\n",
      b->numero_terminal,
      b->punto_inicio,
      b->punto_final,
      b->punto_objetivo,
      b->capacidad_bateria,
      b->bateria_inicial,
      b->bateria_objetivo,
      b->maxima_potencia,
      tm_to_string(b->fecha_inicio),
      tm_to_string(b->fecha_final),
      tm_to_string(b->fecha_objetivo));
  }
  fclose(file);
  return EXITO;
}
/*Se utiliza este subprograma para imprimir la informacion procesada de las restricciones del sistema*/
int imprimir_informacion_procesada_restricciones(const informacion_restricciones_sistema_t* informacion_restricciones,
  const char* nombre_archivo) {
  //Se abre el archivo
  FILE* archivo = fopen(nombre_archivo, "w");
  if (!archivo) {
    perror("Error al abrir el archivo para comprobar la informacion procesada de las restricciones\n");
    registrar_error("Fallo en la comprobacion de la informacion procesada de las restricciones\n",REGISTRO_ERRORES);
    return ERROR;
  }
  /*Se imprime los encabezados en el csv de comprobacion*/
  fprintf(archivo, "PotenciaMaximaRed,PotenciaMinimaRed,PotenciaMaximaRed_R,PotenciaMaximaRed_S,PotenciaMaximaRed_T,"
    "PotenciaMinimaRed_R,PotenciaMinimaRed_S,PotenciaMinimaRed_T\n");

  /*Se imprime la infomracion procesada de las restricciones*/
  fprintf(archivo, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
    informacion_restricciones->potencia_maxima_red,
    informacion_restricciones->potencia_minima_red,
    informacion_restricciones->potencia_maxima_red_R,
    informacion_restricciones->potencia_maxima_red_S,
    informacion_restricciones->potencia_maxima_red_T,
    informacion_restricciones->potencia_minima_red_R,
    informacion_restricciones->potencia_minima_red_S,
    informacion_restricciones->potencia_minima_red_T);

  //Se imprime la potencia maxima de las estaciones de la carga
  fprintf(archivo, "PotenciaMaximaEstacionesCarga\n");
  for (int i = 0; i < NUMERO_TERMINALES; i++) {
    fprintf(archivo, "%d,%.2f\n", i + 1, informacion_restricciones->potencia_maxima_estaciones_carga[i]);
  }

  //Se cierra el archivo
  fclose(archivo);
  return EXITO;
}

/*Se crea un subprograma para imprimir la informacion procesada de los puntos de simulacion*/
int imprimir_puntos_simulacion(const informacion_puntos_simulacion_t* informacion_puntos_simulacion,
  const char* nombre_archivo) {

  //Se abre el archivo
  FILE* archivo = fopen(nombre_archivo, "w");
  /*Si no se puede abrir el archivo -> */
  if (!archivo) {
    perror("Error al abrir el archivo para comprobar la informacion procesada de los puntos de simulacion");
    registrar_error("Fallo en la comprobacion de la informacion de los puntos de simulacion", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se imprime los encabezados del CSV de los puntos de simulacion
  fprintf(archivo, "NumeroPunto,FechaPunto,Delta,PuntoSimulacion\n");
  for (int i = 0; i < informacion_puntos_simulacion->numero_puntos_simulacion; i++) {
    const punto_simulacion_t* p = &informacion_puntos_simulacion->puntos_simulacion[i];
    fprintf(archivo, "%d,%s,%d,%d\n",
      i + 1, // Assuming you want a point number starting from 1
      tm_to_string(p->fecha_punto),
      p->delta,
      p->punto_simulacion);
  
  }
  //Se cierra el archivo
  fclose(archivo);
  return EXITO;
}

int imprimir_informacion_precios(const informacion_precio_t* informacion_precios,
  const char* nombre_archivo) {
  //Se abre el archivo
  FILE* archivo = fopen(nombre_archivo, "w");
  if (!archivo) {
    perror("Error al abrir el archivo para comprobar la informacion procesada de los precios");
    registrar_error("Fallo en la comprobacion de la informacion de precios", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se imprimme los encabezados en el csv de comprobacion de la informacion procesada de los precios
  fprintf(archivo, "FechaAsociada,Precio,PuntoInicial,PuntoFinal\n");

  //Se itera por cada precio
  for (int i = 0; i < informacion_precios->numero_horas; i++) {
    const precio_t* precio = &informacion_precios->precios[i];
    fprintf(archivo, "%s,%.2f,%d,%d\n",
      tm_to_string(precio->fecha_asociada),
      precio->precio,
      precio->punto_inicial,
      precio->punto_final);
  }
  /*Se cierra el archivo*/
  fclose(archivo);
  return EXITO;

}

/*Se imprime los puntos adicionales en un archivo con extension csv para comprobar que todo es correcto*/
int imprimir_informacion_puntos_adicionales(informacion_puntos_adicionales_t* informacion_fechas_adicionales,
                                            const char* nombre_archivo ) {
  FILE* archivo = fopen(nombre_archivo, "w");
  if (!archivo) {
    perror("No se ha podido abrir el archivo de comprobacion de los puntos adicionales\n");
    registrar_error("No se ha podido abrir el archivo de comprobacion de los puntos adicionales\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se imprime los encabezados en el csv
  fprintf(archivo, "NumeroPunto,FechaAdicional\n");
  //Se itera por cada punto adicional y se imprime la informacion
  for (int i = 0; i < informacion_fechas_adicionales->numero_puntos; i++) {
    fprintf(archivo, "%d,%s\n",
      informacion_fechas_adicionales->puntos[i].numero_punto,
      tm_to_string(informacion_fechas_adicionales->puntos[i].fecha_adicional));
      
  }
  
  fclose(archivo);
  return EXITO;
}

/*Este subprograma se utiliza para comprobar que la informacion procesada sea correcta*/
int comprobar_informacion_procesada(informacion_procesada_t* informacion_procesada) {

  /*Se imprime la informacion procesada de los vehiculos*/
  if (imprimir_informacion_procesada_vehiculos(&informacion_procesada->informacion_vehiculos,
    "Comprobar_informacion_procesada_vehiculos") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los vehiculos\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los vehiculos\n",REGISTRO_ERRORES);
    return ERROR;
  }

  /*Se imprime la informacion procesada de las baterias*/
  if (imprimir_informacion_procesada_bateria(&informacion_procesada->informacion_baterias,
    "Comprobar_informacion_procesada_bateria") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de las baterias\n");
    registrar_error("No se ha podido comprobar la informacion procesada de las baterias\n",REGISTRO_ERRORES);
    return ERROR;
  }

  /*Se imprime la informacion procesada de las restricciones*/
  if (imprimir_informacion_procesada_restricciones(&informacion_procesada->informacion_restricciones_sistema,
    "Comprobar_informacion_procesada_restricciones") == ERROR) {
    printf("No se ha comprobado la informacion procesada de las restricciones\n");
    registrar_error("No se ha comprobado la informacion procesada de las restricciones\n",REGISTRO_ERRORES);
    return ERROR;
  }

  /*Se imprime la informacion procesada de los precios de compra*/
  if (imprimir_informacion_precios(&informacion_procesada->informacion_precio_compra,
    "Comprobar_informacion_procesada_precios_compra") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los precios de compra\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los precios de compra\n",REGISTRO_ERRORES);
    return ERROR;
  }

  /*Se imprime la informacion procesada de los precios de venta*/
  if (imprimir_informacion_precios(&informacion_procesada->informacion_precio_venta,
    "Comprobar_informacion_procesada_precios_venta") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los precios de venta\n");
    registrar_error("No se ha podido comprobar la informacion de los precios de venta\n",REGISTRO_ERRORES);
    return ERROR;
  }

  /*Se imprime la informacion procesada de los puntos de simulacion */
  if (imprimir_puntos_simulacion(&informacion_procesada->informacion_puntos_simulacion,
    "Comprobar_informacion_procesada_puntos_simulacion") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los puntos de simulacion\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los puntos de simulacion\n", REGISTRO_ERRORES);
    return ERROR;
  }
  /*Se imprime la informacion procesada de los puntos adicionales */

  if (imprimir_informacion_puntos_adicionales(&informacion_procesada->informacion_puntos_adicionales,
    "Comprobar_informacion_procesada_puntos_adicionales") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los puntos adicionales\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los puntos adicionales\n", REGISTRO_ERRORES);
    return ERROR;
  }

  return EXITO;
}