#include "Particle.h"

#include "PLMN_RK.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// System thread defaults to on in 6.2.0 and later and this line is not required
#ifndef SYSTEM_VERSION_v620
SYSTEM_THREAD(ENABLED);
#endif

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

void setup() {
    // Comment out the next line in production
    waitFor(Serial.isConnected, 10000); delay(1000);

    Cellular.on();
    waitFor(Cellular.isOn, 10000);

    Log.info("cellular on");

    PLMN_RK::instance().updateIfNecessary([](PLMN_RK::PLMNList &list) {
        list.add_US_ATT();
        // list.clear();    
    });

    Log.info("connecting!");
    Particle.connect();
}

void loop() {
    
}
