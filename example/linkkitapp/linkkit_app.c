/** USER NOTIFICATION
 *  this sample code is only used for evaluation or test of the iLop project.
 *  Users should modify this sample code freely according to the product/device TSL, like
 *  property/event/service identifiers, and the item value type(type, length, etc...).
 *  Create user's own execution logic for specific products.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <aos/aos.h>

#include "iot_import_product.h"
#include "linkkit_export.h"

#include "iot_import.h"

#include "linkkit_app.h"
#include "sv6266_user.h"

#define POST_WIFI_STATUS
/*
 * example for product "灯-Demo"
 */

#define LINKKIT_PRINTF(...)  \
    do {                                                     \
        printf("\e[0;32m%s@line%d:\t", __FUNCTION__, __LINE__);  \
        printf(__VA_ARGS__);                                 \
        printf("\e[0m");                                   \
    } while (0)

/* identifier of property/service/event, users should modify this macros according to your own product TSL. */
#define EVENT_ERROR_IDENTIFIER                 "Error"
#define EVENT_ERROR_OUTPUT_INFO_IDENTIFIER     "ErrorCode"
#define EVENT_CUSTOM_IDENTIFIER                "Custom"
#define EVENT_POST_IDENTIFIER                  "post"
#define EVENT_OUTPUTDATA_IDENTIFIER_SW0        "PowerSwitch"
#define EVENT_OUTPUTDATA_IDENTIFIER_SW1        "PowerSwitch_1"
#define EVENT_OUTPUTDATA_IDENTIFIER_SW2        "PowerSwitch_2"

/* specify ota buffer size for ota service, ota service will use this buffer for bin download. */
static void ota_init();

