#include <stdio.h>

#include "ArduinoCore-API/api/deprecated-avr-comp/avr/dtostrf.h"

// dtostrf() - Float/Double to String
char* dtostrf(double val, signed char width, unsigned char prec, char* buf) {
    snprintf(buf, 32, "%*.*f", width, prec, val);
    return buf;
}