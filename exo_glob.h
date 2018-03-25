
#pragma once

// ------- EDITABLE PRE-PROCESSOR DEFINITIONS

/* ------------------------------------------------------------ */
#define SIDE 0                // 0: left; 1: right;
#define SAMPLE_INTERVAL 10    // data sampling/sending interval (ms) (1)
#define ESTIM_SERVO_CURRENT_OFFSET_TIME 5000    // time needed to estimate servo current offset (ms)
#define ESTIM_ANGLES_OFFSET_TIME        5000    // time needed to estimate ankle angle offset (ms)
//#define DEBUG_REAL_SAMPLE_INTERVAL   // uncomment to show real sampling interval (ms) (1)
//#define DEBUG_IMU             // uncomment to show IMU debug prints
//#define DEBUG_SERVO           // uncomment to show servo debug prints
//#define DEBUG_POTENTIOMETERS  // uncomment to show potentiometers debug prints
//#define DEBUG_ANGLES          // uncomment to show angles debug prints
#define WIFI                  // uncomment to use WiFi to send data
#define RUN_ALGO              // uncomment to run the control algorithm
#define IMU_MOUNT_SEGMENT 1   // 0: on shank; 1: on foot
#define IMU_ACC_MODE 2        /* Acceleration sensed by IMU:
                                 0: vertical IMU axis (mounting matters, value varies upon orientation)
                                 1: same as 0, but projected on the sagittal axis (mounting does not matter but orientation needed)
                                 2: total acceleration module (mounting does not matter and orientation not needed, but not able ti distuinguish HS from TO)
                                 3: total acceleration module, with sign given by vertical component (mounting matters, and orientation not needed, but able ti distuinguish HS from TO)
                              */
#define SERVO  2              /* Used servo motor:
                                1: Pololu latching
                                2: Savox non-latching
                              */
#define SERVO_POS_MEASURE   2   /* Way to measure servo curent position:
                                  1: direct (more accurate, less smooth, needs servo hardware tinkering)
                                  2: indirect (less accurate, more smooth, servo cage is preserved)
                              */
#define ENABLE_SERVO_INPUT    // uncomment to enable servo input target position
#define DEF_SETT_SERVOBEH       1
#define DEF_SETT_MECHBLOCK      0
/* ------------------------------------------------------------ */

/*

(1) If the time of one loop() iteration exceeds SAMPLE_INTERVAL,
    DEBUG_REAL_SAMPLE_INTERVAL can be used to debug the real elapsed time.
    If any DEBUG_* is activated, this increases the iteration time.

*/

/* --- Necessary only if WIFI is defined: --------------------- */
#define WIFI_SSID "Linksys-MIRAD"   // WiFi network name
#define WIFI_PASSWORD "ycc885cM_"   // WiFi password
#define IP_SERVER "192.168.1.124"   // IP address of server
#define PORT_SERVER 5001            // port for connection to websocket server
#define PORT_UDP_SERVER 3333        // port for connection to UDP server
#define DEBUG_WIFI                  // uncomment to show WiFi debug prints
#define PACKAGES_LENGTH 1           // number of samples to pack before sending via WiFi
#define SAMPLES_STREAM_PROTOCOL 1   // 0: WebSocket; 1: UDP
/* ------------------------------------------------------------ */

/* --- Necessary only if RUN_ALGO is defined: ----------------- */
#define ALGO 1              /* Number of the algorithm to run:
                               0: track AAN model with motor always active
                               1: normal algorithm
                            */
//#define DEBUG_ALGO                        // uncomment to show angles debug prints
//#define DETACH_SERVO_ON_MECH_LIMITS       // comment to avoid detaching motor when motor shaft reaches mechanical limits
//#define DETACH_SERVO_ON_CURRENT_LIMITS    // comment to avoid detaching motor when motor drags to much current
#define SUBJECT_TYPE 1      /* Subject type:
                               1: normal child
                               2: normal child emulating CP with flat foot walking
                               3: very slow normal child (for debugging only)
                            */
#define AAN_MODEL    4      /* Ankle angle model type:
                               1: linear with fixed target MAD/MAP/MASW (manually set, useful for debugging);
                                  beginning point of each line is variable, target is fixed.
                               2: linear with fixed slopes;
                                  intercept of each line is variable, slope is fixed.
                               3: linear with adaptive MAD/MAP/MASW
                               4: mixed 1:
                                    2nd rocker: fixed slope
                                    3rd rocker: fixed dt, adaptive dy (bounded MAP)
                                    swing (dorsiflexing): fixed dt, adaptive dy
                            */