/* PLEASE set RIGHT tsl string according to your product. */
const char TSL_STRING[] =
    "{\"schema\":\"https://iotx-tsl.oss-ap-southeast-1.aliyuncs.com/schema.json\",\"profile\":{\"productKey\":\"a1qXi2sExXZ\"},\"services\":[{\"outputData\":[],\"identifier\":\"set\",\"inputData\":[{\"identifier\":\"PowerSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关\"},{\"identifier\":\"CountDownList\",\"dataType\":{\"specs\":[{\"identifier\":\"Target\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"操作对象\"},{\"identifier\":\"Contents\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"倒计时命令\"}],\"type\":\"struct\"},\"name\":\"倒计时列表\"},{\"identifier\":\"WIFI_Band\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"频段\"},{\"identifier\":\"WiFI_RSSI\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"-1\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信号强度\"},{\"identifier\":\"WIFI_AP_BSSID\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"热点BSSID\"},{\"identifier\":\"WIFI_Channel\",\"dataType\":{\"specs\":{\"min\":\"1\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信道\"},{\"identifier\":\"WiFI_SNR\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"127\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信噪比\"},{\"identifier\":\"PowerSwitch_1\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_1\"},{\"identifier\":\"PowerSwitch_2\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_2\"}],\"method\":\"thing.service.property.set\",\"name\":\"set\",\"required\":true,\"callType\":\"async\",\"desc\":\"属性设置\"},{\"outputData\":[{\"identifier\":\"PowerSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关\"},{\"identifier\":\"CountDownList\",\"dataType\":{\"specs\":[{\"identifier\":\"Target\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"操作对象\"},{\"identifier\":\"Contents\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"倒计时命令\"}],\"type\":\"struct\"},\"name\":\"倒计时列表\"},{\"identifier\":\"WIFI_Band\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"频段\"},{\"identifier\":\"WiFI_RSSI\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"-1\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信号强度\"},{\"identifier\":\"WIFI_AP_BSSID\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"热点BSSID\"},{\"identifier\":\"WIFI_Channel\",\"dataType\":{\"specs\":{\"min\":\"1\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信道\"},{\"identifier\":\"WiFI_SNR\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"127\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信噪比\"},{\"identifier\":\"PowerSwitch_1\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_1\"},{\"identifier\":\"PowerSwitch_2\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_2\"}],\"identifier\":\"get\",\"inputData\":[\"PowerSwitch\",\"CountDownList\",\"WIFI_Band\",\"WiFI_RSSI\",\"WIFI_AP_BSSID\",\"WIFI_Channel\",\"WiFI_SNR\",\"PowerSwitch_1\",\"PowerSwitch_2\"],\"method\":\"thing.service.property.get\",\"name\":\"get\",\"required\":true,\"callType\":\"async\",\"desc\":\"属性获取\"}],\"properties\":[{\"identifier\":\"PowerSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"CountDownList\",\"dataType\":{\"specs\":[{\"identifier\":\"Target\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"操作对象\"},{\"identifier\":\"Contents\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"倒计时命令\"}],\"type\":\"struct\"},\"name\":\"倒计时列表\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"WIFI_Band\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"频段\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"WiFI_RSSI\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"-1\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信号强度\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"WIFI_AP_BSSID\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"热点BSSID\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"WIFI_Channel\",\"dataType\":{\"specs\":{\"min\":\"1\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信道\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"WiFI_SNR\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"127\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信噪比\",\"accessMode\":\"rw\",\"required\":true},{\"identifier\":\"PowerSwitch_1\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_1\",\"accessMode\":\"rw\",\"required\":false},{\"identifier\":\"PowerSwitch_2\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_2\",\"accessMode\":\"rw\",\"required\":false}],\"events\":[{\"outputData\":[{\"identifier\":\"PowerSwitch\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关\"},{\"identifier\":\"CountDownList\",\"dataType\":{\"specs\":[{\"identifier\":\"Target\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"操作对象\"},{\"identifier\":\"Contents\",\"dataType\":{\"specs\":{\"length\":\"2048\"},\"type\":\"text\"},\"name\":\"倒计时命令\"}],\"type\":\"struct\"},\"name\":\"倒计时列表\"},{\"identifier\":\"WIFI_Band\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"频段\"},{\"identifier\":\"WiFI_RSSI\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"-1\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信号强度\"},{\"identifier\":\"WIFI_AP_BSSID\",\"dataType\":{\"specs\":{\"length\":\"255\"},\"type\":\"text\"},\"name\":\"热点BSSID\"},{\"identifier\":\"WIFI_Channel\",\"dataType\":{\"specs\":{\"min\":\"1\",\"unitName\":\"无\",\"max\":\"255\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信道\"},{\"identifier\":\"WiFI_SNR\",\"dataType\":{\"specs\":{\"min\":\"-127\",\"unitName\":\"无\",\"max\":\"127\",\"step\":\"1\"},\"type\":\"int\"},\"name\":\"信噪比\"},{\"identifier\":\"PowerSwitch_1\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_1\"},{\"identifier\":\"PowerSwitch_2\",\"dataType\":{\"specs\":{\"0\":\"关闭\",\"1\":\"开启\"},\"type\":\"bool\"},\"name\":\"电源开关_2\"}],\"identifier\":\"post\",\"method\":\"thing.event.property.post\",\"name\":\"post\",\"type\":\"info\",\"required\":true,\"desc\":\"属性上报\"},{\"outputData\":[{\"identifier\":\"ErrorCode\",\"dataType\":{\"specs\":{\"0\":\"恢复正常\"},\"type\":\"enum\"},\"name\":\"故障代码\"}],\"identifier\":\"Error\",\"method\":\"thing.event.Error.post\",\"name\":\"故障上报\",\"type\":\"info\",\"required\":true}]}";
/* user sample context struct. */
typedef struct _sample_context {
    const void   *thing;
    int           cloud_connected;
#ifdef LOCAL_CONN_ENABLE
    int           local_connected;
#endif
    int           thing_enabled;

    int           service_custom_input_transparency;
    int           service_custom_output_contrastratio;
#ifdef SERVICE_OTA_ENABLED
    char          ota_buffer[OTA_BUFFER_SIZE];
#endif /* SERVICE_OTA_ENABLED */
} sample_context_t;


sample_context_t g_sample_context;

