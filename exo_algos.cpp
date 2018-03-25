
#include "exo_algos.h"
#include <math.h>


bool detectHS(exo_meas *m, exo_params *p) {
    bool detected;
    /* // C++ only
    static exo_state_HS dHS = {
        .COND = VAC_AND_FVAN_1_HS,
        ...
    };
    */
    static exo_state_HS dHS;
    static bool init = true;
    if (init) {
        dHS.COND = VAC_AND_FVAN_1_HS;
        dHS.CONT = 0;
        dHS.VAC = PATTERN_NONE;
        dHS.FVAN = PATTERN_NONE;
        dHS.PREV_FVAN = 0.;
        dHS.PREV_VAC = 0.;
        dHS.PREV_FVAN_DIFF = 0.;
        dHS.PREV_VAC_DIFF = 0;
        init = false;
    }
    dHS.TH_FVAN = p->FVANTHS;
    dHS.TH_VAC = p->VACTHS;
    dHS.TH_CONT = 30;
#if SUBJECT_TYPE == 1
    detected = detectHS_CHILD_NORMAL(m, &dHS);
#endif
    return detected;
}

bool detectTO(exo_meas *m, exo_params *p) {
    bool detected;
    static exo_state_TO dTO;
    static bool init = true;
    if (init) {
        dTO.COND = VAC_AND_FVAN_1_TO;
        dTO.CONT = 0;
        dTO.VAC = PATTERN_NONE;
        dTO.FVAN = PATTERN_NONE;
        dTO.PREV_FVAN = 0.;
        dTO.PREV_VAC = 0.;
        dTO.PREV_FVAN_DIFF = 0.;
        dTO.PREV_VAC_DIFF = 0.;
        init = false;
    }
    dTO.TH_FVAN = p->FVANTTO;
    dTO.TH_VAC = p->VACTTO;
    dTO.TH_CONT = 30;
#if SUBJECT_TYPE == 1
    detected = detectTO_CHILD_NORMAL(m, &dTO);
#endif
    return detected;
}

bool detectHO(exo_meas *m, exo_params *p) {
    bool detected;
    static exo_state_HO dHO;
    static bool init = true;
    if (init) {
        dHO.CONT = 0;
        dHO.AAN = PATTERN_NONE;
        dHO.FVAN = PATTERN_NONE;
        dHO.SVAN = PATTERN_NONE;
        dHO.PREV_FVAN = 0.;
        dHO.PREV_SVAN = 0.;
        dHO.PREV_AAN = 0.;
        dHO.PREV_AAN_DIFF = 0.;
        init = false;
    }
    dHO.TH_FVAN = p->FVANTHO;
    dHO.TH_SVAN = p->SVANTHO;
    if (p->HOMOD == 1) {
        dHO.COND = FVAN_AND_AAN_1_HO;
    } else if (p->HOMOD == 2) {
        dHO.COND = SVAN_ONLY_1_HO;
    } else if (p->HOMOD == 3) {
        dHO.COND = SVAN_ONLY_2_HO;
    } else if (p->HOMOD == 4) {
        dHO.COND = FVAN_ONLY_1_HO;
    }
    dHO.TH_CONT = 30;
#if SUBJECT_TYPE == 1
    detected = detectHO_CHILD_NORMAL(m, &dHO);
#endif
    return detected;
}


