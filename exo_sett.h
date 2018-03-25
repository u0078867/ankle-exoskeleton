
/**********************

-- NOMENCLATURE:

Settings:

- MECHBLOCK: servo motor temporary short blocking when reaching mechanical limits (0: don't apply; 1: apply)
- SERVOBEH: servo behaviour:
    1: attach/detach+repower; PROS: for non-latching servos, reactivity in both attaching and adetaching signal; CONS: powering/unpowering might decrease lifetime
    2: attach/detach; PROS: for non-latching servos, servo is always powered, so life time should not be affected; CONS: servo may not be very reactive when detaching;
    3: power/unpower; PROS: works for each servo, matching or unlatching; CONS: life time may be affected, servo may not be very reactive when attaching;
    note: reactivity in attaching may be more relevant than reactivity in detaching.

**********************/

#pragma once

/*
#ifdef __cplusplus
extern "C" {
#endif
*/


typedef struct {
    unsigned long MECHBLOCK;
    unsigned long SERVOBEH;
} exo_setts;



/*
#ifdef __cplusplus
}
#endif
*/
