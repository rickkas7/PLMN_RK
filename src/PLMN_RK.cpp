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
String PLMN_RK::MccMnc::toString() const {
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

String PLMN_RK::MccMnc::toPLMN() const {
    String result;

    if (!isClear()) {
        // MCCMNC: abcdef -> bafced
        // or: 012345 -> 105243
        char resultArray[7];
        resultArray[6] = 0;

        String s = toString();
        if (s.length() == 5) {
            s += "F";
        }
        resultArray[0] = s.charAt(1);
        resultArray[1] = s.charAt(0);
        resultArray[2] = s.charAt(5);
        resultArray[3] = s.charAt(2);
        resultArray[4] = s.charAt(4);
        resultArray[5] = s.charAt(3);
        
        result = resultArray;
    }
    else {
        result = "ffffff";
    }

    return result;
}

void PLMN_RK::MccMnc::fromPLMN(const char *str) {
    clear();

    if (strcmp(str, "ffffff") == 0) {
        return;
    }

    String s = str;
    while(s.length() < 6) {
        s = "0" + s;
    }

    if (s.length() == 6) {
		// abcdef    badfec
		// 012345    103542
        char mccStr[4] = {0};
        char mncStr[4] = {0};

        mccStr[0] = s.charAt(1);
        mccStr[1] = s.charAt(0); 
        mccStr[2] = s.charAt(3);
        mncStr[0] = s.charAt(5);
        mncStr[1] = s.charAt(4);

        if (s.charAt(2) == 'F' || s.charAt(2) == 'f') {
            twoDigitMnc = true;
        }
        else {
            mncStr[2] = s.charAt(2);
        }
        mcc = atoi(mccStr);
        mnc = atoi(mncStr);
    }
}



//
// PLMNList
//
PLMN_RK::PLMNList &PLMN_RK::PLMNList::operator=(const PLMNList &other) {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        networks[ii] = other.networks[ii];
    }
    return *this;
}


void PLMN_RK::PLMNList::clear() {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        networks[ii].clear();
    }
}

bool PLMN_RK::PLMNList::isClear() const {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        if (!networks[ii].isClear()) {
            return false;
        }
    }
    return true;
}

bool PLMN_RK::PLMNList::contains(MccMnc value) const {
    for(size_t ii = 0; ii < kNetworksMaxSize; ii++) {
        if (networks[ii] == value) {
            return true;
        }
    }
    return false;
}

