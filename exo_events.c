

#include "exo_events.h"


bool detectHS_CHILD_NORMAL(exo_meas *d, exo_state_HS *dHS) {

    bool detected = false;

    dHS->FVAN_DIFF = d->FVAN - dHS->PREV_FVAN;
    dHS->VAC_DIFF = d->VAC - dHS->PREV_VAC;
    dHS->FVAN_STARTED_FALLING = false;
    if (dHS->PREV_FVAN_DIFF >= 0 && dHS->FVAN_DIFF < 0)
        dHS->FVAN_STARTED_FALLING = true;
    dHS->VAC_STARTED_FALLING = false;
    if (dHS->PREV_VAC_DIFF >= 0 && dHS->VAC_DIFF < 0)
        dHS->VAC_STARTED_FALLING = true;

    if (dHS->CONT > dHS->TH_CONT) {
        dHS->FVAN = PATTERN_NONE;
        dHS->VAC = PATTERN_NONE;
        dHS->CONT = 0;
    }

    if (dHS->FVAN == PATTERN_FALLING || dHS->VAC == PATTERN_AFTER_PEAK)
        dHS->CONT++;

    if (dHS->FVAN_STARTED_FALLING && dHS->PREV_FVAN > dHS->TH_FVAN)
        dHS->FVAN = PATTERN_FALLING;
    if (dHS->VAC_STARTED_FALLING && dHS->PREV_VAC > dHS->TH_VAC)
        dHS->VAC = PATTERN_AFTER_PEAK;

    if (dHS->COND == VAC_AND_FVAN_1_HS)
        if (dHS->FVAN == PATTERN_FALLING && dHS->VAC == PATTERN_AFTER_PEAK)
            detected = true;
    else if (dHS->COND == VAC_ONLY_1_HS)
        if (dHS->VAC == PATTERN_AFTER_PEAK)
            detected = true;

    if (detected) {
        dHS->FVAN = PATTERN_NONE;
        dHS->VAC = PATTERN_NONE;
        dHS->CONT = 0;
    }

    dHS->PREV_FVAN = d->FVAN;
    dHS->PREV_VAC = d->VAC;
    dHS->PREV_FVAN_DIFF = dHS->FVAN_DIFF;
    dHS->PREV_VAC_DIFF = dHS->VAC_DIFF;

    return detected;

}





bool detectTO_CHILD_NORMAL(exo_meas *d, exo_state_TO *dTO) {

    bool detected = false;

    dTO->FVAN_DIFF = d->FVAN - dTO->PREV_FVAN;
    dTO->VAC_DIFF = d->VAC - dTO->PREV_VAC;
    dTO->FVAN_STARTED_RISING = false;
    if (dTO->PREV_FVAN_DIFF <= 0 && dTO->FVAN_DIFF > 0)
        dTO->FVAN_STARTED_RISING = true;
    dTO->VAC_STARTED_FALLING = false;
    if (dTO->PREV_VAC_DIFF >= 0 && dTO->VAC_DIFF < 0)
        dTO->VAC_STARTED_FALLING = true;

    if (dTO->CONT > dTO->TH_CONT) {
        dTO->FVAN = PATTERN_NONE;
        dTO->VAC = PATTERN_NONE;
        dTO->CONT = 0;
    }

    if (dTO->FVAN == PATTERN_RISING || dTO->VAC == PATTERN_AFTER_PEAK)
        dTO->CONT++;

    if (dTO->FVAN_STARTED_RISING && dTO->PREV_FVAN < dTO->TH_FVAN)
        dTO->FVAN = PATTERN_RISING;
    if (dTO->VAC_STARTED_FALLING && dTO->PREV_VAC > dTO->TH_VAC)
        dTO->VAC = PATTERN_AFTER_PEAK;

    if (dTO->COND == VAC_AND_FVAN_1_TO)
        if (dTO->FVAN == PATTERN_RISING && dTO->VAC == PATTERN_AFTER_PEAK)
            detected = true;
    else if (dTO->COND == VAC_ONLY_1_TO)
        if (dTO->VAC == PATTERN_AFTER_PEAK)
            detected = true;

    if (detected) {
        dTO->FVAN = PATTERN_NONE;
        dTO->VAC = PATTERN_NONE;
        dTO->CONT = 0;
    }

    dTO->PREV_FVAN = d->FVAN;
    dTO->PREV_VAC = d->VAC;
    dTO->PREV_FVAN_DIFF = dTO->FVAN_DIFF;
    dTO->PREV_VAC_DIFF = dTO->VAC_DIFF;

    return detected;

}



