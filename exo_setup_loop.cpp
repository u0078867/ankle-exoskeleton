
#include "exo_setup_loop.h"


// --- IMU variables

uint8_t imuSystemStatus=0, imuSelfTestResults=0, imuSystemError=0;
uint8_t imuCalSystem=0, imuCalGyro=0, imuCalAccel=0, imuCalMag=0;
bool saveImuCalib = true;
bool imuDetected = true;

Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B);
sensors_event_t event;
imu::Vector<3> linacc;
FlashStorage(flashStore, imu_cal_t);

#ifdef WIFI
// --- WiFi variables

char ssid[] = WIFI_SSID;            // your network SSID (name)
char pass[] = WIFI_PASSWORD;        // your network password
int status = WL_IDLE_STATUS;        // the Wifi radio's status

WiFiClient client;
WebSocketClient webSocketClient;
#if SAMPLES_STREAM_PROTOCOL == 1
WiFiUDP udpClient;
#endif

char pckg[PACKAGES_LENGTH][100], msg[40];
uint8_t p = 0;
char dataIn[100];
uint8_t opcodeIn;
#endif

// --- Timing variables

unsigned long offsetTime=0;
unsigned long previousTimeSampling=0, currentTimeSampling=0, intervalSampling=0;
#ifdef WIFI
#ifdef RUN_ALGO
#ifdef SEND_ALGO_PARAMS
unsigned long previousTimeAlgoParamsSend=0, currentTimeAlgoParamsSend=0, intervalAlgoParamsSend=0;
#endif
#endif
#endif
unsigned long previousTimeEstim=0, currentTimeEstim=0, timeEstim=0;

bool offsetTimeInit = false;

// --- IMU sampling variable

bool imuSensed = false;

// --- Servo variables

Servo servo;
bool servoRepowering = false;
unsigned long servoRepoweringT1 = 0, servoRepoweringT2 = 0;
bool servoDetach = true;
float servoPositionTarget = 0.0;

// --- Analog input variables

float servoCurrent,
      servoCurrentOffset = 0.0,
      servoCurrentOffsetTemp = 0.0,
      servoTorque,
      servoPosition,
      quadL,
      quadH,
      quadB,
      imuVerticalAngle,
      imuVerticalAngleOffset = 0.0,
      imuVerticalAngleOffsetTemp = 0.0,
      shankVerticalAngle,
      ankleAngle,
      ankleAngleOffset = 0.0,
      ankleAngleOffsetTemp = 0.0,
      footVerticalAngle;
double imuVerticalAcc;

// --- Other state variables

float ankleAngleTarget = 0.0;
unsigned int gaitPhase = 0;

// --- Settings variables

exo_setts setts;

#ifdef RUN_ALGO
// --- Algorithm variables

exo_meas meas;
exo_output outs;
exo_params params, paramsAuto;
#endif

// --- FSM variables

fsm_state state = ESTIM_SERVO_CURRENT_OFFSET;
unsigned int contEstim = 0;

// --- Safety variables

bool mechBlock = false;
unsigned long mechBlockTime;
float mechBlockPos;


// --- Functions

