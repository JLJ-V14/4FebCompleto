#include "definiciones_globales.h"
#include "operaciones_string.h"
#include "registrar_errores.h"
#include "tipos_optimizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void printTm_3(const struct tm* date) {
  if (date == NULL) {
    printf("The date is NULL\n");
    return;
  }

  // Note: tm_year is years since 1900, tm_mon is 0-11, and tm_mday is 1-31
  printf("Year: %d, Month: %02d, Day: %02d\n",
    date->tm_year + 1900, date->tm_mon + 1, date->tm_mday);
  printf("Hour: %02d, Minute: %02d, Second: %02d\n",
    date->tm_hour, date->tm_min, date->tm_sec);

  // Optionally, you can print the day of the week and day of the year
  printf("Weekday (Sunday=0): %d, Day of the year: %d\n",
    date->tm_wday, date->tm_yday);
}


/*Funcion para imprimir la informacion procesada de los vehiculos*/

int imprimir_informacion_procesada_vehiculos(const informacion_vehiculos_t informacion_vehiculos,
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
 
  for (int i = 0; i < informacion_vehiculos.numero_vehiculos; i++) {
    const vehiculos_t v = informacion_vehiculos.vehiculos[i];

    // Convertir las fechas a strings antes de escribir en el archivo
    char fechaInicioStr[20]; // Asegúrate de que el buffer sea lo suficientemente grande
    strftime(fechaInicioStr, sizeof(fechaInicioStr), "%Y-%m-%d %H:%M", &(v.fecha_inicio));

    char fechaFinalStr[20];
    strftime(fechaFinalStr, sizeof(fechaFinalStr), "%Y-%m-%d %H:%M", &(v.fecha_final));

    fprintf(file, "%lld,%lld,%lld,%s,%s,%.2f,%.2f,%.2f,%s\n",
      v.numero_terminal,
      v.punto_inicio,
      v.punto_final,
      fechaInicioStr,
      fechaFinalStr,
      v.bateria_inicial,
      v.bateria_final,
      v.capacidad_bateria,
      v.modo_carga);
      
  }
   
  fclose(file);
 
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


  fprintf(file, "NumeroTerminal,PuntoInicio,PuntoFinal,CapacidadBateria,BateriaInicial,MaximaPotencia,FechaInicio,FechaFinal,BateriaObjetivo,PuntoObjetivo,FechaObjetivo\n");
  //Se itera por cada bateria y se imprime la infomracion procesada de cada bateria
  for (int i = 0; i < informacion_baterias->numero_baterias; i++) {

    const bateria_t* b = &(informacion_baterias->baterias[i]);

    char fechaInicioStr[20]; // Ensure the buffer is large enough for the formatted date-time string
    strftime(fechaInicioStr, sizeof(fechaInicioStr), "%Y-%m-%d %H:%M", &(b->fecha_inicio));

    char fechaFinalStr[20];
    strftime(fechaFinalStr, sizeof(fechaFinalStr), "%Y-%m-%d %H:%M", &(b->fecha_final));

    if (informacion_baterias->baterias[i].considerar_objetivo) {
      char fechaObjetivoStr[20];
      strftime(fechaObjetivoStr, sizeof(fechaObjetivoStr), "%Y-%m-%d %H:%M", &(b->fecha_objetivo));

      fprintf(file, "%lld,%lld,%lld,%.2f,%.2f,%.2f,%s,%s,%.2f,%lld,%s\n",
        b->numero_terminal,
        b->punto_inicio,
        b->punto_final,
        b->capacidad_bateria,
        b->bateria_inicial,
        b->maxima_potencia,
        fechaInicioStr,
        fechaFinalStr,
        b->bateria_objetivo,
        b->punto_objetivo,
        fechaObjetivoStr);
    }


    else {
      fprintf(file, "%lld,%lld,%lld,%.2f,%.2f,%.2f,%s,%s\n",
        b->numero_terminal,
        b->punto_inicio,
        b->punto_final,
       
        b->capacidad_bateria,
        b->bateria_inicial,
        
        b->maxima_potencia,
        fechaInicioStr,
        fechaFinalStr);
    }
  }
  /* */
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
      tm_to_string(&(p->fecha_punto)),
      p->delta,
      p->punto_simulacion);
  
  }
  //Se cierra el archivo
  fclose(archivo);
  return EXITO;
}

int imprimir_informacion_precios(const informacion_precio_t* informacion_precios,
  const char* nombre_archivo, informacion_puntos_simulacion_t* informacion_puntos_simulacion) {

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
  for (int i = 0; i <informacion_precios->numero_precios; i++) {
    
    const precio_t* precio = &(informacion_precios->precios[i]);
 
   
    fprintf(archivo, "%s,%f,%lld,%lld\n",
      tm_to_string(&(precio->fecha_asociada)),
      precio->precio,
      precio->punto_inicial,
      precio->punto_final);
  }
  /*Se cierra el archivo*/
  fclose(archivo);
  return EXITO;

}




