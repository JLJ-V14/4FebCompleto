#pragma once
/*En este archivo se definen las diferentes variables que se
  van a utilizar durante todo el codigo*/

#include "osqp.h"
#include <time.h>
#include <stdbool.h>



#define NUMERO_TERMINALES 12

  //Tipo que sirve para almacenar la posicion de una fecha en los
  //csvs.
typedef struct {
    int columna_anyo;
    int columna_mes;
    int columna_dia;
    int columna_hora;
    int columna_minuto;
}ubicacion_fecha_t;

//Tipo que sirve para definir el tipo de variable donde se almacenan los datos del CSV.
typedef struct {
    char*** datos;
    int filas;
    int columnas;
} datos_csv_t;


//Defino un tipo para almacenar los diferentes csvs de entrada.
typedef struct {
    const char* nombre_archivo;
    datos_csv_t* datos;
} entrada_csv;

/*Se definen una serie de estructuras para almacenar
  la posicion de la informaicon en los diferentes csv
  es decir donde se encuentra la informacion      */
typedef struct {
    int columna_terminales;
    int columna_modo_carga;
    int columna_capacidad_bateria;
    int columna_porcentaje_bateria_inicial;
    int columna_porcentaje_bateria_deseada;
    int columna_maxima_potencia;
    ubicacion_fecha_t ubicacion_fecha_inicial_vehiculo;
    ubicacion_fecha_t ubicacion_fecha_final_vehiculo;
}ubicacion_csv_vehiculos_t;

typedef struct {
    int fila_encabezados;
    int fila_informacion;
    ubicacion_fecha_t ubicacion_fecha_inicial_algoritmo;
    ubicacion_fecha_t ubicacion_fecha_final_algoritmo;
    int resolucion_minutos;
}ubicacion_csv_algoritmo_t;

typedef struct {
    int fila_encabezados;
    int columna_terminal;
    int columna_capacidad_bateria;
    int columna_porcentaje_bateria_inicial;
    int columna_porcentaje_bateria_deseada;
    int columna_potencia_maxima_bateria;
    ubicacion_fecha_t ubicacion_fecha_inicial_baterias;
    ubicacion_fecha_t ubicacion_fecha_final_baterias;
    ubicacion_fecha_t ubicacion_fecha_objetivo_baterias;
    int columna_consideracion_objetivo;
}ubicacion_csv_baterias_t;

typedef struct {
    int fila_encabezados;
    ubicacion_fecha_t ubicacion_fecha_precios;
    int columna_precio;
}ubicacion_csv_precios_t;


typedef struct {
    int fila_encabezados;
    int columna_numero_terminal;
    int columna_fase;
}ubicacion_csv_terminales_t;

typedef struct {
    int fila_encabezados;
    int fila_valores;
    int columna_max_potencia_sistema;
    int columna_max_potencia_terminal_1;
    int columna_max_potencia_terminal_2;
    int columna_max_potencia_terminal_3;
    int columna_max_potencia_terminal_4;
    int columna_max_potencia_terminal_5;
    int columna_max_potencia_terminal_6;
    int columna_max_potencia_terminal_7;
    int columna_max_potencia_terminal_8;
    int columna_max_potencia_terminal_9;
    int columna_max_potencia_terminal_10;
    int columna_max_potencia_terminal_11;
    int columna_max_potencia_terminal_12;
}ubicacion_csv_restricciones_t;

typedef struct {
    datos_csv_t               informacion_algoritmo;
    ubicacion_csv_algoritmo_t posiciones_informacion_algoritmo;
}datos_csv_algoritmo_t;

typedef struct {
    datos_csv_t               informacion_vehiculos;
    ubicacion_csv_vehiculos_t posiciones_informacion_vehiculos;
}datos_csv_vehiculos_t;

typedef struct {
    datos_csv_t               informacion_baterias;
    ubicacion_csv_baterias_t  posiciones_informacion_baterias;
}datos_csv_baterias_t;

typedef struct {
    datos_csv_t               informacion_restricciones;
    ubicacion_csv_restricciones_t posiciones_informacion_restricciones;
}datos_csv_restricciones_t;

typedef struct {
    datos_csv_t                informacion_terminales;
    ubicacion_csv_terminales_t posiciones_informacion_terminales;
}datos_csv_terminales_t;


typedef struct {
    datos_csv_t                    informacion_precio;
    ubicacion_csv_precios_t        posiciones_informacion_precio;
}datos_csv_precio_t;



