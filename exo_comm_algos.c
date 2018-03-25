
#include "exo_comm_algos.h"


void parseParamsMsgIn(char *msgIn, char *param, char *action, char *value) {
    const char s[2] = "|";
    char *token;
    token = strtok(msgIn, s);
    strcpy(param, token);
    token = strtok(NULL, s);
    strcpy(action, token);
    if (strcmp(action, "W") == 0) {
      token = strtok(NULL, s);
      strcpy(value, token);
      value[4] = '\0';
    }
}


void updateParam(char *param, char *value, exo_params *params) {
    // ALGMOD
    if (strcmp(param, "ALGMOD") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->ALGMOD = val;
    }
    // FVANTHS
    if (strcmp(param, "FVANTHS") == 0) {
        double val;
        val = STRTOD(value);
        params->FVANTHS = (float)val;
    }
    // VACTHS
    if (strcmp(param, "VACTHS") == 0) {
        double val;
        val = STRTOD(value);
        params->VACTHS = val;
    }
    // FVANTTO
    if (strcmp(param, "FVANTTO") == 0) {
        double val;
        val = STRTOD(value);
        params->FVANTTO = (float)val;
    }
    // VACTTO
    if (strcmp(param, "VACTTO") == 0) {
        double val;
        val = STRTOD(value);
        params->VACTTO = val;
    }
    // HOMOD
    if (strcmp(param, "HOMOD") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->HOMOD = val;
    }
    // FVANTHO
    if (strcmp(param, "FVANTHO") == 0) {
        double val;
        val = STRTOD(value);
        params->FVANTHO = (float)val;
    }
    // SVANTHO
    if (strcmp(param, "SVANTHO") == 0) {
        double val;
        val = STRTOD(value);
        params->SVANTHO = (float)val;
    }
    // TWNOHO
    if (strcmp(param, "TWNOHO") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->TWNOHO = val;
    }
    // MAD
    if (strcmp(param, "MAD") == 0) {
        double val;
        val = STRTOD(value);
        params->MAD = (float)val;
    }
    // MAP
    if (strcmp(param, "MAP") == 0) {
        double val;
        val = STRTOD(value);
        params->MAP = (float)val;
    }
    // MASW
    if (strcmp(param, "MASW") == 0) {
        double val;
        val = STRTOD(value);
        params->MASW = (float)val;
    }
    // ST
    if (strcmp(param, "ST") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->ST = val;
    }
    // TOP
    if (strcmp(param, "TOP") == 0) {
        double val;
        val = STRTOD(value);
        params->TOP = (float)val;
    }
    // TO
    if (strcmp(param, "TO") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->TO = val;
    }
    // E1RP
    if (strcmp(param, "E1RP") == 0) {
        double val;
        val = STRTOD(value);
        params->E1RP = (float)val;
    }
    // E1R
    if (strcmp(param, "E1R") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->E1R = val;
    }
    // E2RP
    if (strcmp(param, "E2RP") == 0) {
        double val;
        val = STRTOD(value);
        params->E2RP = (float)val;
    }
    // E2R
    if (strcmp(param, "E2R") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->E2R = val;
    }
    // E3RP
    if (strcmp(param, "E3RP") == 0) {
        double val;
        val = STRTOD(value);
        params->E3RP = (float)val;
    }
    // E3R
    if (strcmp(param, "E3R") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->E3R = val;
    }
    // E1SWP
    if (strcmp(param, "E1SWP") == 0) {
        double val;
        val = STRTOD(value);
        params->E1SWP = (float)val;
    }
    // E1SW
    if (strcmp(param, "E1SW") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        params->E1SW = val;
    }
    // AANS
    if (strcmp(param, "AANS") == 0) {
        double val;
        val = STRTOD(value);
        params->AANS = (float)val;
    }
    // MTPE
    if (strcmp(param, "MTPE") == 0) {
        double val;
        val = STRTOD(value);
        params->MTPE = (float)val;
    }
    // AANE
    if (strcmp(param, "AANE") == 0) {
        double val;
        val = STRTOD(value);
        params->AANE = (float)val;
    }
    // ST
    if (strcmp(param, "ST") == 0) {
      unsigned long val;
      val = STRTOUL(value);
      params->ST = val;
    }
}



