/******************************************************************************
 *                      Code generated with sympy 0.7.6                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                       This file is part of 'project'                       *
 ******************************************************************************/
#include "gamma_to_teta_delta_fast.h"
#include <math.h>
double gamma_to_teta_delta_R(double L, double dgamma, double gammac, double tetac) {
   double gamma_to_teta_delta_R_result;
   gamma_to_teta_delta_R_result = dgamma*(-1 - 438.709677419355*sin(gammac)/(L*sqrt(-0.000260145681581686*pow(L, 2) - 14.1519250780437*cos(gammac) + 23.7174817898023 - 192466.181061394*pow(cos(gammac), 2)/pow(L, 2) + 617915.504682622*cos(gammac)/pow(L, 2) - 495956.703694069/pow(L, 2))))/(1 + 91.1764705882353*sin(tetac)/(L*sqrt(-5.40657439446367e-5*pow(L, 2) - 1.34083044982699*cos(tetac) + 4.92917387543253 - 8313.14878892734*pow(cos(tetac), 2)/pow(L, 2) + 48721.7560553633*cos(tetac)/pow(L, 2) - 71387.1955557959/pow(L, 2))));
   return gamma_to_teta_delta_R_result;
}
double gamma_to_teta_delta_L(double L, double dgamma, double gammac, double tetac) {
   double gamma_to_teta_delta_L_result;
   gamma_to_teta_delta_L_result = dgamma*(-1 - 406.451612903226*sin(gammac)/(L*sqrt(-0.000260145681581686*pow(L, 2) - 13.111342351717*cos(gammac) + 23.3766909469303 - 165202.913631634*pow(cos(gammac), 2)/pow(L, 2) + 563892.611862643*cos(gammac)/pow(L, 2) - 481188.362122789/pow(L, 2))))/(1 + 98.4126984126984*sin(tetac)/(L*sqrt(-6.29881582262535e-5*pow(L, 2) - 1.56210632401109*cos(tetac) + 5.66011589821114 - 9685.05920886873*pow(cos(tetac), 2)/pow(L, 2) + 57785.4371378181*cos(tetac)/pow(L, 2) - 86193.5036533132/pow(L, 2))));
   return gamma_to_teta_delta_L_result;
}
