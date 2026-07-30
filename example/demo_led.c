#include <rtthread.h>
#include <rtdevice.h>


static void ledDev_sample(void) {
    rt_uint8_t state;

    /* 查找LED设备 */
    rt_device_t ledDev = rt_device_find(IKUNLED_DEMO_NAME);

    /* 打开LED设备(会点亮LED) */
    rt_device_open(ledDev, RT_DEVICE_OFLAG_RDWR);
    rt_device_read(ledDev, 0, &state, 1);
    rt_kprintf("LED state: %d \n", state);
  
    rt_thread_mdelay(1000);
    state=0;
    rt_device_write(ledDev, 0, &state, 1);
    rt_kprintf("LED state: %d \n", state);    
    
    /* 关闭LED */
    rt_device_close(ledDev);
}

MSH_CMD_EXPORT(ledDev_sample, ledDev_sample);












#include "ikLed.h"

static void ikled_blink(const char* name, rt_uint16_t count, rt_uint16_t period){
    rt_device_t ikDev = rt_device_find(name);
    led_blink_cfg_t blinkCfg;
    blinkCfg.count = count;
    blinkCfg.period = period;
    rt_device_control(ikDev, LED_CMD_BLINK,  (void *)&blinkCfg);
}

static void ikled_on(const char* name){
    rt_device_t ikDev = rt_device_find(name);
    rt_device_open(ikDev, RT_DEVICE_OFLAG_RDWR);
}

static void ikled_off(const char* name){
    rt_device_t ikDev = rt_device_find(name);
    rt_device_close(ikDev);
}


static void ikunLed_sample(void) {
    ikled_on(IKUNLED_DEMO_NAME);
    rt_thread_mdelay(1000);
    ikled_off(IKUNLED_DEMO_NAME);
    rt_thread_mdelay(1000);
    ikled_blink(IKUNLED_DEMO_NAME, 3, 200);
}

MSH_CMD_EXPORT(ikunLed_sample, ikunLed_sample);
