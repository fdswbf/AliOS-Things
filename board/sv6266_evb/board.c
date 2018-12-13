#include "hal/soc/soc.h"
#include <aos/kernel.h>
#include "soc_defs.h"
#include <stdio.h>
#include <aos/aos.h>

/* Logic partition on flash devices */
hal_logic_partition_t hal_partitions[HAL_PARTITION_MAX];


#define KEY_WIFI_CONFIG     12
#define LED_WIFI_STATUS     10
#define LED_RYL1            8
#define LED_RYL2            20
#define LED_RYL3            2

static gpio_dev_t gpio_key_wifi_config;
static gpio_dev_t gpio_output_config;

static void key_poll_func(void *arg)
{
    printf("\r\n[%d]:[%s]\r\n",__LINE__,__func__);
    #if 0
    uint32_t level;
    uint64_t diff;

    hal_gpio_input_get(&gpio_key_boot, &level);

    if (level == 0) {
        aos_post_delayed_action(10, key_poll_func, NULL);
    } else {
        diff = aos_now_ms() - elink_time;
		if (diff > 2000) { /* long press */
            elink_time = 0;
            aos_post_event(EV_KEY, CODE_BOOT, VALUE_KEY_LTCLICK);
        } else if (diff > 40) { /* short press */
            elink_time = 0;
            aos_post_event(EV_KEY, CODE_BOOT, VALUE_KEY_CLICK);
        } else {
            aos_post_delayed_action(10, key_poll_func, NULL);
        }
    }
    #endif
}

void set_ryl_output(int gpioNum,int value)
{
    printf("\r\n[%d]:[%s]\r\n",__LINE__,__func__);
    printf("\ngpio:%d  value:%d\n",gpioNum,value);
    gpio_output_config.port = gpioNum;

    if (value == 1) {
        hal_gpio_output_high(&gpio_output_config);
    } else if (value == 0) {
        hal_gpio_output_low(&gpio_output_config);
    }
}

static void key_proc_work(void *arg)
{
    aos_schedule_call(key_poll_func, NULL);
}


static void handle_elink_key(void *arg)
{
    uint32_t gpio_value;

    hal_gpio_input_get(&gpio_key_wifi_config, &gpio_value);
    if (gpio_value == 0) {
        hal_gpio_clear_irq(&gpio_key_wifi_config);
        aos_post_event(EV_KEY, CODE_BOOT, VALUE_KEY_CLICK);
        //elink_time = aos_now_ms();
        //aos_loop_schedule_work(0, key_proc_work, NULL, NULL, NULL);
    }
}

static void sv6266_board_init(void)
{
    printf("\r\n[%d]:[%s]\r\n",__LINE__,__func__);
    gpio_key_wifi_config.port = KEY_WIFI_CONFIG;
    gpio_key_wifi_config.config = INPUT_PULL_UP;
    hal_gpio_init(&gpio_key_wifi_config);

    hal_gpio_clear_irq(&gpio_key_wifi_config);
    hal_gpio_enable_irq(&gpio_key_wifi_config, IRQ_TRIGGER_FALLING_EDGE, handle_elink_key, NULL);

	gpio_output_config.port = LED_WIFI_STATUS;
	gpio_output_config.config = OUTPUT_OPEN_DRAIN_PULL_UP;
	hal_gpio_init(&gpio_output_config);
    hal_gpio_output_low(&gpio_output_config);

    gpio_output_config.port = LED_RYL1;
	gpio_output_config.config = OUTPUT_OPEN_DRAIN_PULL_UP;
	hal_gpio_init(&gpio_output_config);
    hal_gpio_output_low(&gpio_output_config);

    gpio_output_config.port = LED_RYL2;
	gpio_output_config.config = OUTPUT_OPEN_DRAIN_PULL_UP;
	hal_gpio_init(&gpio_output_config);
    hal_gpio_output_low(&gpio_output_config);

    gpio_output_config.port = LED_RYL3;
	gpio_output_config.config = OUTPUT_OPEN_DRAIN_PULL_UP;
	hal_gpio_init(&gpio_output_config);
    hal_gpio_output_low(&gpio_output_config);
}


void board_init(void)
{
    uint32_t main_partition_size = REG32(MAGIC_HEADER_BASE+OFFSET_PARTITION_MAIN_SIZE);
    uint32_t flash_total_size = REG32(MAGIC_HEADER_BASE+OFFSET_FLASH_TOTAL_SIZE);
    uint32_t up_partition_start_addr = 0;
    uint32_t up_partition_length = 0;
    /////HAL_PARTITION_APPLICATION
    hal_partitions[HAL_PARTITION_APPLICATION].partition_owner            = HAL_FLASH_EMBEDDED;
    hal_partitions[HAL_PARTITION_APPLICATION].partition_description      = "Application";
    hal_partitions[HAL_PARTITION_APPLICATION].partition_start_addr       = (up_partition_start_addr = 0x0);
    hal_partitions[HAL_PARTITION_APPLICATION].partition_length           = (up_partition_length = main_partition_size);
    hal_partitions[HAL_PARTITION_APPLICATION].partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN;
    printf("HAL_PARTITION_APPLICATION start 0x%lx, len %lu\n",
    hal_partitions[HAL_PARTITION_APPLICATION].partition_start_addr,
    hal_partitions[HAL_PARTITION_APPLICATION].partition_length);
    
    /////HAL_PARTITION_OTA_TEMP
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_owner               = HAL_FLASH_EMBEDDED;
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_description         = "OTA Storage";
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_start_addr          = (up_partition_start_addr = (up_partition_start_addr+up_partition_length));
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_length              = (up_partition_length = main_partition_size);
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_options             = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN;
    printf("HAL_PARTITION_OTA_TEMP start 0x%lx, len %lu\n",
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_start_addr,
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_length);

    /////HAL_PARTITION_PARAMETER_1
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_owner               = HAL_FLASH_EMBEDDED;
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_description         = "PARAMETER1";
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_start_addr          = (up_partition_start_addr = (up_partition_start_addr+up_partition_length));
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_length              = (up_partition_length = flash_total_size - (2*main_partition_size));
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_options             = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN;
    printf("HAL_PARTITION_PARAMETER_1 start 0x%lx, len %lu\n",
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_start_addr,
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_length);

    sv6266_board_init();
}