void post_property_cb(const void *thing_id, int respons_id, int code, const char *response_message, void *ctx)
{
#if 0
    sample_context_t *sample_ctx = &g_sample_context;
    char event_output_identifier[64];
    snprintf(event_output_identifier, sizeof(event_output_identifier), "%s.%s", EVENT_ERROR_IDENTIFIER,
             EVENT_ERROR_OUTPUT_INFO_IDENTIFIER);

    int errorCode = 0;
    linkkit_set_value(linkkit_method_set_event_output_value,
                      sample_ctx->thing,
                      event_output_identifier,
                      &errorCode, NULL);

    return linkkit_trigger_event(sample_ctx->thing, EVENT_ERROR_IDENTIFIER, post_property_cb);
#endif
    printf("\n------------------------------------\n");
    printf("thing@%p: response arrived:\nid:%d\tcode:%d\tmessage:%s\n", thing_id, respons_id, code,
                   response_message == NULL ? "NULL" : response_message);
    printf("\n------------------------------------\n");
    LINKKIT_PRINTF("thing@%p: response arrived:\nid:%d\tcode:%d\tmessage:%s\n", thing_id, respons_id, code,
                   response_message == NULL ? "NULL" : response_message);
}

#ifdef LOCAL_CONN_ENABLE
static int on_connect(void *ctx, int cloud)
#else
static int on_connect(void *ctx)
#endif
{
    sample_context_t *sample_ctx = ctx;

    ota_init();

#ifdef LOCAL_CONN_ENABLE
    if (cloud) {
        sample_ctx->cloud_connected = 1;
        aos_post_event(EV_YUNIO, CODE_YUNIO_ON_CONNECTED, 0);
    } else {
        sample_ctx->local_connected = 1;
    }
    LINKKIT_PRINTF("%s is connected\n", cloud ? "cloud" : "local");
#else
    sample_ctx->cloud_connected = 1;
    LINKKIT_PRINTF("%s is connected\n", "cloud");
    aos_post_event(EV_YUNIO, CODE_YUNIO_ON_CONNECTED, 0);
#endif

    return 0;
}

#ifdef LOCAL_CONN_ENABLE
static int on_disconnect(void *ctx, int cloud)
#else
static int on_disconnect(void *ctx)
#endif
{
    sample_context_t *sample_ctx = ctx;

#ifdef LOCAL_CONN_ENABLE
    if (cloud) {
        sample_ctx->cloud_connected = 0;
        aos_post_event(EV_YUNIO, CODE_YUNIO_ON_DISCONNECTED, 0);
    } else {
        sample_ctx->local_connected = 0;
    }
    LINKKIT_PRINTF("%s is disconnect\n", cloud ? "cloud" : "local");
#else
    sample_ctx->cloud_connected = 0;
    aos_post_event(EV_YUNIO, CODE_YUNIO_ON_DISCONNECTED, 0);
    LINKKIT_PRINTF("%s is disconnect\n", "cloud");
#endif

    return 0;
}

static int raw_data_arrived(const void *thing_id, const void *data, int len, void *ctx)
{
    char raw_data[128] = {0};

    LINKKIT_PRINTF("raw data arrived,len:%d\n", len);

    /* do user's raw data process logical here. */

    /* ............................... */

    /* user's raw data process logical complete */

    snprintf(raw_data, sizeof(raw_data), "test down raw reply data %lld", (long long)HAL_UptimeMs());

    linkkit_invoke_raw_service(thing_id, 0, raw_data, strlen(raw_data));

    return 0;
}

static int thing_create(const void *thing_id, void *ctx)
{
    sample_context_t *sample_ctx = ctx;

    LINKKIT_PRINTF("new thing@%p created.\n", thing_id);
    sample_ctx->thing = thing_id;

    return 0;
}

static int thing_enable(const void *thing_id, void *ctx)
{
    sample_context_t *sample_ctx = ctx;

    sample_ctx->thing_enabled = 1;

    return 0;
}

static int thing_disable(const void *thing_id, void *ctx)
{
    sample_context_t *sample_ctx = ctx;

    sample_ctx->thing_enabled = 0;

    return 0;
}
#ifdef RRPC_ENABLED
static int handle_service_custom(sample_context_t *_sample_ctx, const void *thing, const char *service_identifier,
                                 int request_id, int rrpc)
#else
static int handle_service_custom(sample_context_t *_sample_ctx, const void *thing, const char *service_identifier,
                                 int request_id)
