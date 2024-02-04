/*
 * This file was autogenerated by OSQP on Wed Nov  2 13:22:21 2022
 * 
 * This file contains the prototype for the solver structure needed
 * by OSQP. The actual data is contained inside prob1_workspace.c.
 */

#ifndef LARGE_QP_DATA_H_
#define LARGE_QP_DATA_H_

#include "osqp_api_types.h"

#ifdef __cplusplus
extern "C" {
#endif

  extern OSQPFloat prob1_obj_val;

  extern OSQPInt prob1_data_n;
  extern OSQPInt prob1_data_m;

  extern OSQPCscMatrix prob1_data_P_csc;
  extern OSQPCscMatrix prob1_data_A_csc;

  extern OSQPFloat prob1_data_q_val[];
  extern OSQPFloat prob1_data_l_val[];
  extern OSQPFloat prob1_data_u_val[];

  extern OSQPSettings prob1_settings;

#ifdef __cplusplus
}
#endif

#endif /* ifndef LARGE_QP_DATA_H_ */