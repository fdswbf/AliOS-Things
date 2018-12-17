#ifndef __SV6266_USER_H__
#define __SV6266_USER_H__

#define KEY_WIFI_CONFIG     12//12  0
#define LED_WIFI_STATUS     10//10  13
#define LED_RYL1            8
#define LED_RYL2            20
#define LED_RYL3            2
#define RYL_CHANNAL_MAX     3

#define TEST_LED            11

typedef enum{
    DEV_WAIT_CONFIG         = 1,
    DEV_CONNECTED_AP        = 2,
    DEV_DISCONNECT_AP       = 3,
    DEV_CONNECTED_SERVER    = 4,
    DEV_DISCONNECT_SERVER   = 5,
    DEV_DEFAULT_STATUS      = 0xff,
}dev_status_e;         

typedef struct{
    int channel ;
    int value ;
}switchParm_t;


extern int upStreamData;
extern int switchStatusChange;
extern dev_status_e userDevStatus;
extern switchParm_t switchParm[RYL_CHANNAL_MAX];



extern void key_pull_func(void *arg);
extern void set_ryl_output(int gpioNum,int value);
#endif

