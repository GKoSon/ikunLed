#ifndef __IKLED_H__
#define __IKLED_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "twinkle.h"





/* 控制命令 函数:led_control*/
#define LED_CMD_SET_STATE    0x01  /* 设置LED状态 */
#define LED_CMD_GET_STATE    0x02  /* 获取LED状态 */
#define LED_CMD_TOGGLE       0x03  /* 切换LED状态 */
#define LED_CMD_BLINK        0x04  /* LED 闪 */



typedef struct {
    rt_uint16_t count;  
    rt_uint16_t period;  
} led_blink_cfg_t;

/* LED设备结构体 */
struct ikled_device {
    struct rt_device parent;       /* 继承rt_device结构体 */

    rt_base_t  pin;                /* LED连接的引脚 */
    rt_uint8_t activeLevel;        /* 有效电平(1:高电平有效,0:低电平有效) */
    rt_uint8_t state;              /* 当前状态(1:亮,0:灭) */
    twinkle_t *blink;
};

typedef struct ikled_device *ikled_device_t;    /* 结构体指针类型 */

int rt_hw_led1_init(void);

#endif /* __DRV_LED_H__ */