#endif /* RRPC_ENABLED */
{
    char identifier[128] = {0};
    sample_context_t *sample_ctx = _sample_ctx;

    /*
     * get iutput value.
     */
    snprintf(identifier, sizeof(identifier), "%s.%s", service_identifier, "transparency");
    linkkit_get_value(linkkit_method_get_service_input_value, thing, identifier,
                      &sample_ctx->service_custom_input_transparency, NULL);

    /*
     * set output value according to user's process result.
     */

    snprintf(identifier, sizeof(identifier), "%s.%s", service_identifier, "Contrastratio");

    sample_ctx->service_custom_output_contrastratio = sample_ctx->service_custom_input_transparency >= 0 ?
                                                      sample_ctx->service_custom_input_transparency : sample_ctx->service_custom_input_transparency * -1;

    linkkit_set_value(linkkit_method_set_service_output_value, thing, identifier,
                      &sample_ctx->service_custom_output_contrastratio, NULL);
#ifdef RRPC_ENABLED
    linkkit_answer_service(thing, service_identifier, request_id, 200, rrpc);
#else
    linkkit_answer_service(thing, service_identifier, request_id, 200);
#endif /* RRPC_ENABLED */

    return 0;
}
#ifdef RRPC_ENABLED
static int thing_call_service(const void *thing_id, const char *service, int request_id, int rrpc, void *ctx)
#else
static int thing_call_service(const void *thing_id, const char *service, int request_id, void *ctx)
#endif /* RRPC_ENABLED */
{
    sample_context_t *sample_ctx = ctx;

    LINKKIT_PRINTF("service(%s) requested, id: thing@%p, request id:%d\n",
                   service, thing_id, request_id);

    if (strcmp(service, "Custom") == 0) {
#ifdef RRPC_ENABLED
        handle_service_custom(sample_ctx, thing_id, service, request_id, rrpc);
#else
        handle_service_custom(sample_ctx, thing_id, service, request_id);
#endif /* RRPC_ENABLED */
    }

    return 0;
}

static int thing_prop_changed(const void *thing_id, const char *property, void *ctx)
{
    char *value_str = NULL;
    char property_buf[64] = {0};

    /* get new property value */
    if (strstr(property, "HSVColor") != 0) {
        int hue, saturation, value;

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Hue");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &hue, &value_str);

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Saturation");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &saturation, &value_str);

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Value");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &value, &value_str);

        LINKKIT_PRINTF("property(%s), Hue:%d, Saturation:%d, Value:%d\n", property, hue, saturation, value);

        /* XXX: do user's process logical here. */
    } else if (strstr(property, "HSLColor") != 0) {
        int hue, saturation, lightness;

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Hue");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &hue, &value_str);

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Saturation");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &saturation, &value_str);

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Lightness");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &lightness, &value_str);

        LINKKIT_PRINTF("property(%s), Hue:%d, Saturation:%d, Lightness:%d\n", property, hue, saturation, lightness);
        /* XXX: do user's process logical here. */
    }  else if (strstr(property, "RGBColor") != 0) {
        int red, green, blue;

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Red");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &red, &value_str);

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Green");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &green, &value_str);

        snprintf(property_buf, sizeof(property_buf), "%s.%s", property, "Blue");
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property_buf, &blue, &value_str);

        LINKKIT_PRINTF("property(%s), Red:%d, Green:%d, Blue:%d\n", property, red, green, blue);
        /* XXX: do user's process logical here. */
    } else {
        linkkit_get_value(linkkit_method_get_property_value, thing_id, property, NULL, &value_str);

        LINKKIT_PRINTF("property(%s) new value set: %s\n", property, value_str);
    }

    /* do user's process logical here. */
    linkkit_post_property(thing_id, property, post_property_cb);
    return 0;
}

static int linkit_data_arrived(const void *thing_id, const void *params, int len, void *ctx)
{
    LINKKIT_PRINTF("thing@%p: masterdev_linkkit_data(%d byte): %s\n", thing_id, len, (const char *)params);
    return 0;
}

static linkkit_ops_t alink_ops = {
    .on_connect           = on_connect,
    .on_disconnect        = on_disconnect,
    .raw_data_arrived     = raw_data_arrived,
    .thing_create         = thing_create,
    .thing_enable         = thing_enable,
    .thing_disable        = thing_disable,
    .thing_call_service   = thing_call_service,
    .thing_prop_changed   = thing_prop_changed,
    .linkit_data_arrived  = linkit_data_arrived,
};

