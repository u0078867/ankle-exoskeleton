
#pragma once

#include "exo_common.h"
#include <stddef.h>

#define SEP ","


#ifdef __cplusplus
extern "C" {
#endif


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
);


#ifdef __cplusplus
}
#endif
