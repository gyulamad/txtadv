#include <stdio.h>

#include "ArduinoCore-API/api/itoa.h"

// itoa(), ltoa(), ultoa(), utoa() - All via snprintf
char* itoa(int val, char* buf, int base) { 
    snprintf(buf, 32, "%d", val); 
    return buf; 
}

char* ltoa(long val, char* buf, int base) { 
    snprintf(buf, 32, "%ld", val); 
    return buf; 
}

char* ultoa(unsigned long val, char* buf, int base) { 
    snprintf(buf, 32, "%lu", val); 
    return buf; 
}

char* utoa(unsigned int val, char* buf, int base) { 
    snprintf(buf, 32, "%u", val); 
    return buf; 
}