float calcMTP(float AANT, exo_meas *m, exo_params *p) {
    double Kp;
    float MTP;
    double dGamma, dTeta, cGamma, cTeta;
    double quadB;
    dGamma = DEG_TO_RAD * (AANT - m->AAN);
    cGamma = DEG_TO_RAD * (m->AAN + m->AANG);
    quadB = (double)m->QUADB;
#if SERVO_POS_MEASURE == 1
    cTeta = DEG_TO_RAD * POS_SERVO_TO_TETA(m->MP);
#elif SERVO_POS_MEASURE == 2
    double quadAlpha, quadP;
    cTeta = gamma_to_teta_theor(QUADRL_A, quadB, QUADRL_C, m->SPC, cGamma, &quadAlpha, &quadP);
#endif
    //if (cTeta < (DEG_TO_RAD * 146.)) {
    if (cTeta < (DEG_TO_RAD * POS_SERVO_TO_TETA(AAN_MIN_BLOCK_POS))) {
        // Apply quadrilateral trigonometry
#if GAMMA_TO_TETA_DELTA == 1
        dTeta = gamma_to_teta_delta(QUADRL_A, quadB, QUADRL_C, m->SPC, dGamma, cGamma, cTeta);
#elif GAMMA_TO_TETA_DELTA == 2
#if(SIDE)
        dTeta = gamma_to_teta_delta_R(m->SPC, dGamma, cGamma, cTeta);
#else
        dTeta = gamma_to_teta_delta_L(m->SPC, dGamma, cGamma, cTeta);
#endif
#elif GAMMA_TO_TETA_DELTA == 3
        dTeta =  gamma_to_teta_delta_cse_ofast(QUADRL_A, quadB, QUADRL_C, m->SPC, dGamma, cGamma, cTeta);
#endif
        // Apply PID
#if GAMMA_TO_TETA_DELTA == 4
        Kp = ALGO_PID_KP;
#else
        Kp = dTeta / dGamma;
#endif
        dTeta = PID(dGamma, Kp, ALGO_PID_KI, ALGO_PID_KD, m->TS);
        MTP = (float)TETA_TO_POS_SERVO(RAD_TO_DEG * (cTeta + dTeta));
    } else {
        // Can't apply quadrilateral trigonometry. Make teta bit lower than 180 deg
        //MTP = (float)TETA_TO_POS_SERVO(146.); // no vibration
        MTP = (float)AAN_MIN_BLOCK_POS; // no vibration
    }
#ifdef DEBUG_ALGO
#ifdef SERVO_POS_MEASURE == 2
    DEBUG_PRINT("quadB: ");
    DEBUG_PRINTLN(quadB);
    DEBUG_PRINT("quadP: ");
    DEBUG_PRINTLN(quadP);
    DEBUG_PRINT("quadAlpha: ");
    DEBUG_PRINTLN(quadAlpha * RAD_TO_DEG);
#endif
    DEBUG_PRINT("dGamma: ");
    DEBUG_PRINTLN(dGamma * RAD_TO_DEG);
    DEBUG_PRINT("cGamma: ");
    DEBUG_PRINTLN(cGamma * RAD_TO_DEG);
    DEBUG_PRINT("cTeta: ");
    DEBUG_PRINTLN(cTeta * RAD_TO_DEG);
    DEBUG_PRINT("dTeta: ");
    DEBUG_PRINTLN(dTeta * RAD_TO_DEG);
    DEBUG_PRINT("Old MP: ");
    DEBUG_PRINTLN(m->MP);
    DEBUG_PRINT("New MP: ");
    DEBUG_PRINTLN(MTP);
#endif
    return MTP;
}




float AANMLinearFixedValues(unsigned long TS, exo_params *p, exo_params *pv, exo_output * o) {
    float m, q, res;
    if (o->GP == 2) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(p->MAD);
        DEBUG_PRINT("Model AANE1R: ");
        DEBUG_PRINTLN(pv->AANE1R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(p->E2R);
        DEBUG_PRINT("Model E1R: ");
        DEBUG_PRINTLN(pv->E1R);
#endif
        m = (p->MAD - pv->AANE1R) / (p->E2R - pv->E1R);
        q = pv->AANE1R;
        res = m * (TS - pv->E1R) + q;
    } else if (o->GP == 3) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(p->MAP);
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(pv->MAD);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(p->E3R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(pv->E2R);
#endif
        m = (p->MAP - pv->MAD) / (p->E3R - pv->E2R);
        q = pv->MAD;
        res = m * (TS - pv->E2R) + q;
    } else if (o->GP == 4) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MASW: ");
        DEBUG_PRINTLN(p->MASW);
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(pv->MAP);
        DEBUG_PRINT("Model E1SW: ");
        DEBUG_PRINTLN(p->E1SW);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(pv->E3R);
#endif
        m = (p->MASW - pv->MAP) / (p->E1SW - pv->E3R);
        q = pv->MAP;
        res = m * (TS - pv->E3R) + q;
    }
