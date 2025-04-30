#include "Particle.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// System thread defaults to on in 6.2.0 and later and this line is not required
#ifndef SYSTEM_VERSION_v620
SYSTEM_THREAD(ENABLED);
#endif

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

void setup() {
    waitFor(Serial.isConnected, 10000); delay(1000);


}

void loop() {
    
    delay(10000);
}
