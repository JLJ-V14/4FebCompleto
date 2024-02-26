
#include "definiciones_globales.h"
#include "osqp.h"
#include "tipos_optimizacion.h"

/*Este subprograma se utiliza para escribir en el vector l los limites inferiores de los valores que pueden tomar
  los estados de la bateria */
void calcular_limite_superior_restricciones_bateria(informacion_procesada_t* informacion_sistema, OSQPFloat* u) {
  //Cargo el numero de puntos de simulacion que se tienen
  int numero_puntos_simulacion = informacion_sistema->informacion_puntos_simulacion.numero_puntos_simulacion;
  //Creo una variable para situar los terminos en las posiciones correctas del vector
  int offset_index_vector_u;
  //Como hay 12 terminales se itera por cada uno de ellos
  for (int numero_terminal = 0; numero_terminal < NUMERO_TERMINALES; numero_terminal++) {
    offset_index_vector_u = numero_terminal * numero_puntos_simulacion;
    for (int punto_simulacion = 0; punto_simulacion < numero_puntos_simulacion; punto_simulacion++) {
      u[offset_index_vector_u + punto_simulacion] = (OSQPFloat)1.0;
    }
  }
}