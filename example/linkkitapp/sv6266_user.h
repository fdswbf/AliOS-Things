#ifndef __SV6266_USER_H__
#define __SV6266_USER_H__

#define KEY_WIFI_CONFIG     0//12  0
#define LED_WIFI_STATUS     1//10  13
//#define LED_RYL1            8
//#define LED_RYL2            20
//#define LED_RYL3            2
#define RYL_CHANNAL_MAX     3

//#define TEST_LED            11

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



#define CF_CAP_PIN                      12    //green line
#define CF1_CAP_PIN                     10    //orange line
#define SEL_CONTROL_PIN                 2    //white line
#define RELAY_PIN                       13    //black line
#define RELAY_LED_PIN                   11    //purple line

#define TMR_ID                          1
#define U_FREQ                          1400
#define I_FREQ                          80
#define P_FREQ                          100
#define KU_PARAM                        (220UL*1000000/U_FREQ)
#define KI_PARAM                        (1.0*1000000/I_FREQ)
#define KP_PARAM                        (220UL*1000000/P_FREQ)


//#define TM_MODE_AUTO_RELOAD             1
//#define IRQ_UTIMER_0                    20

typedef struct
{
    unsigned int tmr_overflow_counts;
	unsigned int last_time_us;
	unsigned int current_time_us;
    unsigned int period_time_us;
}TmrMeasureInfo_t;

extern TmrMeasureInfo_t cf1_cap_info;
extern TmrMeasureInfo_t cf_cap_info;

extern int upStreamData;
extern int switchStatusChange;
extern dev_status_e userDevStatus;
extern switchParm_t switchParm[RYL_CHANNAL_MAX];



extern void key_pull_func(void *arg);
extern void set_ryl_output(int gpioNum,int value);
#endif

