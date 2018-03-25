

#include "exo_hwio.h"


/**************************************************************************/
/*
    Read distance H (mm, ankle angle linear potentiometer)
*/
/**************************************************************************/
float readH() {
  int val;
  double res;
  val = analogRead(ANALOG_IN_LINPOT_ANKLE);
  res = M_AIN_TO_H * val + Q_AIN_TO_H;
  return (float)res;
}

/**************************************************************************/
/*
    Read quadrilateral side L (mm, spring liner potentiometer)
*/
/**************************************************************************/
float readL() {
  int val;
  double res;
  val = analogRead(ANALOG_IN_LINPOT_SPRING);
  res = M_AIN_TO_L * val + Q_AIN_TO_L;
  return (float)res;
}

/**************************************************************************/
/*
    Read servo current (mA)
*/
/**************************************************************************/
float readServoCurrent() {
  int val;
  double res;
  val = analogRead(ANALOG_IN_SERVO_CURRENT);
  res = M_AIN_TO_SERVO_CURRENT * val + Q_AIN_TO_SERVO_CURRENT;
  return (float)res;
}

/**************************************************************************/
/*
    Read servo position (deg)
*/
/**************************************************************************/
float readServoPosition() {
  int val;
  double res;
  val = analogRead(ANALOG_IN_SERVO_POSITION);
  res = M_AIN_TO_SERVO_POSITION * val + Q_AIN_TO_SERVO_POSITION;
  return (float)res;
}

/**************************************************************************/
/*
    Calculate servo torque (Nm)
*/
/**************************************************************************/
float calcServoTorque(float I) {
  double res;
  res = M_SERVO_CURRENT_TO_TORQUE * I + Q_SERVO_CURRENT_TO_TORQUE;
  return (float)res;
}

/**************************************************************************/
/*
    Power servo
*/
/**************************************************************************/
void powerServo() {
    digitalWrite(POWER_SERVO_OUT, HIGH);
}

/**************************************************************************/
/*
    Unpower servo
*/
/**************************************************************************/
void unpowerServo() {
    digitalWrite(POWER_SERVO_OUT, LOW);
}


/**************************************************************************/
/*
    Check if possible to switch servo power
*/
/**************************************************************************/
bool canSwitchServoPower() {
    bool allowed;
    unsigned long callingTime;
    static unsigned long prevCallingTime = 0;
    callingTime = millis();
    allowed = false;
    if ((callingTime - prevCallingTime) > MIN_INTER_SWITCH_TIME) {
        allowed = true;
    } else {
        allowed = false;
    }
    prevCallingTime = callingTime;
    return allowed;
}
