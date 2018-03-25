
#pragma once

#include "exo_glob.h"

/* IMPORTANT:

Suppose that to two equal motors have:
- equal spine lengths;
- equal positive rotation side;
*/


// Quadrilateral constants (mm)
#if SERVO == 1
#define QUADRL_A    31.0
#elif SERVO == 2
#define QUADRL_A    38.0
#endif

#if(SIDE)
#define QUADRL_C    68.0
#define QUADRL_D    33.0
#else
#define QUADRL_C    63.0
#define QUADRL_D    35.0
#endif

// Deformation of B due to gamma
#if(SIDE)
#if SERVO == 1
#define Z0_H_TO_B       2.55293212e+01
#define Z1_H_TO_B       1.68968056e+00
#define Z2_H_TO_B      -4.02988833e-03
#elif SERVO == 2



#endif
#else
#if SERVO == 1
#define Z0_H_TO_B       1.26535467e+02
#define Z1_H_TO_B       5.96779280e-01
#define Z2_H_TO_B      -1.16798202e-03
#elif SERVO == 2
#define Z0_H_TO_B       9.18135289e+01
#define Z1_H_TO_B       9.05979936e-01
#define Z2_H_TO_B      -1.93742482e-03
#endif
#endif

// Angle offsets (deg)
#if(SIDE)
#if SERVO == 1
#define TETA_TO_90_POS_SERVO   106.0 // value of teta when servo position is 90 deg
#elif SERVO == 2

#endif
#else
#if SERVO == 1
#define TETA_TO_90_POS_SERVO   101.0
#elif SERVO == 2
#define TETA_TO_90_POS_SERVO   102.0
#endif
#endif

// Rotation direction
#if(SIDE)
#if SERVO == 1
#define T_INC_POS_INC 1
#elif SERVO == 2
#define T_INC_POS_INC 0
#endif
#else
#if SERVO == 1
#define T_INC_POS_INC 0
#elif SERVO == 2
#define T_INC_POS_INC 1
#endif
#endif

// Conversion servo position <-> teta
#if T_INC_POS_INC == 1
#define TETA_TO_POS_SERVO(T)    (90. + ((T) - TETA_TO_90_POS_SERVO))
#define POS_SERVO_TO_TETA(P)    (-90. + (P) + TETA_TO_90_POS_SERVO)
#elif T_INC_POS_INC == 0
#define TETA_TO_POS_SERVO(T)    (90. - ((T) - TETA_TO_90_POS_SERVO))
#define POS_SERVO_TO_TETA(P)    (90. - (P) + TETA_TO_90_POS_SERVO)
#endif

// Servo limits (deg)
#if(SIDE)
#if SERVO == 1
#define SERVO_MIN_POS    10
#define SERVO_MAX_POS   140
#elif SERVO == 2


#endif
#else
#if SERVO == 1
#define SERVO_MIN_POS    20
#define SERVO_MAX_POS   130
#elif SERVO == 2
#define SERVO_MIN_POS    30
#define SERVO_MAX_POS   150
#endif
#endif
#define SERVO_MIN_BLOCK_POS (SERVO_MIN_POS+10)
#define SERVO_MAX_BLOCK_POS (SERVO_MAX_POS-10)

// Ankle angle limits (deg)
#define AAN_MIN_POS     -25
#define AAN_MAX_POS      25
#if T_INC_POS_INC == 1
#define AAN_MIN_BLOCK_POS   SERVO_MAX_BLOCK_POS
#define AAN_MAX_BLOCK_POS   SERVO_MIN_BLOCK_POS
#elif T_INC_POS_INC == 0
#define AAN_MIN_BLOCK_POS   SERVO_MIN_BLOCK_POS
#define AAN_MAX_BLOCK_POS   SERVO_MAX_BLOCK_POS
#endif

// Trigonometric constants
#define RAD_TO_DEG   57.29577951308232       // deg/rad
#define DEG_TO_RAD    0.017453292519943295   // rad/deg


#ifdef __cplusplus
extern "C" {
#endif

float calcGAMMA(float H, float B);
float calcB_H(float H);

#ifdef __cplusplus
}
#endif
