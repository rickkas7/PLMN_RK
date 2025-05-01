#ifndef __PLMN_RK_H
#define __PLMN_RK_H

#include "Particle.h"

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 */
class PLMN_RK {
public:
    static const size_t kPLMNSize = 6; //!< Number of digits in a PLMN (may also contain a literal 'F' character)

    static const size_t kPLMNListMaxEntries = 4; //!< Maximum number of entries in a FPLMN list

    static const size_t kPLMNListSize = kPLMNSize * kPLMNListMaxEntries;//!< Size in bytes of a FPLMN list


    /**
     * @brief Container to hold an MCC-MNC pair and convert between formats
     */
    class MccMnc {
    public:
        /**
         * @brief Construct an object with no values set (clear)
         */
        MccMnc() {};

        /**
         * @brief Construct an object with mcc and mnc. This assumes 3-digit MNC values; see withTwoDigitMnc()
         * 
         * @param mcc Mobile Country Code
         * @param mnc Mobile Network Code
         */
        MccMnc(int mcc, int mnc) { this->mcc = mcc; this->mnc = mnc; };

        /**
         * @brief Sets the 2-digit MNC flag
         * 
         * @param twoDigitMnc Set the 2-digit flag. Parameter is optional and the default is true.
         * @return MccMnc& Reference to this object so you can chain the withXXX() calls, fluent-style.
         * 
         * Some countries have a MNC (Mobile Network Code) that is only two digits long. The default for this class is
         * 3 digit, but you can select 2-digit with this method.
         */
        MccMnc &withTwoDigitMnc(bool twoDigitMnc = true) { this->twoDigitMnc = twoDigitMnc; return *this; };

        /**
         * @brief Gets the 2-digit MNC flag
         * 
         * @return true This object represents a 2-digit MNC.
         * @return false This object represents the 3-digit MNC, which is more common.
         * 
         * Some countries have a MNC (Mobile Network Code) that is only two digits long; this method returns the flag.
         */
        bool getTwoDigitMnc() const { return twoDigitMnc; };

        /**
         * @brief Sets the MCC (Mobile Country Code)
         * 
         * @param mcc 
         * @return MccMnc& 
         */
        MccMnc &withMcc(int mcc) { this->mcc = mcc; return *this; };

        /**
         * @brief Get the mobile country code value
         * 
         * @return int 
         */
        int getMcc() const { return mcc; };

        /**
         * @brief Sets the MNC (Mobile Network Code)
         * 
         * @param mnc 
         * @return MccMnc& 
         * 
         * See also withTwoDigitMnc() for details about 2-digit MNC codes, which cannot be inferred from the MNC value.
         */
        MccMnc &withMnc(int mnc) { this->mnc = mnc; return *this; };

        /**
         * @brief Get the mobile network code value
         * 
         * @return int 
         */
        int getMnc() const { return mnc; };

        /**
         * @brief Construct from a 5-digit or 6-digit MCC-MNC string, for example "310410" is MCC=310, MNC=410.
         * 
         * @param str 5 or 6 digit MCC-MNC string
         * 
         * If the string is 5 digits long, then the object will have a two digit MNC.
         */
        MccMnc(const char *str) { fromString(str); };

        /**
         * @brief Construct an object as a copy of the other
         * 
         * @param other 
         */
        MccMnc(const MccMnc &other) { this->mcc = other.mcc; this->mnc = other.mnc; this->twoDigitMnc = other.twoDigitMnc; };

        /**
         * @brief Set this object's value to the same value as another object
         * 
         * @param other 
         * @return MccMnc& 
         */
        MccMnc &operator=(const MccMnc &other) { this->mcc = other.mcc; this->mnc = other.mnc; this->twoDigitMnc = other.twoDigitMnc; return *this; };

        /**
         * @brief Returns true if the mcc and mnc match other item
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool isEqual(const MccMnc &other) const { return this->mcc == other.mcc && this->mnc == other.mnc; };

        /**
         * @brief Returns true if the mcc and mnc match other item
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const MccMnc &other) const { return this->mcc == other.mcc && this->mnc == other.mnc; };


        /**
         * @brief Clear the value to mcc = 0, mnc = 0, twoDigitMnc = false
         */
        void clear() { mcc = mnc = 0; twoDigitMnc = false; };

        /**
         * @brief Returns true if the value is clear. This is also true when the default constructor is used
         * 
         * @return true Returns true of mcc == 0 and mnc == 0. Does not check twoDigitMnc.
         * @return false 
         */
        bool isClear() const { return mcc == 0 && mnc == 0; };

        /**
         * @brief Convert to a 5-digit or 6-digit MCC-MNC string, for example "310410" is MCC=310, MNC=410.
         * 
         * @return String A string value with 5 or 6 ASCII digits
         * 
         * The twoDigitMnc value determines whether the output will be 5 or 6 digits.
         */
        String toString() const;

        /**
         * @brief Set the value from a 5-digit or 6-digit MCC-MNC string, for example "310410" is MCC=310, MNC=410.
         * 
         * @param str 5 or 6 digit MCC-MNC string
         */
        void fromString(const char *str);

        /**
         * @brief Convert this MCC-MNC to a PLMN string (6 ASCII digits, may also contain a literal 'F' character)
         * 
         * @return String 
         */
        String toPLMN() const;

        /**
         * @brief Set this value from a PLMN string (6 ASCII digits, may also contain a literal 'F' character)
         * 
         * @param str 
         */
        void fromPLMN(const char *str);

