
#pragma once

#include "exo_sett.h"
#include "exo_common.h"


#ifdef __cplusplus
extern "C" {
#endif


void parseSettsMsgIn(char *msgIn, char *sett, char *action, char *value);
void updateSett(char *sett, char *value, exo_setts *setts);
void prepareSettsMsgOut(char *sett, char *action, char *value, exo_setts *setts, char *msgOut);


#ifdef __cplusplus
}
#endif
