# PLMN_RK

*Utility functions for manipulating cellular PLMNs on Particle cellular devices*

- Github Repository: https://github.com/rickkas7/PLMN_RK
- License: MIT
- Full browsable API documentation available [here](https://rickkas7.github.io/PLMN_RK/index.html) as well as in the docs directory 



These helper functions are used to convert between MCC-MNC (Mobile Country Code - Mobile Network Code) and
PLMN format. There are also functions for manipulating the FPLMN (Forbidden Public Land Mobile Network) list in the SIM. 

**FPLMN functions are not working in this version of the code**. They appear to be accepted by the cellular modem, but
the change never takes effect and do not appear to be saved on the SIM. I do not know why this is.

## FPLMN

The FPLMN (Forbidden Public Land Mobile Network) list is stored in the SIM and lists up to 4 PLMN to not connect to. 
Often, this list will be empty. It is also possible if your SIM or Device is banned from a network it will be added
automatically to the FPLMN so no more attempts will be made to connect to that network.

In theory, you can also add a specific carrier MCC-MNC to the list yourself. This would prevent connecting to that 
network. This is not recommended. If the remaining networks are unavailable, it will never fall back to a network
in the FPLMN. Also, on some SIMs, forcing a connection via a backup carrier could cause your SIM to be banned.

**In general you should never modify the FPLMN!**

## Usage

See example 1-simple:

Of note:

- Make sure you use `SYSTEM_MODE(SEMI_AUTOMATIC)` as you must update the FPLMN before connecting to cellular. You should also enable the system thread.

```cpp
SYSTEM_MODE(SEMI_AUTOMATIC);

// System thread defaults to on in 6.2.0 and later and this line is not required
#ifndef SYSTEM_VERSION_v620
SYSTEM_THREAD(ENABLED);
#endif
```

The code generally looks like this. In this example, AT&T in the United States is added the the FPLMN, which is not recommended.

```cpp
Cellular.on();
waitFor(Cellular.isOn, 10000);

Log.info("cellular on");

PLMN_RK::instance().updateIfNecessary([](PLMN_RK::PLMNList &list) {
    list.add_US_TMobile();
});
```

This will add the specified carrier to the list if it is not already in the list, and there's room. Note that if an update occurs,
it will take some time because it needs to be written, then the modem powered off and powered back on again.

There are other add* functions in the library for other US carriers, but in general you will use this form to add a single network:

```cpp
PLMN_RK::instance().updateIfNecessary([](PLMN_RK::PLMNList &list) {
    list.add("310410");
});
```

You can add multiple networks with multiple calls to add, though there is a limit of 4.

```cpp
PLMN_RK::instance().updateIfNecessary([](PLMN_RK::PLMNList &list) {
    list.add("311580"); 
    list.add("311588");
});
```



