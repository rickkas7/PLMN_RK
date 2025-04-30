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
}

void PLMN_RK::loop() {
    // Put your code to run during the application thread loop here
}


//
// MccMnc
//
String PLMN_RK::MccMnc::toString() {
    if (twoDigitMnc) {
        return String::format("%03d%02d", mcc, mnc);                
    }
    else {
        return String::format("%03d%03d", mcc, mnc);
    }
}

void PLMN_RK::MccMnc::fromString(const char *str) {
    mcc = mnc = 0;
    twoDigitMnc = false;

    size_t len = strlen(str);
    if (len == 5) {
        twoDigitMnc = true;
        sscanf(str, "%03d%02d", &mcc, &mnc);
    }
    else if (len == 6) {
        sscanf(str, "%03d%03d", &mcc, &mnc);
    }
}

//
// FPLMN
//
PLMN_RK::FPLMN &PLMN_RK::FPLMN::operator=(const FPLMN &other) {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        networks[ii] = other.networks[ii];
    }
    return *this;
}


void PLMN_RK::FPLMN::clear() {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        networks[ii].clear();
    }
}

bool PLMN_RK::FPLMN::isClear() const {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        if (!networks[ii].isClear()) {
            return false;
        }
    }
    return true;
}

bool PLMN_RK::FPLMN::contains(MccMnc value) const {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        if (networks[ii] == value) {
            return true;
        }
    }
    return false;
}

