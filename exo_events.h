

#pragma once

#include <stdbool.h>
#include "exo_meas.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    VAC_ONLY_1_HS,
    VAC_AND_FVAN_1_HS
} cond_HS;

typedef enum {
    VAC_ONLY_1_TO,
    VAC_AND_FVAN_1_TO
} cond_TO;

typedef enum {
    FVAN_AND_AAN_1_HO,
    FVAN_ONLY_1_HO,
    SVAN_ONLY_1_HO,
    SVAN_ONLY_2_HO
} cond_HO;

typedef enum {
    PATTERN_NONE,
    PATTERN_RISING, // optionally above a threshold
    PATTERN_FALLING,    // optionally below a threshold
    PATTERN_AFTER_PEAK, // synonim for FALLING
    PATTERN_RISING_ACROSS_TH,
    PATTERN_FALLING_ACROSS_TH,
    PATTERN_GREATER_THAN_TH,
    PATTERN_LESSER_THAN_TH
} pattern;

typedef struct {
    cond_HS COND;
    float TH_FVAN;
    double TH_VAC;
    unsigned long TH_CONT;
    unsigned long CONT;
    pattern FVAN;
    pattern VAC;
    float PREV_FVAN;
    double PREV_VAC;
    float FVAN_DIFF;
    double VAC_DIFF;
    float PREV_FVAN_DIFF;
    double PREV_VAC_DIFF;
    bool FVAN_STARTED_FALLING;
    bool VAC_STARTED_FALLING;
} exo_state_HS;

typedef struct {
    cond_TO COND;
    float TH_FVAN;
    double TH_VAC;
    unsigned long TH_CONT;
    unsigned long CONT;
    pattern FVAN;
    pattern VAC;
    float PREV_FVAN;
    double PREV_VAC;
    float FVAN_DIFF;
    double VAC_DIFF;
    float PREV_FVAN_DIFF;
    double PREV_VAC_DIFF;
    bool FVAN_STARTED_RISING;
    bool VAC_STARTED_FALLING;
} exo_state_TO;

typedef struct {
    cond_HO COND;
    float TH_FVAN;
    float TH_SVAN;
    unsigned long TH_CONT;
    unsigned long CONT;
    pattern FVAN;
    pattern AAN;
    pattern SVAN;
    float PREV_FVAN;
    float PREV_SVAN;
    double PREV_AAN;
    float FVAN_DIFF;
    float SVAN_DIFF;
    double AAN_DIFF;
    double PREV_AAN_DIFF;
    bool FVAN_CROSSING_TH_DEC;
    bool AAN_STARTED_FALLING;
    bool SVAN_GREATER_THAN_TH;
    bool SVAN_CROSSING_TH_INC;
} exo_state_HO;

bool detectHS_CHILD_NORMAL(exo_meas *d, exo_state_HS *dHS);
bool detectTO_CHILD_NORMAL(exo_meas *d, exo_state_TO *dTO);
bool detectHO_CHILD_NORMAL(exo_meas *d, exo_state_HO *dHO);

#ifdef __cplusplus
}
#endif
