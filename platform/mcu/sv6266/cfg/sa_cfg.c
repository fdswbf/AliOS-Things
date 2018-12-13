#include <stdint.h>
#include "flash.h"
#include "sa_cfg.h"
#include "osal.h"
#include <string.h>
#include <stdio.h>
#include "rf/rf_api.h"
#include "error.h"

#define CFG_ASSERT(cmp) \
    do { \
    } while(!(cmp))

extern const struct sa_cfg g_sa_cfg __attribute__((section(".sa_mp_data")));
const struct sa_cfg g_sa_cfg __attribute__((section(".sa_mp_data"))) = {
    .buf_mp = {0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x97,0x97,0x07,0x09,0x0A,0x07,0x07,0x07,0x07,0x07,0x00,
	           0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x97,0x97,0x07,0x09,0x0A,0x07,0x07,0x07,0x07,0x07,0x00,
			   0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x97,0x97,0x07,0x09,0x0A,0x07,0x07,0x07,0x07,0x07,0x00,
               0x04,0x07,
			   0x0D,0x0B,0x09,0x07,
			   0x0D,0x0B,0x09,0x07,
			   0x0D,0x0B,0x09,0x07,
			   0x23,0x5A,
			   0x00,0x00,
			   0x0D,0x0D,0x0A,0x08,0x4A,0x92,0x64,0x92,0xCC,0xB6,0xDB,0x96,
			   0x0D,0x0D,0x0A,0x08,0x4A,0x92,0x64,0x92,0xCC,0xB6,0xDB,0x96,
			   0x0D,0x0D,0x0A,0x08,0x4A,0x92,0x64,0x92,0xCC,0xB6,0xDB,0x96,
			   0x1E,0x14,0x7C,0x15,0x44,0x16
}
};

void cfg_dump_sa_pre_table() {
    int i;
    volatile uint8_t *ptr = (volatile uint8_t *)&g_sa_cfg;
    ptr -= 256;
    for (i = 0; i < 256; i++) {
        if ((i!=0) && (i%36 == 0)) {
            printf("\n");
        }
        printf("%02X-", ptr[i]);
    }
    printf("\n");
}
void cfg_dump_sa_post_table() {
    extern uint8_t __lds_sa_mp_data_end;
    int i;
    volatile uint8_t *ptr = (volatile uint8_t *)&__lds_sa_mp_data_end;
    for (i = 0; i < 256; i++) {
        if ((i!=0) && (i%36 == 0)) {
            printf("\n");
        }
        printf("%02X-", ptr[i]);
    }
    printf("\n");
}
void cfg_dump_sa_table(int len) {
    int i;
    volatile uint8_t *ptr = (volatile uint8_t *)&g_sa_cfg;
    for (i = 0; i < len; i++) {
        if ((i!=0) && (i%36 == 0)) {
            printf("\n");
        }
        printf("%02X-", ptr[i]);
    }
    printf("\n");
}

void cfg_sa_write_cfg(struct sa_cfg *new_cfg, uint16_t len) {
    CFG_ASSERT(len <= FLASH_SECTOR_SIZE);
    flash_init();
    uint8_t *buf = OS_MemAlloc(FLASH_SECTOR_SIZE);
    memcpy(&(buf[0]), (uint8_t*)new_cfg, len);
    memcpy(&(buf[len]), (((uint8_t *)&g_sa_cfg)+len), FLASH_SECTOR_SIZE-len);
    uint32_t ptr = (uint32_t)((uint32_t)(&g_sa_cfg) & (0xFFFFFF));
    OS_DeclareCritical();
    OS_EnterCritical();
    flash_sector_erase((unsigned int)ptr);
    int i;
    
    for (i = 0; i < FLASH_SECTOR_SIZE; i+=FLASH_PAGE_SIZE) {
        flash_page_program(ptr+i, FLASH_PAGE_SIZE, &(buf[i]));
    }
    OS_ExitCritical();
    OS_MemFree(buf);
}

struct st_rf_table ssv_rf_table;
    
int load_rf_table_from_flash()
{
    volatile uint8_t *ptr = (volatile uint8_t *)&g_sa_cfg;

    memcpy((uint8_t*)&ssv_rf_table, (uint8_t*)ptr, sizeof(ssv_rf_table));
    return 0;    
}

int save_rf_table_to_flash()
{
    uint8_t *ptr = (uint8_t *)OS_MemAlloc(sizeof(g_sa_cfg));
    if (ptr == NULL) 
    {
        printf("[%s] no memory, please check\n", __func__);
        return ERROR_MEMORY_FAILED;
    }
    memset(ptr, 0, sizeof(g_sa_cfg));
    memcpy(ptr, &ssv_rf_table, sizeof(ssv_rf_table));

    cfg_sa_write_cfg((struct sa_cfg*)ptr, sizeof(g_sa_cfg));
    OS_MemFree(ptr);
    return 0;
}

