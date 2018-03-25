
// Needed GCC 4.7+

#pragma GCC push_options
#pragma GCC optimize ("Ofast")

#include "gamma_to_teta_delta_cse_ofast.h"
#include <math.h>

double gamma_to_teta_delta_cse_ofast(double A, double B, double C, double L, double dgamma, double gammac, double tetac) {
  double tmp0 = 1.0*B/L;
  double tmp1 = pow(L, 2);
  double tmp2 = 1.0/tmp1;
  double tmp3 = pow(B, 2);
  double tmp4 = 0.5*tmp3;
  double tmp5 = tmp2*tmp4 + 0.5;
  double tmp6 = pow(A, 2);
  double tmp7 = 1.0/tmp6;
  double tmp8 = 0.5*tmp7;
  double tmp9 = tmp3*tmp8;
  double tmp10 = pow(C, 2);
  double tmp11 = 0.25*tmp1;
  double tmp12 = 0.25*tmp2;
  double tmp13 = tmp10*tmp2;
  double tmp14 = cos(gammac);
  double tmp15 = 1.0*B*C*tmp14;
  double tmp16 = pow(B, 4);
  double tmp17 = 0.25*tmp2*tmp7;
  double tmp18 = pow(B, 3);
  double tmp19 = 1.0/tmp10;
  double tmp20 = 0.5*tmp6;
  double tmp21 = tmp19*tmp20;
  double tmp22 = cos(tetac);
  double tmp23 = 1.0*A*B*tmp22;
  double tmp24 = 0.25*tmp19*tmp2;
  double tmp25 = tmp2*tmp3;
  double tmp26 = tmp19*tmp2*tmp22;
  double r;
  r = dgamma*(-1 - C*tmp0*sin(gammac)/(A*sqrt(1.0*B*pow(C, 3)*tmp14*tmp2*tmp7 - pow(C, 4)*tmp17 + 1.0*C*tmp14*tmp18*tmp2*tmp7 + tmp10*tmp8 - tmp11*tmp7 - tmp12*tmp6 - 1.0*tmp13*pow(tmp14, 2)*tmp3*tmp7 - tmp13*tmp9 + 0.5*tmp13 - tmp15*tmp2 - tmp15*tmp7 - tmp16*tmp17 + tmp5 + tmp9)))/(A*tmp0*sin(tetac)/(C*sqrt(-pow(A, 4)*tmp24 + 1.0*pow(A, 3)*B*tmp26 + 1.0*A*tmp18*tmp26 - tmp10*tmp12 - tmp11*tmp19 - tmp16*tmp24 - 1.0*tmp19*pow(tmp22, 2)*tmp25*tmp6 - tmp19*tmp23 + tmp19*tmp4 + tmp2*tmp20 - tmp2*tmp23 - tmp21*tmp25 + tmp21 + tmp5)) + 1);
  return r;
}

#pragma GCC pop_options