    private:
        int mcc = 0; //!< Mobile Country Code
        int mnc = 0; //!< Mobile Network Code
        bool twoDigitMnc = false; //!< true if MNC is 2 digits instead of 3 digits.
    };

    /**
     * @brief Class to hold a list of up to 4 PLMN values
     */
    class PLMNList {
    public:
        /**
         * @brief Construct a new empty object
         * 
         */
        PLMNList() {};

        /**
         * @brief Destructor. No external storage is allocated for this object so it doesn't do anything.
         */
        virtual ~PLMNList() {};

        /**
         * @brief Construct an object as a copy of another object.
         * 
         * @param other 
         */
        PLMNList(const PLMNList &other) { *this = other; };

        /**
         * @brief Assign the PLMNs of this object to be the same as another object.
         * 
         * @param other 
         * @return PLMNList& 
         */
        PLMNList &operator=(const PLMNList &other);

        /**
         * @brief Returns true if two lists are equal. Items need to be in the same order to be considered equal.
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool isEqual(const PLMNList &other) const;

        
        /**
         * @brief Clear the list. This makes it "FFFFFFFFFFFFFFFFFFFFFFFF" as a string.
         */
        void clear();

        /**
         * @brief Returns true if the list is clear.
         * 
         * @return true 
         * @return false 
         */
        bool isClear() const;

        /**
         * @brief Return the number of non-cleared entries in the list
         * 
         * @return size_t 
         */
        size_t getNumEntries() const;

        /**
         * @brief Returns true if the specified MCC-MNC exists in this list.
         * 
         * @param value 
         * @return true 
         * @return false 
         */
        bool contains(MccMnc value) const;

        /**
         * @brief Get the MCC-MNC at a specific index of the object.
         * 
         * @param index 0-based index. Must be 0 <= index < kPLMNListMaxEntries. Invalid values will crash.
         * @return MccMnc Copy of 
         */
        MccMnc getAt(size_t index) const { return entries[index]; };

        /**
         * 
         * @brief Add an MCC-MNC to the list if it does not exist and there is space
         * 
         * @param value 
         * @return true Operation was successful
         * @return false List was full
         */
        bool add(MccMnc value);

        /**
         * @brief Add a MCC-MNC string (5 or 6 ASCII digits of the form 310410)if it does not exist and there is space
         * 
         * @param str 
         * @return true Operation was successful
         * @return false List was full
         */
        bool add(const char *str) { return add(MccMnc(str)); };

        /**
         * @brief Find the index (0-based) of a list item from its MCC-MNC
         * 
         * @param value 
         * @return size_t index (0-based) or -1 if not found
         */
        ssize_t findIndex(MccMnc value) const;

        /**
         * @brief Remove a MCC-MNC from the list
         * 
         * @param value 
         */
        bool remove(MccMnc value);

        /**
         * @brief Remove an entry from the list by its 0-based index
         * 
         * @param index 
         */
        bool remove(size_t index);

        /**
         * @brief Convert to a string to 24 digit string containing a FPLMN list
         * 
         * @return String 
         */
        String toString() const;

        /**
         * @brief Converts to a comma-separated list of MCCMNC values
         * 
         * @return String 
         */
        String toCommaSeparatedMccMncString() const;

        /**
         * @brief Set this object's value to a 24 digit string containing a FPLMN list
         * 
         * @param str 
         */
        void fromString(const char *str);

        /**
         * @brief Add United States AT&T to the list if it does not already exist
         */
        void add_US_ATT() { add(MccMnc("310410")); };

        /**
         * @brief Add United States AT&T to the list if it does not already exist
         */
        void add_US_TMobile() { add(MccMnc("310260")); };

        /**
         * @brief Add United States US Cellular to the list if it does not already exist. Note that this takes two entries!
         */
        void add_US_USCellular() { add(MccMnc("311580")); add(MccMnc("311588")); };

        /**
         * @brief Add United States Verizon to the list if it does not already exist
         */
        void add_US_Verizon() { add(MccMnc("311480")); };

    private:
        MccMnc entries[kPLMNListMaxEntries];
    };

    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use PLMN_RK::instance() to instantiate the singleton.
     */
    static PLMN_RK &instance();

#ifndef UNITTEST
    /**
     * @brief Utility function to get the FPLMN and update it only if necessary
     * 
     * @param updaterFn Function to call, typically a C++11 lambda.
     * 
     * See example 1-simple.cpp for how to use this function
     * 
     * Updater function prototype:
     * 
     * void updater(PLMN_RK::PLMNList &list)
     */
    void updateIfNecessary(std::function<void(PLMNList &list)> updaterFn);
#endif // UNITTEST


#ifndef UNITTEST
    /**
     * @brief Get the FPLMNList from the SIM.
     * 
     * @return PLMNList 
     * 
     * Cellular must be on for this call to be used.
     */
    bool readList(PLMNList &list);
#endif // UNITTEST

#ifndef UNITTEST
    /**
     * @brief Update the FPLMNList in the SIM.
     * 
     * @return PLMNList 
     * 
     * Cellular must be on for this call to be used.
     */
    bool updateList(const PLMNList &list);
#endif // UNITTEST

#ifndef UNITTEST
    /**
     * @brief Function to reset the SIM card and power cycle the modem
     * 
     * @return true 
     * @return false 
     */
    bool resetModemAndSim();
#endif // UNITTEST


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
     * @brief Internal function used by readList as the callback to Cellular.command
     * 
     * @param type 
     * @param buf 
     * @param len 
     * @param param 
     * @return int 
     */
    static int readListCallback(int type, const char* buf, int len, PLMNList* param);

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