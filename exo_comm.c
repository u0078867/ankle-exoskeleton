
#include "exo_comm.h"


void prepareSamplesMsgOut(
    unsigned long currentTimeSampling,
    float servoTorque,
    float servoPosition,
    float quadL,
    float shankVerticalAngle,
    float ankleAngle,
    float imuVerticalAcc,
    float footVerticalAngle,
    float servoPositionTarget,
    float ankleAngleTarget,
    bool servoDetach,
    unsigned int gaitPhase,
    char *msgOut
) {

    char meast[30];
    // Create 1 sample string
    sprintf(msgOut, "%s", "SAMPLES|");
    // TS
    sprintf(meast, "%7lu", currentTimeSampling);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // MT
    DTOSTRF(servoTorque, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // MP
    DTOSTRF(servoPosition, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // SPC
    DTOSTRF(quadL, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // SVAN
    DTOSTRF(shankVerticalAngle, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // AAN
    DTOSTRF(ankleAngle, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // VAC
    DTOSTRF(imuVerticalAcc, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // FVAN
    DTOSTRF(footVerticalAngle, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // MTP
    DTOSTRF(servoPositionTarget, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // AANT
    DTOSTRF(ankleAngleTarget, 6, 1, meast);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // MDET
    sprintf(meast, "%1u", (unsigned int)servoDetach);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // GP
    sprintf(meast, "%1u", gaitPhase);
    strcat(msgOut, meast);
    strcat(msgOut, SEP);
    // Add final separator
    strcat(msgOut, ";");
}