#if 0
static unsigned long long uptime_sec(void)
{
    static unsigned long long start_time = 0;

    if (start_time == 0) {
        start_time = HAL_UptimeMs();
    }

    return (HAL_UptimeMs() - start_time) / 1000;
}
#endif

int post_event_switch(sample_context_t *sample,char* switchChannel,int value)
{
    //char event_output_identifier[64];

    //snprintf(event_output_identifier, sizeof(event_output_identifier), "%s.%s", \
    EVENT_POST_IDENTIFIER,switchChannel);



    value += '0';

    int ret = linkkit_set_value(linkkit_method_set_property_value,
                      sample->thing,
                      switchChannel,
                      NULL, &value);  
    printf("\n------------------------------------\n");
    printf("%s value:%d ret:%d",switchChannel,value,ret);
    printf("\n------------------------------------\n");
    return linkkit_post_property(sample->thing,switchChannel,post_property_cb);
    //return linkkit_trigger_event(sample->thing, EVENT_POST_IDENTIFIER, post_property_cb);
}

static int post_event_error(sample_context_t *sample)
{
    char event_output_identifier[64];
    snprintf(event_output_identifier, sizeof(event_output_identifier), "%s.%s", EVENT_ERROR_IDENTIFIER,
             EVENT_ERROR_OUTPUT_INFO_IDENTIFIER);

    int errorCode = 0;
    linkkit_set_value(linkkit_method_set_event_output_value,
                      sample->thing,
                      event_output_identifier,
                      &errorCode, NULL);

    return linkkit_trigger_event(sample->thing, EVENT_ERROR_IDENTIFIER, post_property_cb);
}

static int is_active(sample_context_t *sample_ctx)
{
#ifdef LOCAL_CONN_ENABLE
    return (sample_ctx->cloud_connected && sample_ctx->thing_enabled) || (sample_ctx->local_connected &&
                                                                          sample_ctx->thing_enabled);
#else
    return sample_ctx->cloud_connected && sample_ctx->thing_enabled;
#endif
}

#ifdef POST_WIFI_STATUS
static int post_property_wifi_status_once(sample_context_t *sample_ctx)
{
    int ret = -1;
    static char post_idx = 0;

    if (is_active(sample_ctx) && post_idx < 5) {
        hal_wireless_info_t wireless_info;
        memset(&wireless_info, 0x0, sizeof(hal_wireless_info_t));
        HAL_GetWirelessInfo(&wireless_info);

        if (post_idx == 0) {
            int i = 0;
            uint8_t bssid[6] = {0};
            char val_buf[20] = {0};
            char *band = NULL;

            band = wireless_info.band == 0 ? "2.4G" : "5G";

            HAL_Wifi_Get_Ap_Info(NULL, NULL, bssid);
            for (i = 0; i < 6; i ++) {
                snprintf(val_buf + strlen(val_buf), sizeof(val_buf) - strlen(val_buf), "%02X:", bssid[i]);
            }
            if (strlen(val_buf) > 0 && val_buf[strlen(val_buf) - 1] == ':') {
                val_buf[strlen(val_buf) - 1] = '\0';
            }

            linkkit_set_value(linkkit_method_set_property_value, sample_ctx->thing, "WIFI_Band", band, NULL);
            linkkit_set_value(linkkit_method_set_property_value, sample_ctx->thing, "WIFI_AP_BSSID", val_buf, NULL);
        } else {
            int post_array[5] = {0};
            char *post_key[5] = {"WIFI_Channel", "WiFI_RSSI", "WiFI_SNR", "WIFI_Tx_Rate", "WIFI_Rx_Rate"};

            post_array[0] = wireless_info.channel;
            post_array[1] = wireless_info.rssi;
            post_array[2] = wireless_info.snr;
            post_array[3] = wireless_info.tx_rate;
            post_array[4] = wireless_info.rx_rate;
            linkkit_set_value(linkkit_method_set_property_value, sample_ctx->thing,
                              post_key[post_idx - 1], &post_array[post_idx - 1], NULL);
        }

        post_idx ++;
        ret = 0;
    }
    return ret;
}
#endif

