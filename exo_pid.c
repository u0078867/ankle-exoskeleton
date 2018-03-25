

#include "exo_pid.h"
#include <stdbool.h>


double PID(double e, double Kp, double Ki, double Kd, unsigned long T) {
    static double intE = 0.0;
    static bool init = false;
    static unsigned long prevT;
    double output;

    if (!init) {
        prevT = T;
        init = true;
    }

    intE += e * (T - prevT);
    output = Kp * e + Ki * intE;

    prevT = T;

    return output;
}
