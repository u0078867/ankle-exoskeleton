
#pragma once


#include <math.h>
#include <string.h>
#include "exo_glob.h"
#include "exo_common.h"
#include "exo_hwio.h"
#include "exo_mech.h"
#include "exo_imu.h"
//#include <FlashStorage.h>
#include "exo_sett.h"
#include "exo_servo.h"


#ifdef WIFI
#include "exo_wifi.h"
#include "exo_comm.h"
#include "exo_comm_sett.h"
#ifdef RUN_ALGO
#include "exo_comm_algos.h"
#endif
#endif

#ifdef RUN_ALGO
#include "exo_meas.h"
#include "exo_algos.h"
#endif

// --- IMU variables

extern uint8_t imuSystemStatus, imuSelfTestResults, imuSystemError;
extern uint8_t imuCalSystem, imuCalGyro, imuCalAccel, imuCalMag;
extern bool saveImuCalib;
extern bool imuDetected;

extern Adafruit_BNO055 bno;
extern sensors_event_t event;
extern imu::Vector<3> linacc;
//FlashStorage(flashStore, imu_cal_t);

#ifdef WIFI
// --- WiFi variables

extern char ssid[];
extern char pass[];
extern int status;

extern WiFiClient client;
extern WebSocketClient webSocketClient;
#if SAMPLES_STREAM_PROTOCOL == 1
extern WiFiUDP udpClient;
#endif

extern char pckg[][100], msg[];
extern uint8_t p;
extern char dataIn[];
extern uint8_t opcodeIn;
#endif

// --- Timing variables

extern unsigned long offsetTime;
extern unsigned long previousTimeSampling, currentTimeSampling, intervalSampling;
extern unsigned long previousTimeAlgoParamsSend, currentTimeAlgoParamsSend, intervalAlgoParamsSend;
extern unsigned long previousTimeEstim, currentTimeEstim, timeEstim;

extern bool offsetTimeInit;

// --- IMU sampling variable

extern bool imuSensed;

// --- Servo variables

extern Servo servo;
extern bool servoRepowering;
extern unsigned long servoRepoweringT1, servoRepoweringT2;
extern bool servoDetach;
extern float servoPositionTarget;

// --- Analog input variables

extern float servoCurrent,
      servoCurrentOffset,
      servoCurrentOffsetTemp,
      servoTorque,
      servoPosition,
      quadL,
      quadH,
      quadB,
      imuVerticalAngle,
      imuVerticalAngleOffset,
      imuVerticalAngleOffsetTemp,
      shankVerticalAngle,
      ankleAngle,
      ankleAngleOffset,
      ankleAngleOffsetTemp,
      footVerticalAngle;
extern double imuVerticalAcc;

// --- Other state variables

extern float ankleAngleTarget;
extern unsigned int gaitPhase;

// --- Settings variables

extern exo_setts setts;

#ifdef RUN_ALGO
// --- Algorithm variables

extern exo_meas meas;
extern exo_output outs;
extern exo_params params, paramsAuto;
#endif

// --- FSM variables
enum fsm_state {
  ESTIM_SERVO_CURRENT_OFFSET,
  ESTIM_ANGLES_OFFSET,
  EXEC_ALGO,
  MANAGE_SERVO
};
extern fsm_state state;
extern unsigned int contEstim;

#ifdef RUN_ALGO
// --- Safety variables

extern bool mechBlock;
extern unsigned long mechBlockTime;
extern float mechBlockPos;
#endif

// --- Functions

#ifdef WIFI
void sendAllSetts(exo_setts *setts);
#ifdef RUN_ALGO
void sendAllParams(exo_params *params);
#endif
#endif
void gsetup();
void gloop();
