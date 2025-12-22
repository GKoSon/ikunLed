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
typedef struct ikled_device ikled_device_t;       /* 结构体类型 */
typedef struct ikled_device *ikled_device_ptr;    /* 结构体指针类型 */

rt_err_t rt_hw_led_init(const char *name, rt_base_t pin, rt_uint8_t activeLevel) ;



/*****************************************画蛇添足 二次封装*****************************************************/
/*****************************************画蛇添足 二次封装*****************************************************/
//其实，上面函数全部可以隐藏了  直接使用下面的函数 即可
int ikled_init(const char *name, rt_base_t pin, rt_uint8_t activeLevel);
void ikled_blink(const char* name, rt_uint16_t count, rt_uint16_t period);
void ikled_on(const char* name);
void ikled_off(const char* name);

#endif /* __DRV_LED_H__ */

