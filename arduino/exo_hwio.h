
#pragma once

#include "../exo_glob.h"
#include <math.h>
#include <Arduino.h>
#include <stdbool.h>

/*
Consider:
- "p" is each point in analog input units

*/


// Analog inputs
#define ANALOG_IN_SERVO_CURRENT   0
#define ANALOG_IN_SERVO_POSITION  1
#define ANALOG_IN_LINPOT_SPRING   2
#define ANALOG_IN_LINPOT_ANKLE    3

// PWM servo output
#define PWM_OUT_SERVO   9

// Power servo output
#define POWER_SERVO_OUT 4

// Minimum time between 2 consecutive switches of power for the servo
#define MIN_INTER_SWITCH_TIME   200     // ms

// Analog-to-measure constants

#if(SIDE)
#if SERVO == 1
#define M_AIN_TO_H     0.00961241084839 // mm/p
#define Q_AIN_TO_H   180.734206735      // mm
#elif SERVO == 2


#endif
#define M_AIN_TO_L    -0.00979504499922 // mm/p
//#define Q_AIN_TO_L   215.302776591      // mm
#define Q_AIN_TO_L   205.302776591      // mm
#else
#if SERVO == 1
#define M_AIN_TO_H     0.00963950274048 // mm/p
#define Q_AIN_TO_H   176.713500561      // mm
#elif SERVO == 2
#define M_AIN_TO_H     0.00960823394758 // mm/p
#define Q_AIN_TO_H   167.754142209      // mm
#endif
#define M_AIN_TO_L    -0.00972873349318 // mm/p
//#define Q_AIN_TO_L   215.961940379      // mm
#define Q_AIN_TO_L   205.961940379      // mm
#endif

#if SERVO == 1
#define M_AIN_TO_SERVO_POSITION  0.0509639041772    // deg/p
#define Q_AIN_TO_SERVO_POSITION  -11.9293468775     // deg
#elif SERVO == 2
#if SERVO_POS_MEASURE == 1
#error "Savox servo was never opened to take out the position signal"
#endif
#define M_AIN_TO_SERVO_POSITION  0.0    // deg/p
#define Q_AIN_TO_SERVO_POSITION  0.0    // deg
#endif

// Analog-to-current constants
/*
If:
- Vout is partitioned with a 1/1.7434 factor
- 9.6V/A is the theoretical conversion factor (http://www.farnell.com/datasheets/1639866.pdf)
- 0A corresponds to a measure of 2.5V

m = 1.7434 * (-9.6A/V) * (3.3V / 4096p) = - 1.7434 * 9600mA/V * 0.0008056640625 V/p = -13.48mA/p
q = -(-9.6A/V * 2.5V) = 24000.0mA

*/
#define M_AIN_TO_SERVO_CURRENT      -13.48  // mA/p
#define Q_AIN_TO_SERVO_CURRENT   24000.0   // mA

// Max current for servo
#if SERVO == 1
#define SERVO_CURRENT_MAX   8000    // mA
#elif SERVO == 2
#define SERVO_CURRENT_MAX   3000    // mA
#endif

// Motor current-torque curve
/* See:

--- Pololu:

https://www.pololu.com/docs/0J21/5.c,
https://www.pololu.com/file/0J706/HD-1235MG.pdf

at 7.4V:
I = T * (9000 - 900) / 3.92266 + 900;
T = (I - 900) * 3.92266 / 8100;
T = 3.92266 / 8100 * I - 900 * 3.92266 / 8100;
T = 0.000484279012345679 * I - 0.43585111111111113;

at 6.0V:
T = 0.0005047540441176471 * I - 0.3533278308823529

--- Savox:

I-T curve at stall point was calulcated by using a dynamometer and appling
manually 40 N perpendicular to the servo spine. The stall torque in 42 kg * cm,
and with a spine length of 3.8 cm, the stall weight would be 11 kg, around
108 N. The current at 40 N is 1200 mA; to find the stall current, it should
then be multiplied by 108/40 = 2.7. The stall current is then around 3200 mA.
Idle current is around 100 mA.

at 7.4V:
I = T * (3200 - 100) / 4.119 + 100;
T = (I - 100) * 4.119 / 3100;
T = 4.119 / 3100 * I - 100 * 4.119 / 3100;
T = 0.0013545161290323 * I - 0.1328709677419355;

Servo Vin is not measured, but we assume we work close to 7.4V.

*/
#if SERVO == 1
#define M_SERVO_CURRENT_TO_TORQUE   0.000484279012345679    // Nm/mA
#define Q_SERVO_CURRENT_TO_TORQUE  -0.43585111111111113     // Nm
#elif SERVO == 2
#define M_SERVO_CURRENT_TO_TORQUE   0.0013545161290323    // Nm/mA
#define Q_SERVO_CURRENT_TO_TORQUE  -0.1328709677419355     // Nm
#endif

// LEDs constants
#define LED_GREEN 12
#define LED_RED   13

// LEDs constants
#define LED_GREEN_ON()      digitalWrite(LED_GREEN, HIGH);
#define LED_GREEN_OFF()     digitalWrite(LED_GREEN, LOW);
#define LED_RED_ON()        digitalWrite(LED_RED, HIGH);
#define LED_RED_OFF()       digitalWrite(LED_RED, LOW);

// Init function
#define HW_INIT() {   \
    /* ---- Initialize serial */    \
    Serial.begin(115200);   \
    while (!Serial) {   \
      ; /* wait for serial port to connect. Needed for native USB port only */  \
    }   \
    /* ---- Initialize analog input */  \
    analogReadResolution(12);   \
    /* ---- Initialise LEDs connections */  \
    pinMode(LED_RED, OUTPUT);   \
    pinMode(LED_GREEN, OUTPUT); \
    pinMode(POWER_SERVO_OUT, OUTPUT); \
}



#ifdef __cplusplus
extern "C" {
#endif

float readH();
float readL();
float readServoCurrent();
float readServoPosition();
float calcServoTorque(float I);
void powerServo();
void unpowerServo();
bool canSwitchServoPower();


#ifdef __cplusplus
}
#endif
