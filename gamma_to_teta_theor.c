
#include "gamma_to_teta_theor.h"
#include <math.h>

double gamma_to_teta_theor(double A, double B, double C, double L, double aGamma, double *aAlpha2, double *P2) {
   double aTeta, aAlpha, aTeta1, aTeta2;
   double P;

   P = sqrt(pow(B, 2) + pow(C, 2) - 2. * B * C * cos(aGamma));
   *P2 = P;
   aAlpha = acos((pow(A, 2) + pow(L, 2) - pow(P, 2)) / (2. * A * L));
   *aAlpha2 = aAlpha;

   aTeta1 = asin((L / P) * sin(aAlpha));
   if ((pow(A, 2) + pow(P, 2) - pow(L, 2)) / (2. * A * P) < 0)   {
       aTeta1 = M_PI - aTeta1;
   }
   aTeta2 = asin((C / P) * sin(aGamma));
   if ((pow(B, 2) + pow(P, 2) - pow(C, 2)) / (2. * B * P) < 0)   {
       aTeta2 = M_PI - aTeta2;
   }

   aTeta = aTeta1 + aTeta2;

   return aTeta;
}
