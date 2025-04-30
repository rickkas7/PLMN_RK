#ifndef __PLMN_RK_H
#define __PLMN_RK_H

#include "Particle.h"

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * From global application setup you must call:
 * PLMN_RK::instance().setup();
 * 
 * From global application loop you must call:
 * PLMN_RK::instance().loop();
 */
class PLMN_RK {
public:
    /**
     * @brief Container to hold an MCC-MNC pair and convert between formats
     */
    class MccMnc {
    public:
        /**
         * @brief Construct an object with no values set
         */
        MccMnc() {};

        /**
         * @brief Construct an object with mcc and mnc. This assumes 3-digit MNC values; see withTwoDigitMnc()
         * 
         * @param mcc Mobile Country Code
         * @param mnc Mobile Network Code
         */
        MccMnc(int mcc, int mnc) { this->mcc = mcc; this->mnc = mnc; };

        MccMnc &withTwoDigitMnc(bool twoDigitMnc = true) { this->twoDigitMnc = twoDigitMnc; return *this; };
        bool getTwoDigitMnc() const { return twoDigitMnc; };

        MccMnc &withMcc(int mcc) { this->mcc = mcc; return *this; };
        int getMcc() const { return mcc; };

        MccMnc &withMnc(int mnc) { this->mnc = mnc; return *this; };
        int getMnc() const { return mnc; };

        MccMnc(const char *str) { fromString(str); };

        MccMnc(const MccMnc &other) { this->mcc = other.mcc; this->mnc = other.mnc; this->twoDigitMnc = other.twoDigitMnc; };
        MccMnc &operator=(const MccMnc &other) { this->mcc = other.mcc; this->mnc = other.mnc; this->twoDigitMnc = other.twoDigitMnc; return *this; };

        bool operator==(const MccMnc &other) const { return this->mcc == other.mcc && this->mnc == other.mnc && this->twoDigitMnc == other.twoDigitMnc;; };

        void clear() { mcc = mnc = 0; twoDigitMnc = false; };

        bool isClear() const { return mcc == 0 && mnc == 0; };

        String toString();

        void fromString(const char *str);


    private:
        int mcc = 0;
        int mnc = 0;
        bool twoDigitMnc = false;
    };

    class FPLMN {
    public:
        FPLMN() {};
        virtual ~FPLMN() {};

        FPLMN(const FPLMN &other) { *this = other; };
        FPLMN &operator=(const FPLMN &other);

        void clear();

        bool isClear() const;

        bool contains(MccMnc value) const;

    private:
        static const size_t kNetworksMaxSize = 4;
        MccMnc networks[kNetworksMaxSize];
    };

    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use PLMN_RK::instance() to instantiate the singleton.
     */
    static PLMN_RK &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use PLMN_RK::instance().setup();
     */
    void setup();

    /**
     * @brief Perform application loop operations; call this from global application loop()
     * 
     * You typically use PLMN_RK::instance().loop();
     */
    void loop();

    /**
     * @brief Locks the mutex that protects shared resources
     * 
     * This is compatible with `WITH_LOCK(*this)`.
     * 
     * The mutex is not recursive so do not lock it within a locked section.
     */
    void lock() { mutex.lock(); };

    /**
     * @brief Attempts to lock the mutex that protects shared resources
     * 
     * @return true if the mutex was locked or false if it was busy already.
     */
    bool tryLock() { return mutex.try_lock(); };

    /**
     * @brief Unlocks the mutex that protects shared resources
     */
    void unlock() { mutex.unlock(); };


protected:
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use PLMN_RK::instance() to instantiate the singleton.
     */
    PLMN_RK();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~PLMN_RK();

    /**
     * This class is a singleton and cannot be copied
     */
    PLMN_RK(const PLMN_RK&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    PLMN_RK& operator=(const PLMN_RK&) = delete;

    /**
     * @brief Mutex to protect shared resources
     * 
     * This is initialized in setup() so make sure you call the setup() method from the global application setup.
     */
    Mutex mutex;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static PLMN_RK *_instance;

};
#endif  /* __PLMN_RK_H */