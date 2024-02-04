#pragma once
/*En este archivo se definen las diferentes variables que se
  van a utilizar durante todo el codigo*/

#include "osqp.h"
#include <stdbool.h>
#include <wctype.h>




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
    int       numero_terminal;
    int       punto_inicio;
    int       punto_final;
    struct tm fecha_inicio;
    struct tm fecha_final;
    float     bateria_inicial;
    float     bateria_final;
    float     capacidad_bateria;
    char*     modo_carga;
}vehiculos_t;

typedef struct {
    int     numero_vehiculos;
    vehiculos_t vehiculos;
}informacion_vehiculos_t;


typedef struct {
    bool   considerar_objetivo;
    int    numero_terminal;
    int    capacidad_bateria;
    int    bateria_inicial;
    float  bateria_objetivo;
    float  maxima_potencia;
    struct tm* fecha_inicio;
    struct tm* fecha_final;
    struct tm* fecha_objetivo;
}bateria_t;

typedef struct {
    int numero_baterias;
    bateria_t* baterias;
}informacion_baterias_t;


typedef struct {
    struct tm* fecha_punto;
    int        delta;
}punto_simulacion_t;

typedef struct {
    int numero_puntos_simulacion;
    int delta_minutos;
    punto_simulacion_t* puntos_simulacion;
}informacion_puntos_simulacion_t;

typedef struct {
    float potencia_maxima_red;
    float potencia_minima_red;
    float potencia_maxima_red_R;
    float potencia_maxima_red_S;
    float potencia_maxima_red_T;
    float potencia_minima_red_R;
    float potencia_minima_red_S;
    float potencia_minima_red_T;
    float potencia_maxima_estaciones_carga[12];
}informacion_restricciones_sistema_t;
typedef struct {
    informacion_vehiculos_t         informacion_vehiculos;
    informacion_baterias_t          informacion_baterias;
    informacion_puntos_simulacion_t informacion_puntos_simulacion;
    informacion_restricciones_sistema_t informacion_restricciones_sistema;
   
}informacion_procesada_t;