// Se utiliza el siguiente subprograma para imprimir la informacion procesada de los precios
int imprimir_informacion_terminales(informacion_terminales_t* informacion_terminales,const char* nombre_archivo) {
  FILE* archivo = fopen(nombre_archivo, "w");
  if (!archivo) {
    perror("No se ha podido abrir el archivo de comprobación de los puntos adicionales\n");
    registrar_error("No se ha podido abrir el archivo de comprobacion de la informacion de los terminales\n", REGISTRO_ERRORES);
    return ERROR;
  }
  //Se imprimen los encabezados 
  fprintf(archivo, "Numero Terminal,Fase\n");
  //Se imprimime 
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    
    fprintf(archivo, "%d,%c\n", numero_terminal + 1, informacion_terminales->fases_electricas[numero_terminal]);
  }
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
      tm_to_string(&(informacion_fechas_adicionales->puntos[i].fecha_adicional)));
      
  }
  
  fclose(archivo);
  return EXITO;
}

/*Este subprograma se utiliza para comprobar que la informacion procesada sea correcta*/
int comprobar_informacion_procesada(informacion_procesada_t informacion_procesada) {
  
  //Se imprime la informacion procesada de los vehiculos
  if (imprimir_informacion_procesada_vehiculos((informacion_procesada.informacion_vehiculos),
    "Comprobar_informacion_procesada_vehiculos.csv") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los vehiculos\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los vehiculos\n",REGISTRO_ERRORES);
    return ERROR;
  }
  
  
  
  //Se imprime la informacion procesada de las baterias
  
  if (imprimir_informacion_procesada_bateria(&(informacion_procesada.informacion_baterias),
    "Comprobar_informacion_procesada_bateria.csv") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de las baterias\n");
    registrar_error("No se ha podido comprobar la informacion procesada de las baterias\n",REGISTRO_ERRORES);
    return ERROR;
  }
 
  //Se imprime la informacion procesada de las restricciones
  if (imprimir_informacion_procesada_restricciones(&(informacion_procesada.informacion_restricciones_sistema),
    "Comprobar_informacion_procesada_restricciones.csv") == ERROR) {
    printf("No se ha comprobado la informacion procesada de las restricciones\n");
    registrar_error("No se ha comprobado la informacion procesada de las restricciones\n",REGISTRO_ERRORES);
    return ERROR;
  }
  
  
  


  // Se imprime la informacion procesada de los precios de compra

 
 
  
  // Se imprime la informacion procesada de los puntos de simulacion 
  if (imprimir_puntos_simulacion(&(informacion_procesada.informacion_puntos_simulacion),
    "Comprobar_informacion_procesada_puntos_simulacion.csv") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los puntos de simulacion\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los puntos de simulacion\n", REGISTRO_ERRORES);
    return ERROR;
  }
  // Se imprime la informacion procesada de los puntos adicionales 
 
  if (imprimir_informacion_puntos_adicionales(&(informacion_procesada.informacion_puntos_adicionales),
    "Comprobar_informacion_procesada_puntos_adicionales.csv") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los puntos adicionales\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los puntos adicionales\n", REGISTRO_ERRORES);
    return ERROR;
  }
  
  
  if (imprimir_informacion_precios(&(informacion_procesada.informacion_precio_compra),
    "Comprobar_informacion_procesada_precios_compra.csv",&(informacion_procesada.informacion_puntos_simulacion)) == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los precios de compra\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los precios de compra\n",REGISTRO_ERRORES);
    return ERROR;
  }
  
  
    // Se imprime la informacion procesada de los precios de venta
  if (imprimir_informacion_precios(&informacion_procesada.informacion_precio_venta,
    "Comprobar_informacion_procesada_precios_venta.csv", &(informacion_procesada.informacion_puntos_simulacion)) == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los precios de venta\n");
    registrar_error("No se ha podido comprobar la informacion de los precios de venta\n",REGISTRO_ERRORES);
    return ERROR;
  }
  
  
  if (imprimir_informacion_terminales(&(informacion_procesada.informacion_terminales), "Comprobar_informacion_procesada_terminales.csv") == ERROR) {
    printf("No se ha podido comprobar la informacion procesada de los terminales\n");
    registrar_error("No se ha podido comprobar la informacion procesada de los terminales\n", REGISTRO_ERRORES);
    return ERROR;
  }
  /*
/**/
  return EXITO;
}