bool detectHO_CHILD_NORMAL(exo_meas *d, exo_state_HO *dHO) {

    bool detected = false;

    dHO->FVAN_DIFF = d->FVAN - dHO->PREV_FVAN;
    dHO->SVAN_DIFF = d->SVAN - dHO->PREV_SVAN;
    dHO->AAN_DIFF = d->AAN - dHO->PREV_AAN;
    dHO->FVAN_CROSSING_TH_DEC = false;
    if (dHO->FVAN_DIFF < 0 && d->FVAN <= dHO->TH_FVAN && dHO->PREV_FVAN > dHO->TH_FVAN)
        dHO->FVAN_CROSSING_TH_DEC = true;
    dHO->AAN_STARTED_FALLING = false;
    if (dHO->PREV_AAN_DIFF >= 0 && dHO->AAN_DIFF < 0)
        dHO->AAN_STARTED_FALLING = true;
    dHO->SVAN_GREATER_THAN_TH = false;
    if (d->SVAN >= dHO->TH_SVAN)
        dHO->SVAN_GREATER_THAN_TH = true;
    dHO->SVAN_CROSSING_TH_INC = false;
    if (dHO->SVAN_DIFF > 0 && d->SVAN >= dHO->TH_SVAN && dHO->PREV_SVAN < dHO->TH_SVAN)
        dHO->SVAN_CROSSING_TH_INC = true;

    if (dHO->CONT > dHO->TH_CONT) {
        dHO->FVAN = PATTERN_NONE;
        dHO->AAN = PATTERN_NONE;
        dHO->SVAN = PATTERN_NONE;
        dHO->CONT = 0;
    }

    if (dHO->FVAN == PATTERN_FALLING_ACROSS_TH || dHO->AAN == PATTERN_FALLING)
        dHO->CONT++;

    if (dHO->FVAN_CROSSING_TH_DEC)
        dHO->FVAN = PATTERN_FALLING_ACROSS_TH;
    if (dHO->AAN_STARTED_FALLING)
        dHO->AAN = PATTERN_FALLING;
    if (dHO->SVAN_GREATER_THAN_TH)
        dHO->SVAN = PATTERN_GREATER_THAN_TH;
    if (dHO->SVAN_CROSSING_TH_INC)
        dHO->SVAN = PATTERN_RISING_ACROSS_TH;

    if (dHO->COND == FVAN_AND_AAN_1_HO)
        if (dHO->FVAN == PATTERN_FALLING_ACROSS_TH && dHO->AAN == PATTERN_FALLING)
            detected = true;
    if (dHO->COND == FVAN_ONLY_1_HO)
        if (dHO->FVAN == PATTERN_FALLING_ACROSS_TH)
            detected = true;
    else if (dHO->COND == SVAN_ONLY_1_HO)
        if (dHO->SVAN == PATTERN_GREATER_THAN_TH || dHO->SVAN == PATTERN_RISING_ACROSS_TH)
            detected = true;
    else if (dHO->COND == SVAN_ONLY_2_HO)
        if (dHO->SVAN == PATTERN_RISING_ACROSS_TH)
            detected = true;

    if (detected) {
        dHO->FVAN = PATTERN_NONE;
        dHO->AAN = PATTERN_NONE;
        dHO->SVAN = PATTERN_NONE;
        dHO->CONT = 0;
    }

    dHO->PREV_FVAN = d->FVAN;
    dHO->PREV_SVAN = d->SVAN;
    dHO->PREV_AAN = d->AAN;
    dHO->PREV_AAN_DIFF = dHO->AAN_DIFF;

    return detected;

}
