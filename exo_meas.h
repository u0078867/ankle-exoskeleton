
/**********************

-- NOMENCLATURE:

Measures:

- TS: time (ms from uC start)
- MT: motor torque (Nm)
- MP: motor position (deg, shaft up positive)
- SPC: spring compression (mm, compression positive)
- SVAN: shank angle with vertical (deg, forward inclination positive)
- AAN: ankle angle (deg, dorsiflexion positive)
- AANG: ankle angle to gamma angle (deg)
- FVAN: foot angle with vertical (deg, toe down positive)
- VAC: IMU vertical acceleration (m/s2, up positive)
- QUADB: distance between motor shaft and ankle axis shaft (mm)

**********************/


#pragma once


typedef struct {
    unsigned long TS;
    float MT;
    float MP;
    float SPC;
    float SVAN;
    float AAN;
    float AANG;
    float FVAN;
    double VAC;
    float QUADB;
} exo_meas;



#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif
