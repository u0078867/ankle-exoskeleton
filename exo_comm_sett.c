
#include "exo_comm_sett.h"


void parseSettsMsgIn(char *msgIn, char *sett, char *action, char *value) {
    const char s[2] = "|";
    char *token;
    token = strtok(msgIn, s);
    strcpy(sett, token);
    token = strtok(NULL, s);
    strcpy(action, token);
    if (strcmp(action, "W") == 0) {
      token = strtok(NULL, s);
      strcpy(value, token);
      value[4] = '\0';
    }
}


void updateSett(char *sett, char *value, exo_setts *setts) {
    // MECHBLOCK
    if (strcmp(sett, "MECHBLOCK") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        setts->MECHBLOCK = val;
    }
    // SERVOBEH
    if (strcmp(sett, "SERVOBEH") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        setts->SERVOBEH = val;
    }
}



/*
Some messages are space-padded in the end.
I don't know why, but that is necessary, otherwise the WiFi message is not sent,
and Arduino gets stuck. Please do not chante it.
*/
void prepareSettsMsgOut(char *sett, char *action, char *value, exo_setts *setts, char *msgOut) {
    char *msg;
    msg = msgOut;
    char strVal[10];
    // MECHBLOCK
    if (strcmp(sett, "MECHBLOCK") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "MECHBLOCK|Read %lu", setts->MECHBLOCK);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "MECHBLOCK| Written %lu", setts->MECHBLOCK);
      } else {
        sprintf(msg, "MECHBLOCK| Msg unknown");
      }
    }
    // SERVOBEH
    if (strcmp(sett, "SERVOBEH") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "SERVOBEH|Read %lu", setts->SERVOBEH);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "SERVOBEH| Written %lu", setts->SERVOBEH);
      } else {
        sprintf(msg, "SERVOBEH| Msg unknown");
      }
    }
}