#define GAMMA_TO_TETA_DELTA   3   /* How to calculate variation of motor target from variation of ankle angle target
                                    1: raw formula with no optimization:
                                        - slow: sub-expressions are recalculated every time; this can make calculation last even 8-9 ms;
                                        - math is safe;
                                        - best accuracy;
                                    2: optimized formula where consant terms (A, C) or little varying terms (B) are hardcoded and pre-calculated:
                                        - fast: expression is much shorter than (1);
                                        - math is safe;
                                        - accuracy is sub-optimal; this should not create problems though;
                                    3: expression from 1 is CSE-ed with external tools, and GCC is set with -Ofast for the expression only:
                                        - fast: with respect to (1), speed increases significantly;
                                        - math *might* be unsafe; however, particular conditions of nan or inf are checked;
                                        - best accuracy;
                                    4: constant value, must be set by ALGO_PID_KP.
                                  */

#define ALGO_PID_KP  -0.9         // Manual tuning of proportional constant for PID (ignored if GAMMA_TO_TETA_DELTA <= 3)
#define ALGO_PID_KI  -0.01        // Manual tuning of integral constant for PID
#define ALGO_PID_KD   0.0         // Manual tuning of derivative constant for PID

/*
    --- test on bench:
    ALGO 0, SUBJECT_TYPE 3, AAN_MODEL 1
    --- test on subject:
    ALGO 1, SUBJECT_TYPE 1, AAN_MODEL 4
*/
#if SUBJECT_TYPE == 1
#define DEF_PAR_ALGMOD      1
#define DEF_PAR_FVANTHS     20.
#define DEF_PAR_VACTHS      15.
#define DEF_PAR_FVANTTO     -30.
#define DEF_PAR_VACTTO      10.
#define DEF_PAR_HOMOD       4
#define DEF_PAR_FVANTHO     -5.
#define DEF_PAR_SVANTHO     10.
#define DEF_PAR_TWNOHO      300
#define DEF_PAR_E2R         500
#define DEF_PAR_E3R         700
#define DEF_PAR_E1SW        800
#define DEF_PAR_MAD         15.
#define DEF_PAR_MAP         -15.
#define DEF_PAR_MASW        0.
#define DEF_PAR_AANE        0.
#define DEF_PAR_ST          1000
#elif SUBJECT_TYPE == 2
#define DEF_PAR_ALGMOD      1
#define DEF_PAR_FVANTHS     0.
#define DEF_PAR_VACTHS      25.
#define DEF_PAR_FVANTTO     -20.
#define DEF_PAR_VACTTO      15.
#define DEF_PAR_HOMOD       4
#define DEF_PAR_FVANTHO     -5.
#define DEF_PAR_SVANTHO     10.
#define DEF_PAR_TWNOHO      300
#define DEF_PAR_E2R         500
#define DEF_PAR_E3R         700
#define DEF_PAR_E1SW        800
#define DEF_PAR_MAD         15.
#define DEF_PAR_MAP         -15.
#define DEF_PAR_MASW        0.
#define DEF_PAR_AANE        0.
#define DEF_PAR_ST          1000
#elif SUBJECT_TYPE == 3
#define DEF_PAR_ALGMOD      1
#define DEF_PAR_FVANTHS     20.
#define DEF_PAR_VACTHS      25.
#define DEF_PAR_FVANTTO     -30.
#define DEF_PAR_VACTTO      15.
#define DEF_PAR_HOMOD       4
#define DEF_PAR_FVANTHO     -5.
#define DEF_PAR_SVANTHO     10.
#define DEF_PAR_TWNOHO      600
#define DEF_PAR_E2R         1000
#define DEF_PAR_E3R         1400
#define DEF_PAR_E1SW        1600
#define DEF_PAR_MAD         15.
#define DEF_PAR_MAP         -15.
#define DEF_PAR_MASW        0.
#define DEF_PAR_AANE        0.
#define DEF_PAR_ST          2000
#elif SUBJECT_TYPE == 4
#define DEF_PAR_ALGMOD      1
#define DEF_PAR_FVANTHS     20.
#define DEF_PAR_VACTHS      15.
#define DEF_PAR_FVANTTO     -30.
#define DEF_PAR_VACTTO      15.
#define DEF_PAR_HOMOD       4
#define DEF_PAR_FVANTHO     -5.
#define DEF_PAR_SVANTHO     10.
#define DEF_PAR_TWNOHO      300
#define DEF_PAR_E2R         500
#define DEF_PAR_E3R         700
#define DEF_PAR_E1SW        800
#define DEF_PAR_MAD         15.
#define DEF_PAR_MAP         -10.
#define DEF_PAR_MASW        0.
#define DEF_PAR_AANE        0.
#define DEF_PAR_ST          1000
#endif
/* ------------------------------------------------------------ */

/* --- Necessary only if WIFI and RUN_ALGO are defined: ------- */
//#define SEND_ALGO_PARAMS    // uncomment to enable auto-send of algorithm params after an interval
#define SEND_ALGO_PARAMS_INTERVAL 5000   // interval (ms) after which to send algorithm params to websocket server
/* ------------------------------------------------------------ */