dev_status_e userDevStatus;
void user_wifi_status(void *arg)
{
    static unsigned char ledStatus = 0; 
    static unsigned long long now  = 0;
    now += 1;
    static unsigned char pre_status;

    switch(userDevStatus)
    {
        case DEV_WAIT_CONFIG:
            //every 200ms blink led
            if (ledStatus) {
                ledStatus = 0;
                set_ryl_output(LED_WIFI_STATUS,0);
            } else if (!ledStatus) {
                ledStatus = 1;
                set_ryl_output(LED_WIFI_STATUS,1);
            }
        break;

        

        case DEV_CONNECTED_SERVER:
            ledStatus = 1;
            set_ryl_output(LED_WIFI_STATUS,1);
        break;

        case DEV_CONNECTED_AP:
        case DEV_DISCONNECT_SERVER:
            //every 1s blink led
            if ((now % 5) == 0) {
                if (ledStatus) {
                    ledStatus = 0;
                    set_ryl_output(LED_WIFI_STATUS,0);
                } else if (!ledStatus) {
                    ledStatus = 1;
                    set_ryl_output(LED_WIFI_STATUS,1);
                }
            }
        break;

        case DEV_DISCONNECT_AP:
        case DEV_DEFAULT_STATUS:
            ledStatus = 1;
            set_ryl_output(LED_WIFI_STATUS,1);
        break;

        default:
        break;
    }
    if ((now % 10) == 0) {
        LINKKIT_PRINTF("====================%d====================\n",userDevStatus);
    }
    aos_post_delayed_action(100, user_wifi_status, NULL);
}

int switchStatusChange = 0;
void linkkit_action(void *params)
{
    static unsigned long long now = 0;
    //static int now_size = 0;
    //static int last_size = 0;

    sample_context_t *sample_ctx = params;

    linkkit_dispatch();

    now += 1;

#ifdef POST_WIFI_STATUS
    if ((now & 0x02) == 0) {
        post_property_wifi_status_once(sample_ctx);
    }
#endif

#if 1
    /* about 60 seconds, assume trigger event about every 60s. */
    if (now % 600 == 0 && is_active(sample_ctx)) {
        int id_send = 0;
        int ret;
        LINKKIT_PRINTF("====================Thread1====================\n");
        ret = post_event_error(sample_ctx);
        if (ret > 0) {
            id_send = ret;
            LINKKIT_PRINTF("send id:%d\n", id_send);
        }
    }
    if (is_active(sample_ctx)) {
        static int ret0  = 0;
        static int ret1  = 0;
        static int ret2  = 0;
        static int value = 0;

        if (switchStatusChange == 1) {
            if ((switchParm[0].value == switchParm[1].value) && \
            (switchParm[2].value == switchParm[1].value) && \
            ((ret0 == 0) && (ret1 == 0) && (ret2 == 0))) {
                if (switchParm[2].value) value = 0;
                else value = 1;
            } else if ((ret0 == 0) && (ret1 == 0) && (ret2 == 0)) {
                value = 0;
            }

            if (!ret0) {
                ret0 = post_event_switch(sample_ctx,EVENT_OUTPUTDATA_IDENTIFIER_SW0,value);
                set_ryl_output(LED_RYL1,value);
                //switchParm[0].value = value;
            } else if (!ret1) {
                ret1 = post_event_switch(sample_ctx,EVENT_OUTPUTDATA_IDENTIFIER_SW1,value);
                set_ryl_output(LED_RYL2,value);
                //switchParm[1].value = value;
            } else if (!ret2) {
                ret2 = post_event_switch(sample_ctx,EVENT_OUTPUTDATA_IDENTIFIER_SW2,value);
                set_ryl_output(LED_RYL3,value);
                //switchParm[2].value = value;
            }
        }

        if (ret0 && ret1 && ret2) {
            switchStatusChange = 0;
            ret0 = 0;
            ret1 = 0;
            ret2 = 0;
        }
        #if 0
        if (upStreamData == 1) {
            upStreamData = 0;
            post_event_switch(sample_ctx,EVENT_OUTPUTDATA_IDENTIFIER_SW0,switchParm[0].value);
        } else if (upStreamData == 2) {
            upStreamData = 0;
            post_event_switch(sample_ctx,EVENT_OUTPUTDATA_IDENTIFIER_SW1,switchParm[1].value);
        } else if (upStreamData == 3) {
            upStreamData = 0;
            post_event_switch(sample_ctx,EVENT_OUTPUTDATA_IDENTIFIER_SW2,switchParm[2].value);
        } 
        #endif
    }
#endif

#if 0
    now_size = system_get_free_heap_size();
    if (now_size != last_size) {
        last_size = now_size;
        if ((now_size - last_size) > 256 ||  (last_size - now_size) > 256) {
            LINKKIT_PRINTF("[heap check task] free heap size:%d\n", now_size);
        }
    } else if (now % 600 == 0) {
        LINKKIT_PRINTF("[heap check task] free heap size:%d Bytes(now time:%d)\n", now_size, now);
    }
#endif

    aos_post_delayed_action(100, linkkit_action, sample_ctx);
}

