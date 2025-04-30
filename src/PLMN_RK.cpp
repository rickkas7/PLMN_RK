#include "PLMN_RK.h"

PLMN_RK *PLMN_RK::_instance;

// [static]
PLMN_RK &PLMN_RK::instance() {
    if (!_instance) {
        _instance = new PLMN_RK();
    }
    return *_instance;
}

PLMN_RK::PLMN_RK() {
}

PLMN_RK::~PLMN_RK() {
}

void PLMN_RK::setup() {
    os_mutex_create(&mutex);

}

void PLMN_RK::loop() {
    // Put your code to run during the application thread loop here
}