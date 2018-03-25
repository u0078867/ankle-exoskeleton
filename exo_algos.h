
/**********************

-- NOMENCLATURE:

Outputs:

- MTP: motor target position (deg, see MP)
- AANT: target ankle angle (deg, see AAN)
- GP: gait phase:
    - 0: undefined
    - 1: 1st rocker
    - 2: 2nd rocker
    - 3: 3rd rocker
    - 4: swing (dorsiflexing)
    - 5  swing (dorsiflexed)
- MDET: detach motor (boolean)

Parameters:

- ALGMOD: algorithm mode (0: idle; 1: gait)
- FVANTHS: threshold on FVAN to detect heel-strike
- VACTHS: threshold on VAC to detect heel-strike
- FVANTTO: threshold on FVAN to detect toe-off
- VACTTO: threshold on VAC to detect toe-off
- HOMOD: HO detection mode (1: FVAN/AAN-based; 2: SVAN-based)
- FVANTHO: threshold on FVAN to detect heel-off
- SVANTHO: threshold on SVAN to detect heel-off
- TWNOHO: time window after HS to ignore HO detection (ms)
- MAD: max ankle dorsiflexion angle at end of 2nd rocker (deg, see AAN)
- MAP: max ankle plantarflexion at end of 3rd rocker (deg, see AAN)
- MASW: min ankle during swing (deg, see AAN)
- ST: step time (ms, from heel-strike)
- TOP: Toe-off (% ST)
- TO: Toe-off (ms, from heel-strike)
- E1RP: End of 1st rocker (% ST)
- E1R: end of 1st rocker (ms, from heel-strike)
- E2RP: End of 2nd rocker (% ST)
- E2R: end of 2nd rocker (ms, from heel-strike)
- E3RP: End of 3rd rocker (% ST)
- E3R: end of 3rd rocker (ms, from heel-strike)
- E1SWP: end of 1st part of swing phase with fast dorsiflexion (% ST)
- E1SW: end of 1st part of swing phase with fast dorsiflexion (ms, from heel-strike)
- AANS: starting ankle angle (deg, at heel-strike, see AAN)
- AAN1RP: ankle angle at the start of 1st rocker (deg, at heel-strike, see AAN)
- MTPE: maximum allowed arror between motor target position and current position (deg, see MP)
- AANE: maximum allowed arror between motor target position and current position (deg, see AAN)

**********************/

#pragma once


#include "exo_common.h"
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "exo_glob.h"
#include "exo_mech.h"
#include "exo_meas.h"
#include "exo_events.h"
#include "exo_pid.h"
#if GAMMA_TO_TETA_DELTA == 1
#include "gamma_to_teta_delta.h"
#elif GAMMA_TO_TETA_DELTA == 2
#include "gamma_to_teta_delta_fast.h"
#elif GAMMA_TO_TETA_DELTA == 3
#include "gamma_to_teta_delta_cse_ofast.h"
#endif

#if SERVO_POS_MEASURE == 2
#include "gamma_to_teta_theor.h"
#endif


/*
#ifdef __cplusplus
extern "C" {
#endif
*/


typedef struct {
    float MTP;
    float AANT;
    unsigned int GP;
    bool MDET;
} exo_output;

typedef struct {
    unsigned long ALGMOD;
    float FVANTHS;
    double VACTHS;
    float FVANTTO;
    double VACTTO;
    unsigned long HOMOD;
    float FVANTHO;
    float SVANTHO;
    unsigned long TWNOHO;
    float MAD;
	float MAP;
    float MASW;
    unsigned long ST;
	float TOP;
    unsigned long TO;
    float E1RP;
    unsigned long E1R;
	float E2RP;
    unsigned long E2R;
    float E3RP;
    unsigned long E3R;
    float E1SWP;
    unsigned long E1SW;
    float AANS;
    float AANE1R;
    float MTPE;
    float AANE;
} exo_params;


// ------ ALGORITHMS
void algo1(exo_meas *m, exo_output *o, exo_params *p, exo_params *pv, float (*AANM)(unsigned long, exo_params *, exo_params *, exo_output *));
void algoTrackAANM(exo_meas *m, exo_output *o, exo_params *p, exo_params *pv, float (*AANM)(unsigned long, exo_params *, exo_params *, exo_output *));
// ------



// ------ GENERAL PURPOSE FUNCTIONS
bool detectHS(exo_meas *m, exo_params *p);
bool detectTO(exo_meas *m, exo_params *p);
bool detectHO(exo_meas *m, exo_params *p);
float calcMTP(float AANT, exo_meas *m, exo_params *p);
// ------


// ------ ANKLE ANGLE MODEL
float AANMLinearFixedValues(unsigned long TS, exo_params *p, exo_params *pv, exo_output *o);
float AANMLinearFixedSlopes(unsigned long TS, exo_params *p, exo_params *pv, exo_output *o);
float AANMLinearAdaptive(unsigned long TS, exo_params *p, exo_params *pv, exo_output *o);
float AANMLinearMixed1(unsigned long TS, exo_params *p, exo_params *pv, exo_output *o);
// ------

/*
#ifdef __cplusplus
}
#endif
*/