void linkkit_set_post_thread_action(void *params)
{
    static unsigned long long now = 0;
    //static int now_size = 0;
    //static int last_size = 0;

    sample_context_t *sample_ctx = params;

    //linkkit_dispatch();

    now += 1;

#ifdef POST_WIFI_STATUS
    if ((now & 0x02) == 0) {
        post_property_wifi_status_once(sample_ctx);
    }
#endif

#if 1
    /* about 60 seconds, assume trigger event about every 60s. */
    if (now % 600 == 0 && is_active(sample_ctx)) {
        int id_send = 0;
        int ret;
        LINKKIT_PRINTF("====================Thread2====================\n");
        ret = post_event_error(sample_ctx);
        if (ret > 0) {
            id_send = ret;
            LINKKIT_PRINTF("send id:%d\n", id_send);
        }
    }
#endif

#if 0
    now_size = system_get_free_heap_size();
    if (now_size != last_size) {
        last_size = now_size;
        if ((now_size - last_size) > 256 ||  (last_size - now_size) > 256) {
            LINKKIT_PRINTF("[heap check task] free heap size:%d\n", now_size);
        }
    } else if (now % 600 == 0) {
        LINKKIT_PRINTF("[heap check task] free heap size:%d Bytes(now time:%d)\n", now_size, now);
    }
#endif

    aos_post_delayed_action(100, linkkit_set_post_thread_action, sample_ctx);
}

int get_tsl_from_cloud = 0;

void linkkit_set_tsl_action(void *params)
{
    LINKKIT_PRINTF("tsl_from_cloud %d\n", get_tsl_from_cloud);
    if (!get_tsl_from_cloud) {
        linkkit_set_tsl(TSL_STRING, strlen(TSL_STRING));
    }
}

void ntp_time_reply(const char *offset_time)
{
    LINKKIT_PRINTF("ntp time:%s\n", offset_time);
}

void linkkit_ntp_time_test(void *param)
{
    linkkit_ntp_time_request(ntp_time_reply);
}

int linkkit_main()
{
    sample_context_t *sample_ctx = &g_sample_context;

    LINKKIT_PRINTF("%s tsl from cloud\n", get_tsl_from_cloud == 0 ? "Not get" : "get");

    memset(sample_ctx, 0, sizeof(sample_context_t));
    sample_ctx->thing_enabled = 1;

    linkkit_start(16, get_tsl_from_cloud, linkkit_loglevel_debug, &alink_ops, linkkit_cloud_domain_shanghai, sample_ctx);

    memset(switchParm,0,(sizeof(switchParm_t))*RYL_CHANNAL_MAX);

    aos_post_delayed_action(5000, linkkit_set_tsl_action, NULL);
    aos_post_delayed_action(6000, linkkit_action, sample_ctx);
    aos_post_delayed_action(16000, linkkit_ntp_time_test, NULL);

    //int stack_used = 0;
    //aos_task_new("Thread2",linkkit_set_post_thread,NULL,1024);

    return 0;
}

int linkkit_app(void)
{
    linkkit_main();

    return 0;
}

static void ota_init()
{
    static int init = 0;
    if (init) {
        return;
    }
    init = 1;

    aos_post_event(EV_SYS, CODE_SYS_ON_START_FOTA, 0);
}