typedef struct {
    datos_csv_algoritmo_t       datos_algoritmo;
    datos_csv_vehiculos_t       datos_vehiculos;
    datos_csv_baterias_t        datos_baterias;
    datos_csv_restricciones_t   datos_restricciones;
    datos_csv_terminales_t      datos_terminales;
    datos_csv_precio_t          datos_precio_compra;
    datos_csv_precio_t          datos_precio_venta;

}informacion_entrada_t;


//Defino una estructura para guardar las matrices del problema de optimizacion 

//La siguiente escructura contiene los siguientes campos->
// Hay dos matrices la P y la Q la matriz P sirve para indicar los objetivos 
// cuadraticos y la q los objetivos no cuadraticos la matriz p tiene los siguientes
// cambios
// La matriz P tiene los siguientes campos 
// P_nnz-> Numero de términos que son distintos de 0.
// P_x->   Valores de los términos diferentes de 0.
// P_i->   Indica los índices de las filas en las que se encuentran los 
// terminos diferentes de 0.

typedef struct {
    OSQPCscMatrix* P;
    OSQPInt        P_nnz;
    OSQPFloat*     P_x;
    OSQPInt*       P_i;
    OSQPInt*       P_p;
    OSQPFloat*     q;
}matriz_objetivos_t;

//La matriz A sirve para indicar las restricciones a las que están sometidas las variables 
//de decisión 
//A_nnz-> Términos diferenes de 0
//A_x-> Este Array contiene los valores diferentes de 0 en la matriz 
//A_i-> Este vector contiene los índices de las filas en las que se encuentran los términos diferentes 
//de 0 
//A_p-> Indica cuando empiezan las columnas respecto al vector A_i 
//l-> valor inferior de la restricción
//u-> valor superior de la restricción

typedef struct {
    OSQPCscMatrix* A;
    OSQPInt        A_nnz;
    OSQPFloat*     A_x;
    OSQPInt*       A_i;
    OSQPInt*       A_p;
    OSQPFloat*     l;
    OSQPFloat*     u;
}matriz_restricciones_t;

//Ajustes generales del problema de optimizacion en este estructura
//se guardan las matrices, el solver los ajustes el numero de variables
//y restricciones.

typedef struct {
    OSQPSolver*                solver;
    OSQPSettings*              ajustes;
    OSQPInt                    bandera_salida;
    matriz_objetivos_t         matriz_objetivos;
    matriz_restricciones_t     matriz_restricciones;
    OSQPInt                    numero_variables;
    OSQPInt                    numero_restricciones;
}problema_optimizacion_t;

//Struct que se utiliza para guardar la informacion importante de un vehiculo
//Se compone de los siguientes campos: 
//numero_terminal -> numero de terminal a que esta conectado el vehiculo
//punto_inicio    -> punto de inicio de la simulacion
//punto_final     -> punto final de la simulacion 
//fecha_inicio    -> fecha de inicio de la carga
//fecha_final     -> fecha final de la carga 
//bateria_inicial -> bateria inicial de la carga
//bateria_final   -> bateria final de la carga
//capacidad_bateria -> capacidad de la bateria del vehiculo 
//modo_carga-> modo de carga del vehiculo rapida o normal.

typedef struct {
    OSQPInt       numero_terminal;
    OSQPInt       punto_inicio;
    OSQPInt       punto_final;
    struct tm     fecha_inicio;
    struct tm     fecha_final;
    OSQPFloat     bateria_inicial;
    OSQPFloat     bateria_final;
    OSQPFloat     capacidad_bateria;
    char*         modo_carga;
}vehiculos_t;

//numero_vehiculos -> numero de vehiculos en el sistema 
//vehiculos-> variable que sirve para almacenar la informacion de un vehiculo
typedef struct {
    int     numero_vehiculos;
    vehiculos_t *vehiculos;
}informacion_vehiculos_t;

//Struct que sirve para almacenar la informacion relevante de las 
// baterías conectadas al sistema
// Considerar_objetivo boolean type 
// int numero_terminal -> numero de terminal al que está conectado el vehículo
// capacidad_batería -> capacidad de la batería del vehículo en cuestión 
// bateria_inicial  -> bateria inicial de la batería
// bateria_objetivo  -> objetivo de carga de la batería (% carga deseado)
// maxima_potencia   -> maxima potencia que puede aceptar la batería 
// fecha_inicio      -> fecha de inicio de carga de la batería 
// fecha_final       -> fecha de finalización de carga de la batería 
// fecha_objetivo    -> fecha de objetivo de carga de la batería (posible
// punto_inicio      -> punto de inciio de carga de la batería 
// punto_final       -> punto de finalización de carga de la batería 
// punto_objetivo    -> punto de objetivo de carga de la batería 


typedef struct {
    bool         considerar_objetivo;
    OSQPInt      numero_terminal;
    OSQPInt      punto_inicio;
    OSQPInt      punto_final;
    OSQPInt      punto_objetivo;
    OSQPFloat    capacidad_bateria;
    OSQPFloat    bateria_inicial;
    OSQPFloat    bateria_objetivo;
    OSQPFloat    maxima_potencia;
    struct tm*   fecha_inicio;
    struct tm*   fecha_final;
    struct tm*   fecha_objetivo;
}bateria_t;

// Numero_baterias-> Numero de baterías presente en el sistema
// baterias-> array con la informacion de todas las baterías presentes en el sistema 

typedef struct {
    int numero_baterias;
    bateria_t* baterias;
}informacion_baterias_t;

//Struct que sirve para almacenar cuantos puntos de cálculo tiene la optimización
//cada punto tiene una fecha asociada.

// fecha_punto -> fecha del punto asociado en cuestión 
// delta -> deltat entre el punto de simulacion en cuestión y el anterior deltat puede cambiar) 
typedef struct {
    struct tm* fecha_punto;
    int        delta;
    int        punto_simulacion;
}punto_simulacion_t;

//Struct que sirve para almacenar la informacion de los puntos de simulacion 
//  numero_puntos_simulacion-> variable que sirve para almacenar el número de puntos que contiene la simulación.
//  delta_minutos-> diferencia temporal entre los diferentes puntos de la simulación.
//  puntos_simulacion-> array que contiene la informacion de todos los puntos de la simulacion

typedef struct {
    int numero_puntos_simulacion;
    int delta_minutos;
    punto_simulacion_t* puntos_simulacion;
}informacion_puntos_simulacion_t;

//Struct que sirve para almacenar las restricciones del sistema 
//fácilmente  almacenan en este caso la maxima potencia que pueden intercambiar
//la red con el sistema y la red en cada fase.
typedef struct {
    OSQPFloat  potencia_maxima_red;
    OSQPFloat  potencia_minima_red;
    OSQPFloat  potencia_maxima_red_R;
    OSQPFloat  potencia_maxima_red_S;
    OSQPFloat  potencia_maxima_red_T;
    OSQPFloat  potencia_minima_red_R;
    OSQPFloat  potencia_minima_red_S;
    OSQPFloat  potencia_minima_red_T;
    float      potencia_maxima_estaciones_carga[12];
    float      potencia_minima_estaciones_carga[12];
}informacion_restricciones_sistema_t;

// Este struct contiene para cada precio el valor del precio, la fecha asociada ( hora) y el punto 
// inicial y final de la simulacion a los cuales le corresponde ese precio.

typedef struct {
    struct tm* fecha_asociada;
    float      precio;
    int        punto_inicial;
    int        punto_final;
}precio_t;

//Este struct contiene un array con los precios asociados a cada hora y el numero de horas.
typedef struct {
    int numero_horas;
    precio_t* precios;
}informacion_precio_t ;

/*Se define un tipo para almacenar las fechas que deben estar presenten en la simulacion
  y a que punto corresponde, cada fecha.*/

typedef struct {
    int numero_punto;
    struct tm fecha_adicional;
}puntos_adicionales_t;


typedef struct {
    int numero_puntos;
    puntos_adicionales_t* puntos;
}informacion_puntos_adicionales_t;

// Se definen structs para almacenar la informacion ya extraida de los CSVs de
// entrada para poder rellenar las matrices con la informacion relevante fácilmente
// informacion_vehiculos_t -> este struct se utiliza para almacenar toda la informacion relevante a los vehiculos
// informacion_baterias_t  -> este struct se utiliza para almcanear toda la informacion relevante a las baterias
// informacion_puntos_simulacion_t -> informacion de los puntos de simulacion
// informacion_restricciones_sistema -> informacion de las restricciones del sistema
// informacion_precio_t -> informacion de los precios de la electricidad
typedef struct {
    informacion_vehiculos_t             informacion_vehiculos;
    informacion_baterias_t              informacion_baterias;
    informacion_puntos_simulacion_t     informacion_puntos_simulacion;
    informacion_restricciones_sistema_t informacion_restricciones_sistema;
    informacion_precio_t                informacion_precio_compra;
    informacion_precio_t                informacion_precio_venta;
    informacion_puntos_adicionales_t    informacion_puntos_adicionales;
}informacion_procesada_t;