#ifdef WIFI
void sendAllSetts(exo_setts *setts) {
    char msg[40];
    bool m = true;
    prepareSettsMsgOut("MECHBLOCK", "R", "", setts, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareSettsMsgOut("SERVOBEH", "R", "", setts, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
}
#ifdef RUN_ALGO
void sendAllParams(exo_params *params) {
    char msg[40];
    bool m = true;
    prepareParamsMsgOut("ALGMOD", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("FVANTHS", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("VACTHS", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("FVANTTO", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("VACTTO", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("HOMOD", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("FVANTHO", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("SVANTHO", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("TWNOHO", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("MAD", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("MAP", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("MASW", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("E2R", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("E3R", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("E1SW", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("AANE", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
    prepareParamsMsgOut("ST", "R", "", params, msg);
    webSocketClient.sendData(msg, WS_OPCODE_TEXT, m);
}
#endif
#endif


/**************************************************************************/
/*
    Setup function (automatically called at startup)
*/
/**************************************************************************/
void gsetup(void)
{
  // ---- Initialize hardware

  HW_INIT();

  // ---- Initialise the IMU sensor

  // P8: suitable for back-mounting, point up and facing back
  // P9: suitable for left-mounting, point in the left-up corner, if you look at the IMU from top. Point towards foot tip
  // P10: suitable for right-mounting, point in the left-up corner, if you look at the IMU from top. Point towards calcaneus

#if(SIDE)
  imuDetected = bno.begin(bno.OPERATION_MODE_NDOF, bno.REMAP_CONFIG_P10, bno.REMAP_SIGN_P10);
#else
  imuDetected = bno.begin(bno.OPERATION_MODE_NDOF, bno.REMAP_CONFIG_P9, bno.REMAP_SIGN_P9);
#endif

  if(!imuDetected)
  {
    /* There was a problem detecting the BNO055 ... check your connections */
#ifdef DEBUG_IMU
    DEBUG_PRINTLN("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
#endif
    while(1);
  }
  DEBUG_PRINTLN("BNO055 detected");
  DELAY(1000);

  // Set calibration data from Flash if existing
  getImuCalibFromFlashZERO(bno, flashStore);

#ifdef DEBUG_IMU
  // Display some basic information on this sensor
  displaySensorDetails(bno);
#endif

  // Get sensor status
  getImuStatus(bno, &imuSystemStatus, &imuSelfTestResults, &imuSystemError);

  // Set external crystal
  bno.setExtCrystalUse(true);

  // ---- Initialize settings
  setts.MECHBLOCK = DEF_SETT_MECHBLOCK;
  setts.SERVOBEH = DEF_SETT_SERVOBEH;

#ifdef RUN_ALGO
  // ---- Initialize algorithm outputs
  outs.GP = 0;
  //outs.GP = 5;
  outs.MTP = 90.;   // safe position
  outs.MDET = true;
  outs.AANT = 0;
  // ---- Initialize algorithm params
  params.ALGMOD = DEF_PAR_ALGMOD;
  params.FVANTHS = DEF_PAR_FVANTHS;
  params.VACTHS = DEF_PAR_VACTHS;
  params.FVANTTO = DEF_PAR_FVANTTO;
  params.VACTTO = DEF_PAR_VACTTO;
  params.HOMOD = DEF_PAR_HOMOD;
  params.FVANTHO = DEF_PAR_FVANTHO;
  params.SVANTHO = DEF_PAR_SVANTHO;
  params.TWNOHO = DEF_PAR_TWNOHO;
  params.AANE1R = 0.;
  params.E1R = 0;
  params.E2R = DEF_PAR_E2R;
  params.E3R = DEF_PAR_E3R;
  params.E1SW = DEF_PAR_E1SW;
  params.MAD = DEF_PAR_MAD;
  params.MAP = DEF_PAR_MAP;
  params.MASW = DEF_PAR_MASW;
  params.AANE = DEF_PAR_AANE;
  params.ST = DEF_PAR_ST;
  // ---- Copy manual params to the automatic params
  memcpy(&paramsAuto, &params, sizeof(paramsAuto));
#endif

  // ---- Initialize WiFi

#ifdef WIFI
  // Check for the presence of the WiFi101 shield:
  if (WiFi.status() == WL_NO_SHIELD) {
#ifdef DEBUG_WIFI
    DEBUG_PRINTLN("WiFi shield not present");
#endif
    // don't continue:
    while (true);
  }

  // Attempt to connect to Wifi network
  while (status != WL_CONNECTED) {
#ifdef DEBUG_WIFI
    DEBUG_PRINT("Attempting to connect to WPA SSID: ");
    DEBUG_PRINTLN(ssid);
#endif
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    //status = WiFi.begin(ssid);
    // wait 1 second for connection
    DELAY(5000);
  }

#ifdef DEBUG_WIFI
  // Display connection info
  DEBUG_PRINT("You're connected to the network");
  displayCurrentNet();
  displayWifiData();
#endif

  // Connect to the websocket server
#ifdef DEBUG_WIFI
  DEBUG_PRINT("Attempting to connect to host on port ");
  DEBUG_PRINT(PORT_SERVER);
  DEBUG_PRINTLN(" ...");
#endif
  while (!client.connect(IP_SERVER, PORT_SERVER)) {
#ifdef DEBUG_WIFI
    DEBUG_PRINTLN("Connection failed. Retrying ...");
#endif
  }
#ifdef DEBUG_WIFI
  DEBUG_PRINTLN("Connected");
  DEBUG_PRINTLN();
#endif

  // Handshake with the server
  webSocketClient.path = "/";
#ifdef DEBUG_WIFI
  DEBUG_PRINTLN("Handshaking connection to WebSocket server ...");
#endif
  while (!webSocketClient.handshake(client)) {
#ifdef DEBUG_WIFI
    DEBUG_PRINTLN("Handshake failed. Retrying ...");
#endif
  }
#ifdef DEBUG_WIFI
  DEBUG_PRINTLN("Handshake successful");
#endif

#if SAMPLES_STREAM_PROTOCOL == 1
  // Begin listening for UDP socket
  udpClient.begin(PORT_UDP_SERVER);
#endif

  // Send settings
  sendAllSetts(&setts);

#ifdef RUN_ALGO
  // Send parameters
  sendAllParams(&params);
#endif

#endif

/*
 * NOTE:
 * The websocket handshaking with server shoud be the last thing before
 * the loop(). This because we suppose that the moment just after the
 * handshaking is the common start taming for both server and client.
 * If we have the common statring point, then we can estimate the high
 * frequency data transmission delay.
 *
 */

}

/**************************************************************************/
/*
    Loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void gloop()
{

  // Initialize previous time
  if (!offsetTimeInit) {
    offsetTime = MILLIS();
    previousTimeSampling = offsetTime;
#ifdef WIFI
#ifdef RUN_ALGO
#ifdef SEND_ALGO_PARAMS
    previousTimeAlgoParamsSend = offsetTime;
#endif
#endif
#endif
    previousTimeEstim = offsetTime;
    offsetTimeInit = true;
  }

#ifdef WIFI
  // Get data from server if existing
  if (webSocketClient.getData(dataIn, &opcodeIn)) {
    if (opcodeIn == WS_OPCODE_TEXT) {
      size_t L = strlen(dataIn);
      DEBUG_PRINT("Received data: ");
      DEBUG_PRINTLN(dataIn);
      if (L > 0) {
        const char s[2] = "|";
        char *token, ctx[10];
        token = strtok(dataIn, s);
        strcpy(ctx, token);
        if (strcmp(ctx, "ALGOPAR") == 0) {
#ifdef RUN_ALGO
            // Parse parameters message
            char param[10], action[10], value[20];
            parseParamsMsgIn(dataIn+strlen(ctx)+strlen(s), param, action, value);
            if (strcmp(action, "W") == 0) {
                // Update manual parameter
                updateParam(param, value, &params);
                // Update auto parameter
                updateParam(param, value, &paramsAuto);
            }
            // Prepare answer
            prepareParamsMsgOut(param, action, value, &params, msg);
#endif
        } else if (strcmp(ctx, "SETT") == 0) {
            // Parse settings message
            char sett[10], action[10], value[20];
            parseSettsMsgIn(dataIn+strlen(ctx)+strlen(s), sett, action, value);
            if (strcmp(action, "W") == 0) {
                // Update setting
                updateSett(sett, value, &setts);
            }
            // Prepare answer
            prepareSettsMsgOut(sett, action, value, &setts, msg);
        }
        // Send message back
        DEBUG_PRINT("Answer: ");
        DEBUG_PRINTLN(msg);
        webSocketClient.sendData(msg);
        DEBUG_PRINTLN("sent!");
      }
    }
    if (opcodeIn == WS_OPCODE_PING) {
      DEBUG_PRINTLN("ping received. ponging ...");
      webSocketClient.sendData(dataIn, WS_OPCODE_PONG);
    }
  }
#endif

  // Get measures from IMU
  if (!imuSensed) {

    // Get IMU calibration status
    getImuCalStatus(bno, &imuCalSystem, &imuCalGyro, &imuCalAccel, &imuCalMag);

    if (bno.isFullyCalibrated()) {
      if (saveImuCalib) {
        // Save IMU calibration data
        setImuCalibToFlashZERO(bno, flashStore);
        saveImuCalib = false;
      }
    } else {
      saveImuCalib = true;
    }

    /*
    if (imuCalSystem == 0 || imuCalGyro == 0 || imuCalAccel == 0 || imuCalMag == 0) {
      LED_RED_ON();
    } else {
      LED_RED_OFF();
    }
    */

    // Get a new IMU event
    bno.getEvent(&event);

    // Get IMU vertical angle
    imuVerticalAngle = event.orientation.y;

#ifdef DEBUG_IMU
    // Display the floating point data
    DEBUG_PRINT("euler.X: ");
    DEBUG_PRINT(event.orientation.x, 4);
    DEBUG_PRINT("\teuler.Y: ");
    DEBUG_PRINT(event.orientation.y, 4);
    DEBUG_PRINT("\teuler.Z: ");
    DEBUG_PRINT(event.orientation.z, 4);
#endif

#if(SIDE)
    // Adjust some signals
    event.orientation.z *= -1.;
#endif

    // Get linear acceleration
    linacc = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    // Get IMU vertical acceleration
#if IMU_ACC_MODE == 0
    imuVerticalAcc = linacc.z();
#elif IMU_ACC_MODE == 1
    imuVerticalAcc = linacc.z() * cos(DEG_TO_RAD * imuVerticalAngle) * cos(DEG_TO_RAD * event.orientation.z);
#elif IMU_ACC_MODE == 2
    imuVerticalAcc = sqrt(pow(linacc.z(), 2) + pow(linacc.x(), 2) + pow(linacc.y(), 2));
#elif IMU_ACC_MODE == 3
        imuVerticalAcc = ((linacc.z() > 0) ? +1: -1) * sqrt(pow(linacc.z(), 2) + pow(linacc.x(), 2) + pow(linacc.y(), 2));
#endif

#ifdef DEBUG_IMU
    // Display linear acceleration data
    DEBUG_PRINTLN("");
    DEBUG_PRINT("lacc.X: ");
    DEBUG_PRINT(linacc.x(), 4);
    DEBUG_PRINT("\tlacc.Y: ");
    DEBUG_PRINT(linacc.y(), 4);
    DEBUG_PRINT("\tlacc.Z: ");
    DEBUG_PRINT(linacc.z(), 4);
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("");
#endif

    imuSensed = true;

  }

#ifdef WIFI
#ifdef RUN_ALGO
#ifdef SEND_ALGO_PARAMS
  // Measure time interval for algorithm parameters sending
  currentTimeAlgoParamsSend = MILLIS() - offsetTime;
  intervalAlgoParamsSend = currentTimeAlgoParamsSend - previousTimeAlgoParamsSend;

  // Send current algorithm params
  if(intervalAlgoParamsSend >= SEND_ALGO_PARAMS_INTERVAL) {

    // Reset previous time
    previousTimeAlgoParamsSend = currentTimeAlgoParamsSend;

    // Send parameters
    sendAllParams(&params);

  }
#endif
#endif
#endif

  // Get servo current
  servoCurrent = readServoCurrent() - servoCurrentOffset;

  // Get servo torque
  servoTorque = calcServoTorque(servoCurrent);

  // Get servo position
#if SERVO_POS_MEASURE == 1
  servoPosition = readServoPosition();
#else SERVO_POS_MEASURE == 2
  servoPosition = 0.;
#endif

  // Get spring compression
  quadL = readL();

  // Get quadrilateral side B
  quadH = readH();
  quadB = calcB_H(quadH);

  // Get ankle angle
  ankleAngle = calcGAMMA(quadH, quadB) - ankleAngleOffset;

#if IMU_MOUNT_SEGMENT == 0
    // Get shank vertical angle
    shankVerticalAngle = imuVerticalAngle - imuVerticalAngleOffset;

    // Get foot vertical angle
    footVerticalAngle = shankVerticalAngle - ankleAngle;
#else
    // Get foot vertical angle
    footVerticalAngle = imuVerticalAngle - imuVerticalAngleOffset;

    // Get shank vertical angle
    shankVerticalAngle = -footVerticalAngle + ankleAngle;
#endif

  // Manage FSM state
  float c;
  switch(state) {

      case ESTIM_SERVO_CURRENT_OFFSET:
        c = (float)contEstim;
        servoCurrentOffsetTemp = servoCurrentOffsetTemp * (c / (c+1.)) + servoCurrent * (1. / (c+1.));
        contEstim++;
        currentTimeEstim = MILLIS() - offsetTime;
        timeEstim = currentTimeEstim - previousTimeEstim;
        if (timeEstim > ESTIM_SERVO_CURRENT_OFFSET_TIME) {
          state = ESTIM_ANGLES_OFFSET;
          previousTimeEstim = currentTimeEstim;
          contEstim = 0;
          servoCurrentOffset = servoCurrentOffsetTemp;
        }
        powerServo();
        break;

      case ESTIM_ANGLES_OFFSET:
        c = (float)contEstim;
        ankleAngleOffsetTemp = ankleAngleOffsetTemp * (c / (c+1.)) + ankleAngle * (1. / (c+1.));
        imuVerticalAngleOffsetTemp = imuVerticalAngleOffsetTemp * (c / (c+1.)) + imuVerticalAngle * (1. / (c+1.));
        contEstim++;
        currentTimeEstim = MILLIS() - offsetTime;
        timeEstim = currentTimeEstim - previousTimeEstim;
        if (timeEstim > ESTIM_ANGLES_OFFSET_TIME) {
          state = EXEC_ALGO;
          previousTimeEstim = currentTimeEstim;
          contEstim = 0;
          ankleAngleOffset = ankleAngleOffsetTemp;
          imuVerticalAngleOffset = imuVerticalAngleOffsetTemp;
        }
        break;

      case EXEC_ALGO:
#ifdef RUN_ALGO
          // Prepare structs for algorithm
          meas.TS = currentTimeSampling;
          meas.MT = servoTorque;
          meas.MP = servoPosition;
          meas.SPC = quadL;
          meas.SVAN = shankVerticalAngle;
          meas.AAN = ankleAngle;
          meas.AANG = ankleAngleOffset;
          meas.FVAN = footVerticalAngle;
          meas.VAC = imuVerticalAcc;
          meas.QUADB = quadB;

          // Select model
          float (*AANModel)(unsigned long, exo_params *, exo_params *, exo_output *);
#if AAN_MODEL == 1
          AANModel = &AANMLinearFixedValues;
#elif AAN_MODEL == 2
          AANModel = &AANMLinearFixedSlopes;
#elif AAN_MODEL == 3
          AANModel = &AANMLinearAdaptive;
#elif AAN_MODEL == 4
          AANModel = &AANMLinearMixed1;
#endif

          // Run algorithm
#if ALGO == 0
          //DEBUG_PRINTLN("algo0");
          algoTrackAANM(&meas, &outs, &params, &paramsAuto, AANModel);
#elif ALGO == 1
          //DEBUG_PRINTLN("algo1");
          algo1(&meas, &outs, &params, &paramsAuto, AANModel);
#endif
          servoDetach = outs.MDET;
          servoPositionTarget = outs.MTP;
          gaitPhase = outs.GP;
          ankleAngleTarget = outs.AANT;
#endif  // #ifdef RUN_ALGO

      case MANAGE_SERVO:
#ifdef DETACH_SERVO_ON_MECH_LIMITS
        // Important check on mechanical limits
        if (servoPositionTarget < SERVO_MIN_POS || servoPositionTarget > SERVO_MAX_POS) {
          servoDetach = true;
        }
#endif

#ifdef DETACH_SERVO_ON_CURRENT_LIMITS
        // Important check on servo current limit
        if (servoCurrent > SERVO_CURRENT_MAX) {
          servoDetach = true;
        }
#endif

        if (servoCurrent > SERVO_CURRENT_MAX) {
          LED_RED_ON();
        } else {
          LED_RED_OFF();
        }

        if (setts.MECHBLOCK == 1) {
            /* NOTES:
              - these checks are valid for motors both increasing or decreasing
                position while dorsiflexing;
              - when the relay is switched off, meas.MP equals Q_AIN_TO_SERVO_POSITION
                that is normally negative; a check for positivity is therefore necessary;
              - tests on ankle angle are necessary, since the ones on motor position
                make sense only if the servo is powered and its position can be measured
              - it turned out that the second check has to be commented: when the motor
                is unpowered, the signal does not drop suddenly below 0, but there is
                a few instants where it is between zero and SERVO_MIN_POS. This continously
                turns on and off the motor. Commenting the second block is ok for the right
                side orthosis, not for the left one. I conclude that it is better to test
                only the ankle angle extremes: their measure is continous.
            */
            /*if (servoPosition > 0 && servoPosition > (SERVO_MAX_POS)) {
              mechBlock = true;
              mechBlockPos = SERVO_MAX_BLOCK_POS;
              mechBlockTime = MILLIS();
          }*/
            /*if (servoPosition > 0 && servoPosition < (SERVO_MIN_POS)) {
              mechBlock = true;
              mechBlockPos = SERVO_MIN_BLOCK_POS;
              mechBlockTime = MILLIS();
            }*/
            if (ankleAngle > AAN_MAX_POS) {
                mechBlock = true;
                mechBlockPos = AAN_MAX_BLOCK_POS;
                mechBlockTime = MILLIS();
            }
            if (ankleAngle < AAN_MIN_POS) {
                mechBlock = true;
                mechBlockPos = AAN_MIN_BLOCK_POS;
                mechBlockTime = MILLIS();
            }
            if (mechBlock) {
              servoDetach = false;
              servoPositionTarget = mechBlockPos;
              if ((MILLIS() - mechBlockTime) > 50) {
                servoDetach = true;
                mechBlock = false;
              }
            }
        }

        /*
        // Store output values
        gaitPhase = outs.GP;
        servoPositionTarget = outs.MTP;
        ankleAngleTarget = outs.AANT;
        */

#ifdef ENABLE_SERVO_INPUT
        // Manage servo repowering (if needed)
        if (setts.SERVOBEH == 1) {
          if (servoRepowering) {
            servoRepoweringT2 = MILLIS();
            if (servoRepoweringT2 - servoRepoweringT1 > 20) {
              powerServo();
              servoRepowering = false;
            }
          }
        }

        // Manage servo
        if (!servoRepowering) {
            if (servoDetach) {
              if (servo.attached()) {
                  servo.detach();
                  if (setts.SERVOBEH == 1 || setts.SERVOBEH == 3) {
                    unpowerServo();
                  }
                  if (setts.SERVOBEH == 1) {
                    servoRepowering = true;
                    servoRepoweringT1 = MILLIS();
                  }
              }
            } else {
              if (!servo.attached()) {
                  if (setts.SERVOBEH == 3) {
                    powerServo();
                  }
                  servo.attach(PWM_OUT_SERVO);
              }
              servo.write((int)(servoPositionTarget+0.5));
            }
        }
#endif

        // Manage LED
        if (servoDetach) {
          LED_GREEN_OFF();
        } else {
          LED_GREEN_ON();
        }

  } // switch()

#ifdef DEBUG_SERVO
  DEBUG_PRINT("Servo current (mA): ");
  DEBUG_PRINT(servoCurrent);
  DEBUG_PRINT("; Servo current offset (mA): ");
  DEBUG_PRINT(servoCurrentOffset);
  DEBUG_PRINT("; Servo torque (Nm): ");
  DEBUG_PRINT(servoTorque);
  DEBUG_PRINT("; Servo position (deg): ");
  DEBUG_PRINT(servoPosition);
  DEBUG_PRINT("; Servo attached: ");
  DEBUG_PRINT(servo.attached());
  DEBUG_PRINTLN();
#endif

#ifdef DEBUG_POTENTIOMETERS
  DEBUG_PRINT("Quad L (mm): ");
  DEBUG_PRINT(quadL);
  DEBUG_PRINT("; Quad H (mm): ");
  DEBUG_PRINT(quadH);
  DEBUG_PRINTLN();
#endif

#ifdef DEBUG_ANGLES
  DEBUG_PRINT("IMU vertical angle (deg): ");
  DEBUG_PRINT(imuVerticalAngle);
  DEBUG_PRINT("; IMU vertical angle offset (deg): ");
  DEBUG_PRINT(imuVerticalAngleOffset);
  DEBUG_PRINT("; Shank vertical angle (deg): ");
  DEBUG_PRINT(shankVerticalAngle);
  DEBUG_PRINT("; Ankle angle (deg): ");
  DEBUG_PRINT(ankleAngle);
  DEBUG_PRINT("; Ankle angle offset (deg): ");
  DEBUG_PRINT(ankleAngleOffset);
  DEBUG_PRINT("; Foot vertical angle (deg): ");
  DEBUG_PRINT(footVerticalAngle);
  DEBUG_PRINTLN();
#endif

#ifdef RUN_ALGO
#ifdef DEBUG_ALGO
  DEBUG_PRINT("Servo target position (deg): ");
  DEBUG_PRINT(servoPositionTarget);
  DEBUG_PRINT("; Ankle traget angle (deg): ");
  DEBUG_PRINT(ankleAngleTarget);
  DEBUG_PRINT("; Gait phase: ");
  DEBUG_PRINT(gaitPhase);
  DEBUG_PRINTLN();
#endif
#endif

  // Measure time interval for data sampling/sending
  currentTimeSampling = MILLIS() - offsetTime;
  intervalSampling = currentTimeSampling - previousTimeSampling;

  if(intervalSampling >= SAMPLE_INTERVAL) {

    // Reset previous time (should be very close to where current time is estimated)
    previousTimeSampling = currentTimeSampling;
    imuSensed = false;

#ifdef DEBUG_REAL_SAMPLE_INTERVAL
    DEBUG_PRINT("Real sampling interval: ");
    DEBUG_PRINTLN(intervalSampling);
#endif

#ifdef WIFI

    // Prepare samples message
    prepareSamplesMsgOut(
      currentTimeSampling,
      servoTorque,
      servoPosition,
      quadL,
      shankVerticalAngle,
      ankleAngle,
      imuVerticalAcc,
      footVerticalAngle,
      servoPositionTarget,
      ankleAngleTarget,
      servoDetach,
      gaitPhase,
      pckg[p]
    );

    // Increment package counter
    p++;
#endif

  }

#ifdef WIFI
    // Send data over WiFi
    if(p == PACKAGES_LENGTH) {
        for(uint8_t i = 0; i < p; i++) {
#if SAMPLES_STREAM_PROTOCOL == 0
            //webSocketClient.sendData("SAMPLES|xxxxxxx,x,x,x,x,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,x;");
            //webSocketClient.sendData("SAMPLES|xxxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,xxxxxx,x,x;");
            webSocketClient.sendData(pckg[i]);
#elif SAMPLES_STREAM_PROTOCOL == 1
            int ok1 = udpClient.beginPacket(IP_SERVER, PORT_UDP_SERVER);
            int nb = udpClient.write(pckg[i], strlen(pckg[i]));
            int ok2 = udpClient.endPacket();
#endif
        }
        p = 0;
    }
#endif

}