/*
Some messages are space-padded in the end.
I don't know why, but that is necessary, otherwise the WiFi message is not sent,
and Arduino gets stuck. Please do not chante it.
*/
void prepareParamsMsgOut(char *param, char *action, char *value, exo_params *params, char *msgOut) {
    char *msg;
    msg = msgOut;
    char strVal[10];
    // TH0 (dummy)
    if (strcmp(param, "TH0") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(-15.2, 6, 2, strVal);
        sprintf(msg, "TH0|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 2, strVal);
        sprintf(msg, "TH0| Written %s", strVal);
      } else {
        sprintf(msg, "TH0| Msg unknown");
      }
    }
    // ALGMOD
    if (strcmp(param, "ALGMOD") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "ALGMOD|Read %lu", params->ALGMOD);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "ALGMOD| Written %lu", params->ALGMOD);
      } else {
        sprintf(msg, "ALGMOD| Msg unknown");
      }
    }
    // FVANTHS
    if (strcmp(param, "FVANTHS") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->FVANTHS, 6, 1, strVal);
        sprintf(msg, "FVANTHS|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "FVANTHS| Written %s", strVal);
      } else {
        sprintf(msg, "FVANTHS| Msg unknown");
      }
    }
    // VACTHS
    if (strcmp(param, "VACTHS") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->VACTHS, 6, 1, strVal);
        sprintf(msg, "VACTHS|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "VACTHS| Written %s ", strVal);
      } else {
        sprintf(msg, "VACTHS| Msg unknown");
      }
    }
    // FVANTTO
    if (strcmp(param, "FVANTTO") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->FVANTTO, 6, 1, strVal);
        sprintf(msg, "FVANTTO|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "FVANTTO| Written %s", strVal);
      } else {
        sprintf(msg, "FVANTTO| Msg unknown");
      }
    }
    // VACTTO
    if (strcmp(param, "VACTTO") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->VACTTO, 6, 1, strVal);
        sprintf(msg, "VACTTO|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "VACTTO| Written %s ", strVal);
      } else {
        sprintf(msg, "VACTTO| Msg unknown");
      }
    }
    // HOMOD
    if (strcmp(param, "HOMOD") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "HOMOD|Read %lu ", params->HOMOD);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "HOMOD| Written %lu ", params->HOMOD);
      } else {
        sprintf(msg, "HOMOD| Msg unknown");
      }
    }
    // FVANTHO
    if (strcmp(param, "FVANTHO") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->FVANTHO, 6, 1, strVal);
        sprintf(msg, "FVANTHO|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "FVANTHO| Written %s", strVal);
      } else {
        sprintf(msg, "FVANTHO| Msg unknown");
      }
    }
    // SVANTHO
    if (strcmp(param, "SVANTHO") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->SVANTHO, 6, 1, strVal);
        sprintf(msg, "SVANTHO|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "SVANTHO| Written %s", strVal);
      } else {
        sprintf(msg, "SVANTHO| Msg unknown");
      }
    }
    // TWNOHO
    if (strcmp(param, "TWNOHO") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "TWNOHO|Read %lu", params->TWNOHO);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "TWNOHO| Written %lu", params->TWNOHO);
      } else {
        sprintf(msg, "TWNOHO| Msg unknown");
      }
    }
    // MAD
    if (strcmp(param, "MAD") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->MAD, 6, 1, strVal);
        sprintf(msg, "MAD|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "MAD| Written %s", strVal);
      } else {
        sprintf(msg, "MAD| Msg unknown");
      }
    }
    // MAP
    if (strcmp(param, "MAP") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->MAP, 6, 1, strVal);
        sprintf(msg, "MAP|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "MAP| Written %s", strVal);
      } else {
        sprintf(msg, "MAP| Msg unknown");
      }
    }
    // MASW
    if (strcmp(param, "MASW") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->MASW, 6, 1, strVal);
        sprintf(msg, "MASW|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "MASW| Written %s", strVal);
      } else {
        sprintf(msg, "MASW| Msg unknown");
      }
    }
    // ST
    if (strcmp(param, "ST") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "ST|Read %lu", params->ST);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "ST| Written %lu", params->ST);
      } else {
        sprintf(msg, "ST| Msg unknown");
      }
    }
    // TOP
    if (strcmp(param, "TOP") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->TOP, 6, 2, strVal);
        sprintf(msg, "TOP|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 2, strVal);
        sprintf(msg, "TOP| Written %s", strVal);
      } else {
        sprintf(msg, "TOP| Msg unknown");
      }
    }
    // TO
    if (strcmp(param, "TO") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "TO|Read %lu", params->TO);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "TO| Written %lu", params->TO);
      } else {
        sprintf(msg, "TO| Msg unknown");
      }
    }
    // E1RP
    if (strcmp(param, "E1RP") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->E1RP, 6, 2, strVal);
        sprintf(msg, "E1RP|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 2, strVal);
        sprintf(msg, "E1RP| Written %s", strVal);
      } else {
        sprintf(msg, "E1RP| Msg unknown");
      }
    }
    // E1R
    if (strcmp(param, "E1R") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "E1R|Read %lu", params->E1R);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "E1R| Written %lu", params->E1R);
      } else {
        sprintf(msg, "E1R| Msg unknown");
      }
    }
    // E2RP
    if (strcmp(param, "E2RP") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->E2RP, 6, 2, strVal);
        sprintf(msg, "E2RP|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 2, strVal);
        sprintf(msg, "E2RP| Written %s", strVal);
      } else {
        sprintf(msg, "E2RP| Msg unknown");
      }
    }
    // E2R
    if (strcmp(param, "E2R") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "E2R|Read %lu", params->E2R);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "E2R| Written %lu", params->E2R);
      } else {
        sprintf(msg, "E2R| Msg unknown");
      }
    }
    // E3RP
    if (strcmp(param, "E3RP") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->E3RP, 6, 2, strVal);
        sprintf(msg, "E3RP|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 2, strVal);
        sprintf(msg, "E3RP| Written %s", strVal);
      } else {
        sprintf(msg, "E3RP| Msg unknown");
      }
    }
    // E3R
    if (strcmp(param, "E3R") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "E3R|Read %lu", params->E3R);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "E3R| Written %lu", params->E3R);
      } else {
        sprintf(msg, "E3R| Msg unknown");
      }
    }
    // E1SWP
    if (strcmp(param, "E1SWP") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->E1SWP, 6, 2, strVal);
        sprintf(msg, "E1SWP|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 2, strVal);
        sprintf(msg, "E1SWP| Written %s", strVal);
      } else {
        sprintf(msg, "E1SWP| Msg unknown");
      }
    }
    // E1SW
    if (strcmp(param, "E1SW") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "E1SW|Read %lu  ", params->E1SW);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "E1SW| Written %lu", params->E1SW);
      } else {
        sprintf(msg, "E1SW| Msg unknown");
      }
    }
    // AANS
    if (strcmp(param, "AANS") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->AANS, 6, 1, strVal);
        sprintf(msg, "AANS|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "AANS| Written %s", strVal);
      } else {
        sprintf(msg, "AANS| Msg unknown");
      }
    }
    // MTPE
    if (strcmp(param, "MTPE") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->MTPE, 6, 1, strVal);
        sprintf(msg, "MTPE|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "MTPE| Written %s", strVal);
      } else {
        sprintf(msg, "MTPE| Msg unknown");
      }
    }
    // AANE
    if (strcmp(param, "AANE") == 0) {
      if (strcmp(action, "R") == 0) {
        DTOSTRF(params->AANE, 6, 1, strVal);
        sprintf(msg, "AANE|Read %s", strVal);
      } else if (strcmp(action, "W") == 0) {
        double val;
        val = STRTOD(value);
        DTOSTRF(val, 6, 1, strVal);
        sprintf(msg, "AANE| Written %s", strVal);
      } else {
        sprintf(msg, "AANE| Msg unknown");
      }
    }
    // ST
    if (strcmp(param, "ST") == 0) {
      if (strcmp(action, "R") == 0) {
        sprintf(msg, "ST|Read %lu ", params->ST);
      } else if (strcmp(action, "W") == 0) {
        unsigned long val;
        val = STRTOUL(value);
        sprintf(msg, "ST| Written %lu ", params->ST);
      } else {
        sprintf(msg, "ST| Msg unknown ");
      }
    }
}
