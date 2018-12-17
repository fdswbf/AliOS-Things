#include "hal/soc/soc.h"
#include <aos/kernel.h>
#include "soc_defs.h"
#include <stdio.h>
#include <aos/aos.h>
#include "sv6266_user.h"
#include <string.h>

#define LED_PARTITION 6
/* Logic partition on flash devices */
hal_logic_partition_t hal_partitions[HAL_PARTITION_MAX];

static gpio_dev_t gpio_key_wifi_config;
static gpio_dev_t gpio_output_config;


void set_ryl_toggle(int gpioNum)
{
    gpio_output_config.port = gpioNum;
    hal_gpio_output_toggle(&gpio_output_config);
}

void sv6266_flash_write(char *inbuf,char len)
{
    int blk_size;
    uint32_t offset = 12288;
    char buf[10] = {0};
   
    blk_size = len;
    memcpy(buf, inbuf, len);
    //offset = 3*blk_size;
    printf("buf[0]------- = %d\n",buf[0]);
    hal_flash_erase(LED_PARTITION, offset, blk_size);
    hal_flash_write(LED_PARTITION, &offset, buf, blk_size);
}

void sv6266_flash_read(char *outbuf)
{
    uint32_t offset = 12288;
    char buf[10] = {0};
    hal_flash_read(LED_PARTITION, &offset, buf, 1);
    printf("buf[0] = %d\n",buf[0]);
    memcpy(outbuf, buf, 1);
    printf("outbuf = %d\r\n",outbuf[0]);

}

void key_pull_func(void *arg)
{ 
    static uint32_t level;
    static uint64_t timer;
    static uint32_t times;

    hal_gpio_input_get(&gpio_key_wifi_config, &level);
    if (level == 0) {
        timer++;
        if (timer >= 20) {   
            if (times == 0) {
                aos_post_event(EV_KEY, CODE_BOOT, VALUE_KEY_LTCLICK); 
               
            }
            times++;
        }
    } else {
        if ((timer >= 1) && (timer <= 50)) {
            aos_post_event(EV_KEY, CODE_BOOT, VALUE_KEY_CLICK); 
            //set_ryl_toggle(LED_RYL1);
            //set_ryl_toggle(LED_RYL2);
            //set_ryl_toggle(LED_RYL3);
            //set_ryl_toggle(TEST_LED);
        }
        times = 0;
        timer = 0; 
    }
    
    aos_post_delayed_action(100, key_pull_func, NULL);
}


void set_ryl_output(int gpioNum,int value)
{
    //printf("\r\n[%d]:[%s]\r\n",__LINE__,__func__);
    //printf("\ngpio:%d  value:%d\n",gpioNum,value);
    gpio_output_config.port = gpioNum;

    if (value == 1) {
        hal_gpio_output_high(&gpio_output_config);
    } else if (value == 0) {
        hal_gpio_output_low(&gpio_output_config);
    }
}


static void sv6266_board_init(void)
{
    printf("\r\n[%d]:[%s]\r\n",__LINE__,__func__);
    gpio_key_wifi_config.port = KEY_WIFI_CONFIG;
    gpio_key_wifi_config.config = INPUT_PULL_UP;
    hal_gpio_init(&gpio_key_wifi_config);

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

    /*gpio_output_config.port = TEST_LED;
	gpio_output_config.config = OUTPUT_OPEN_DRAIN_PULL_UP;
	hal_gpio_init(&gpio_output_config);
    hal_gpio_output_low(&gpio_output_config);*/

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

