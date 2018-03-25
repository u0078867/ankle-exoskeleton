
#pragma once

#include "exo_algos.h"
//#include <stdlib.h>     // strtod
#include "exo_common.h"


#ifdef __cplusplus
extern "C" {
#endif


void parseParamsMsgIn(char *msgIn, char *param, char *action, char *value);
void updateParam(char *param, char *value, exo_params *params);
void prepareParamsMsgOut(char *param, char *action, char *value, exo_params *params, char *msgOut);


#ifdef __cplusplus
}
#endif
