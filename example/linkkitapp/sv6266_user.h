#ifndef __SV6266_USER_H__
#define __SV6266_USER_H__

#define KEY_WIFI_CONFIG     0//12
#define LED_WIFI_STATUS     13//10
#define LED_RYL1            8
#define LED_RYL2            20
#define LED_RYL3            2

#define TEST_LED            11

typedef enum{
    DEV_WAIT_CONFIG         = 1,
    DEV_CONNECTED_AP        = 2,
    DEV_DISCONNECT_AP       = 3,
    DEV_CONNECTED_SERVER    = 4,
    DEV_DISCONNECT_SERVER   = 5,
    DEV_DEFAULT_STATUS      = 0xff,
}dev_status_e;         

extern dev_status_e userDevStatus;
extern void key_pull_func(void *arg);
extern void set_ryl_output(int gpioNum,int value);
#endif