#ifdef DEBUG_ALGO
    DEBUG_PRINT("Model m: ");
    DEBUG_PRINTLN(m);
    DEBUG_PRINT("Model q: ");
    DEBUG_PRINTLN(q);
#endif
    return res;
}



float AANMLinearFixedSlopes(unsigned long TS, exo_params *p, exo_params *pv, exo_output * o) {
    float m, q, res;
    if (o->GP == 2) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(p->MAD);
        DEBUG_PRINT("Model AANE1R: ");
        DEBUG_PRINTLN(p->AANE1R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(p->E2R);
        DEBUG_PRINT("Model E1R: ");
        DEBUG_PRINTLN(pv->E1R);
#endif
        m = (p->MAD - p->AANE1R) / (p->E2R - pv->E1R);  // E1R must be variable
        q = pv->AANE1R;
        res = m * (TS - pv->E1R) + q;
    } else if (o->GP == 3) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(p->MAP);
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(p->MAD);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(p->E3R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(p->E2R);
#endif
        m = (p->MAP - p->MAD) / (p->E3R - p->E2R);
        q = pv->MAD;
        res = m * (TS - pv->E2R) + q;
    } else if (o->GP == 4) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MASW: ");
        DEBUG_PRINTLN(p->MASW);
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(p->MAP);
        DEBUG_PRINT("Model E1SW: ");
        DEBUG_PRINTLN(p->E1SW);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(p->E3R);
#endif
        m = (p->MASW - p->MAP) / (p->E1SW - p->E3R);
        q = pv->MAP;
        res = m * (TS - pv->E3R) + q;
    }
#ifdef DEBUG_ALGO
    DEBUG_PRINT("Model m: ");
    DEBUG_PRINTLN(m);
    DEBUG_PRINT("Model q: ");
    DEBUG_PRINTLN(q);
#endif
    return res;
}



float AANMLinearAdaptive(unsigned long TS, exo_params *p, exo_params *pv, exo_output * o) {
    float m, q, res;
    if (o->GP == 2) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(pv->MAD);
        DEBUG_PRINT("Model AANE1R: ");
        DEBUG_PRINTLN(pv->AANE1R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(pv->E2R);
        DEBUG_PRINT("Model E1R: ");
        DEBUG_PRINTLN(pv->E1R);
#endif
        m = (pv->MAD - pv->AANE1R) / (pv->E2R - pv->E1R);
        q = pv->AANE1R;
        res = m * (TS - pv->E1R) + q;
    } else if (o->GP == 3) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(pv->MAP);
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(pv->MAD);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(pv->E3R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(pv->E2R);
#endif
        m = (pv->MAP - pv->MAD) / (pv->E3R - pv->E2R);
        q = pv->MAD;
        res = m * (TS - pv->E2R) + q;
    } else if (o->GP == 4) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MASW: ");
        DEBUG_PRINTLN(pv->MASW);
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(pv->MAP);
        DEBUG_PRINT("Model E1SW: ");
        DEBUG_PRINTLN(pv->E1SW);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(pv->E3R);
#endif
        m = (pv->MASW - pv->MAP) / (pv->E1SW - pv->E3R);
        q = pv->MAP;
        res = m * (TS - pv->E3R) + q;
    }
#ifdef DEBUG_ALGO
    DEBUG_PRINT("Model m: ");
    DEBUG_PRINTLN(m);
    DEBUG_PRINT("Model q: ");
    DEBUG_PRINTLN(q);
#endif
    return res;
}


