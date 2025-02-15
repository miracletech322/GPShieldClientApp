#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <QString>

#define APP_NAME "GPShield Client Application"

#define APP_REG_COMPANY "MiracleTech"
#define APP_REG_NAME "GPShieldApp"

#define REG_KEY_USERNAME "Username"
#define REG_KEY_IPADDRESS "IpAddress"
#define REG_KEY_DEPART "Department"

enum RequestHeader {
    RH_SET_NAME = 0,
    RH_SEND_ALERT,
    RH_RECEIVED_ALERT,
};

#endif // GLOBAL_VARS_H
