#include "PLMN_RK.h"

PLMN_RK *PLMN_RK::_instance;

static Logger _log("PLMN");

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

#ifndef UNITTEST
void PLMN_RK::updateIfNecessary(std::function<void(PLMNList &list)> updaterFn) {
    PLMN_RK::PLMNList origList;
    PLMN_RK::instance().readList(origList);
    _log.info("PLMN at boot %s", origList.toCommaSeparatedMccMncString().c_str());

    PLMN_RK::PLMNList updatedList(origList);

    updaterFn(updatedList);
    
    if (!updatedList.isEqual(origList)) {
        _log.info("about to updateList %s", updatedList.toCommaSeparatedMccMncString().c_str());

        PLMN_RK::instance().updateList(updatedList);
    
        _log.info("power cycle modem");
        PLMN_RK::instance().modemPowerOffOn();    
    }
    else {
        _log.info("list unchanged");
    }
}
#endif // UNITTEST



#ifndef UNITTEST
bool PLMN_RK::readList(PLMN_RK::PLMNList &list) {
    bool result = false;

    int res = Cellular.command(readListCallback, &list, 10000, "AT+CRSM = 176,28539,0,0,12\r\n");
    if (res == RESP_OK) {
        result = true;
    }

    return result;
}
#endif // UNITTEST

#ifndef UNITTEST
// [static] 
int PLMN_RK::readListCallback(int type, const char* buf, int len, PLMNList* param) {
    if (type == TYPE_PLUS) {
        int start = -1, end = -1;

        for(int ii = 0; ii < len; ii++) {
            if (buf[ii] == '"') {
                if (start == -1) {
                    start = ii + 1;
                }
                else
                if (end == -1) {
                    end = ii;
                }
            }
        }
        if (start >= 0 && end >= 0) {
            String s(&buf[start], end - start);
            param->fromString(s);
        }
    }
    return WAIT;
}
#endif // UNITTEST

#ifndef UNITTEST
bool PLMN_RK::updateList(const PLMNList &list) {
    bool result = false;

    int res = Cellular.command(10000, "AT+CRSM=214,28539,0,0,12,\"%s\"\r\n", list.toString().c_str());
    if (res == RESP_OK) {
        result = true;
    }

    return result;
}
#endif // UNITTEST

#ifndef UNITTEST
bool PLMN_RK::modemPowerOffOn() {
    bool result = true;

    Cellular.off();
    waitFor(Cellular.isOff, 10000);
    Cellular.on();
    waitFor(Cellular.isOn, 10000);

    return result;
}
#endif // UNITTEST


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
        entries[ii] = other.entries[ii];
    }
    return *this;
}

bool PLMN_RK::PLMNList::isEqual(const PLMNList &other) const{
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (entries[ii].isEqual(other.entries[ii])) {
            return false;
        }
    }
    return true;
}



void PLMN_RK::PLMNList::clear() {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        entries[ii].clear();
    }
}

bool PLMN_RK::PLMNList::isClear() const {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (!entries[ii].isClear()) {
            return false;
        }
    }
    return true;
}

size_t PLMN_RK::PLMNList::getNumEntries() const {
    size_t result = 0;

    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (!entries[ii].isClear()) {
            result++;
        }
    }
    return result;
}


bool PLMN_RK::PLMNList::contains(MccMnc value) const {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (entries[ii] == value) {
            return true;
        }
    }
    return false;
}

bool PLMN_RK::PLMNList::add(MccMnc value) {
    if (!contains(value)) {
        for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
            if (entries[ii].isClear()) {
                entries[ii] = value;
                return true;
            }
        }
        return false;
    }
    return true;
}


ssize_t PLMN_RK::PLMNList::findIndex(MccMnc value) const {
    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        if (entries[ii] == value) {
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
        entries[ii] = entries[ii + 1];
    }
    entries[kPLMNListMaxEntries -1].clear();

    return true;
}


String PLMN_RK::PLMNList::toString() const {
    String result;

    for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
        result += entries[ii].toPLMN();
    }

    return result;
}

String PLMN_RK::PLMNList::toCommaSeparatedMccMncString() const {
    String result;

    size_t numEntries = getNumEntries();

    for(size_t ii = 0; ii < numEntries; ii++) {
        result += entries[ii].toString();

        if (ii < (numEntries - 1)) {
            result += String(", ");
        }
    }

    return result;
}


void PLMN_RK::PLMNList::fromString(const char *str) {
    if (str && strlen(str) == kPLMNListSize) {
        for(size_t ii = 0; ii < kPLMNListMaxEntries; ii++) {
            String s(&str[ii * kPLMNSize], kPLMNSize);
            entries[ii].fromPLMN(s);
        }
    }
}
