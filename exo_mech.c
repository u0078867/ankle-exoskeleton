
#include "exo_mech.h"


/**************************************************************************/
/*
    Calculate quadrilateral angle GAMMA (deg)
*/
/**************************************************************************/
float calcGAMMA(float H, float B) {
  double res;
  res = acos( (pow(QUADRL_D, 2) + pow(B, 2) - pow(H, 2)) / (2. * QUADRL_D * B) );
  return (float)(RAD_TO_DEG * res);
}


/**************************************************************************/
/*
    Calculate quadrilateral side B from H (mm)
*/
/**************************************************************************/
float calcB_H(float H) {
  double res;
  res = Z0_H_TO_B + Z1_H_TO_B * H + Z2_H_TO_B * pow(H, 2);
  return (float)res;
}