float AANMLinearMixed1(unsigned long TS, exo_params *p, exo_params *pv, exo_output * o) {
    float m, q, res;
    if (o->GP == 2) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(p->MAD);
        DEBUG_PRINT("Model AANE1R: ");
        DEBUG_PRINTLN(p->AANE1R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(p->E2R);
        DEBUG_PRINT("Model E1R: ");
        DEBUG_PRINTLN(pv->E1R);
#endif
        m = (p->MAD - p->AANE1R) / (p->E2R - pv->E1R);  // E1R must be variable
        q = pv->AANE1R;
        res = m * (TS - pv->E1R) + q;
    } else if (o->GP == 3) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(p->MAP);
        DEBUG_PRINT("Model MAD: ");
        DEBUG_PRINTLN(pv->MAD);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(p->E3R);
        DEBUG_PRINT("Model E2R: ");
        DEBUG_PRINTLN(p->E2R);
#endif
        m = (p->MAP - pv->MAD) / (p->E3R - p->E2R);
        q = pv->MAD;
        res = m * (TS - pv->E2R) + q;
        if (res < p->MAP) {
            res = p->MAP;
        }
    } else if (o->GP == 4) {
#ifdef DEBUG_ALGO
        DEBUG_PRINT("Model MASW: ");
        DEBUG_PRINTLN(p->MASW);
        DEBUG_PRINT("Model MAP: ");
        DEBUG_PRINTLN(p->MAP);
        DEBUG_PRINT("Model E1SW: ");
        DEBUG_PRINTLN(p->E1SW);
        DEBUG_PRINT("Model E3R: ");
        DEBUG_PRINTLN(p->E3R);
#endif
        m = (p->MASW - p->MAP) / (p->E1SW - p->E3R);
        q = p->MAP;
        res = m * (TS - pv->E3R) + q;
    }
#ifdef DEBUG_ALGO
    DEBUG_PRINT("Model m: ");
    DEBUG_PRINTLN(m);
    DEBUG_PRINT("Model q: ");
    DEBUG_PRINTLN(q);
#endif
    return res;
}



void algo1(exo_meas *m, exo_output *o, exo_params *p, exo_params *pv, float (*AANM)(unsigned long, exo_params *, exo_params *, exo_output *)) {
    // Local variables
    static unsigned long TS0, TS2;
    unsigned long TS;
    float pct, AANT, MTP;
    bool HS, HO, canDetectHO, TO;
    // Handle states
    if (o->GP > 0) {
        TS = m->TS - TS0;
        pct = TS / p->ST * 100.;
    }
    // Detect events
    HS = detectHS(m, p);
    HO = detectHO(m, p);
    TO = detectTO(m, p);
#ifdef DEBUG_ALGO
    // Print debug messages
    DEBUG_PRINT("TS: ");
    DEBUG_PRINTLN(TS);
    DEBUG_PRINT("GP: ");
    DEBUG_PRINTLN(o->GP);
#endif
    switch(o->GP) {
        case 0: // waiting for step
            // Detect HS
            if (HS) {
                TS0 = m->TS;
                o->GP = 1;
            }
            o->MDET = true;
            break;
        case 1: // 1st rocker
            // Detect end of 1st rocker
            if (m->SVAN > 0) {
                o->GP = 2;
                pv->AANE1R = m->AAN;
                pv->E1R = TS;
                TS2 = TS;
            }
            o->MDET = true;
            break;
        case 2: // 2nd rocker
            // Calculate targets for ankle and motor
            AANT = (*AANM)(TS, p, pv, o);
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Decide if to detach motor or not
            if ((m->AAN - AANT) > p->AANE) {
                o->MDET = false;
            } else {
                o->MDET = true;
            }
            // Detect HO
            canDetectHO = false;
            if ((TS - TS2) > p->TWNOHO) {
                canDetectHO = true;
            }
            if (canDetectHO && HO) {
                o->GP = 3;
                pv->MAD = m->AAN;
                pv->E2R = TS;
            }
            break;
        case 3: // 3rd rocker
            // Calculate targets for ankle and motor
            AANT = (*AANM)(TS, p, pv, o);
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Decide if to detach motor or not
            if ((m->AAN - AANT) > p->AANE) {
                o->MDET = false;
            } else {
                o->MDET = true;
            }
            // Detect TO
            if (TO) {
                o->GP = 4;
                pv->MAP = m->AAN;
                pv->E3R = TS;
            }
            break;
        case 4: // swing (dorsiflexing)
            // Calculate targets for ankle and motor
            AANT = (*AANM)(TS, p, pv, o);
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Decide if to detach motor or not
            if ((AANT - m->AAN) > p->AANE) {
                o->MDET = false;
            } else {
                o->MDET = true;
            }
            // Detect time after dorsiflexion
            if ((TS - pv->E3R) > (p->E1SW - p->E3R)) {
                o->GP = 5;
                pv->MASW = m->AAN;
                pv->E1SW = TS;
            }
            break;
        case 5: // swing (dorsiflexed)
            // Calculate targets for ankle and motor
            AANT = p->MASW;
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Decide if to detach motor or not
            if ((AANT - m->AAN) > p->AANE) {
                o->MDET = false;
            } else {
                o->MDET = true;
            }
            // Detect HS
            if (HS) {
                TS0 = m->TS;
                o->GP = 1;
            }
            break;
        case 6: // idle
            if (p->ALGMOD == 1) {
                o->GP = 0;
            }
            o->MDET = true;
            break;

    }
    // Check if algo mode is on idle
    if (p->ALGMOD == 0) {
        o->GP = 6;
    }
}




