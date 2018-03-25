/******************************************************************************
 *                      Code generated with sympy 0.7.6                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'project'                       *
 ******************************************************************************/
#include "gamma_to_teta_delta.h"
#include <math.h>
double gamma_to_teta_delta(double A, double B, double C, double L, double dgamma, double gammac, double tetac) {
   double gamma_to_teta_delta_result;
   gamma_to_teta_delta_result = dgamma*(-1 - 1.0*B*C*sin(gammac)/(A*L*sqrt(-0.25*pow(A, 2)/pow(L, 2) + 0.5*pow(B, 2)/pow(L, 2) - 1.0*B*C*cos(gammac)/pow(L, 2) + 0.5*pow(C, 2)/pow(L, 2) + 0.5 - 0.25*pow(B, 4)/(pow(A, 2)*pow(L, 2)) + 1.0*pow(B, 3)*C*cos(gammac)/(pow(A, 2)*pow(L, 2)) - 1.0*pow(B, 2)*pow(C, 2)*pow(cos(gammac), 2)/(pow(A, 2)*pow(L, 2)) - 0.5*pow(B, 2)*pow(C, 2)/(pow(A, 2)*pow(L, 2)) + 0.5*pow(B, 2)/pow(A, 2) + 1.0*B*pow(C, 3)*cos(gammac)/(pow(A, 2)*pow(L, 2)) - 1.0*B*C*cos(gammac)/pow(A, 2) - 0.25*pow(C, 4)/(pow(A, 2)*pow(L, 2)) + 0.5*pow(C, 2)/pow(A, 2) - 0.25*pow(L, 2)/pow(A, 2))))/(1.0*A*B*sin(tetac)/(C*L*sqrt(-0.25*pow(A, 4)/(pow(C, 2)*pow(L, 2)) + 1.0*pow(A, 3)*B*cos(tetac)/(pow(C, 2)*pow(L, 2)) - 1.0*pow(A, 2)*pow(B, 2)*pow(cos(tetac), 2)/(pow(C, 2)*pow(L, 2)) - 0.5*pow(A, 2)*pow(B, 2)/(pow(C, 2)*pow(L, 2)) + 0.5*pow(A, 2)/pow(L, 2) + 0.5*pow(A, 2)/pow(C, 2) + 1.0*A*pow(B, 3)*cos(tetac)/(pow(C, 2)*pow(L, 2)) - 1.0*A*B*cos(tetac)/pow(L, 2) - 1.0*A*B*cos(tetac)/pow(C, 2) - 0.25*pow(B, 4)/(pow(C, 2)*pow(L, 2)) + 0.5*pow(B, 2)/pow(L, 2) + 0.5*pow(B, 2)/pow(C, 2) - 0.25*pow(C, 2)/pow(L, 2) + 0.5 - 0.25*pow(L, 2)/pow(C, 2))) + 1);
   return gamma_to_teta_delta_result;
}
