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
        result = "FFFFFF";
    }

    return result;
}

void PLMN_RK::MccMnc::fromPLMN(const char *str) {
    clear();

    if (strcmp(str, "FFFFFF") == 0) {
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
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        networks[ii] = other.networks[ii];
    }
    return *this;
}


void PLMN_RK::PLMNList::clear() {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        networks[ii].clear();
    }
}

bool PLMN_RK::PLMNList::isClear() const {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (!networks[ii].isClear()) {
            return false;
        }
    }
    return true;
}

size_t PLMN_RK::PLMNList::getNumEntries() const {
    size_t result = 0;

    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (!networks[ii].isClear()) {
            result++;
        }
    }
    return result;
}


bool PLMN_RK::PLMNList::contains(MccMnc value) const {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (networks[ii] == value) {
            return true;
        }
    }
    return false;
}

bool PLMN_RK::PLMNList::add(MccMnc value) {
    if (!contains(value)) {
        for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
            if (networks[ii].isClear()) {
                networks[ii] = value;
                return true;
            }
        }
        return false;
    }
    return true;
}


ssize_t PLMN_RK::PLMNList::findIndex(MccMnc value) const {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (networks[ii] == value) {
            return ii;
        }
    }

    return -1;
}

bool PLMN_RK::PLMNList::remove(MccMnc value) {
    ssize_t index = findIndex(value);
    if (index >= 0) {
        return remove(index);
    }
    else {
        return false;
    }
}


bool PLMN_RK::PLMNList::remove(size_t index) {
    if (index >= kPLMNListMaxEntries) {
        return false;
    }
    for(size_t ii = index; ii < (kPLMNListMaxEntries - 1); ii++) {
        networks[ii] = networks[ii + 1];
    }
    networks[kPLMNListMaxEntries -1].clear();

    return true;
}


String PLMN_RK::PLMNList::toString() const {
    String result;

    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        result += networks[ii].toPLMN();
    }

    return result;
}



void PLMN_RK::PLMNList::fromString(const char *str) {
    if (str && strlen(str) == kPLMNListSize) {
        for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
            String s(&str[ii * kPLMNSize], kPLMNSize);
            networks[ii].fromPLMN(s);
        }
    }
}