void algoTrackAANM(exo_meas *m, exo_output *o, exo_params *p, exo_params *pv, float (*AANM)(unsigned long, exo_params *, exo_params *, exo_output *)) {
    // Local variables
    static unsigned long TS0;
    unsigned long TS;
    float pct, AANT, MTP;
    // Handle states
    if (o->GP > 0) {
        TS = m->TS - TS0;
        pct = TS / p->ST * 100.;
    }
#ifdef DEBUG_ALGO
    // Print debug messages
    DEBUG_PRINT("TS: ");
    DEBUG_PRINTLN(TS);
    DEBUG_PRINT("GP: ");
    DEBUG_PRINTLN(o->GP);
#endif
    switch(o->GP) {
        case 0: // waiting for step
            // Pass to next state immediately
            TS0 = m->TS;
            o->GP = 1;
            break;
        case 1: // 1st rocker
            // Set end of 1st rocker immediately
            o->GP = 2;
            pv->AANE1R = m->AAN;
            pv->E1R = TS;
            break;
        case 2: // 2nd rocker
            // Calculate targets for ankle and motor
            AANT = (*AANM)(TS, p, pv, o);
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Detect end of 2nd rocker
            if (TS > p->E2R) {
                o->GP = 3;
                pv->MAD = m->AAN;
                pv->E2R = TS;
            }
            break;
        case 3: // 3rd rocker
            // Calculate targets for ankle and motor
            AANT = (*AANM)(TS, p, pv, o);
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Detect TO
            if (TS > p->E3R) {
                o->GP = 4;
                pv->MAP = m->AAN;
                pv->E3R = TS;
            }
            break;
        case 4: // swing (dorsiflexing)
            // Calculate targets for ankle and motor
            AANT = (*AANM)(TS, p, pv, o);
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Detect time after dorsiflexion
            if ((TS - pv->E3R) > (p->E1SW - p->E3R)) {
                o->GP = 5;
                pv->MASW = m->AAN;
                pv->E1SW = TS;
            }
            break;
        case 5: // swing (dorsiflexed)
            // Calculate targets for ankle and motor
            AANT = p->MASW;
#ifdef DEBUG_ALGO
            DEBUG_PRINT("AANT: ");
            DEBUG_PRINTLN(AANT);
#endif
            o->AANT = AANT;
            MTP = calcMTP(AANT, m, p);
            o->MTP = MTP;
            // Detect end of 3rd rocker
            if (TS > p->ST) {
                TS0 = m->TS;
                o->GP = 1;
            }
            break;
        case 6: // idle
            if (p->ALGMOD == 1) {
                o->GP = 0;
            }
            //o->MDET = true;
            break;

    }
    // Check if algo mode is on idle
    if (p->ALGMOD == 0) {
        o->GP = 6;
    }
    // Impose servo always active (but idle state)
    if (o->GP == 6) {
        o->MDET = true;
    } else {
        o->MDET = false;
    }
}
