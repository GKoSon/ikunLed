/*
 * ikLed.c
 * 首先制作一个设备名字是ikled_device 然后继续二次封装 
 * Change Logs:
 * Date           Author            Notes
 * 2025-10-14     koson            first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "ikLed.h"




/*************************************1>独立模块函数:为规范多态接口函数提供支持***********************************************/
static rt_err_t rt_led_set_state(ikled_device_ptr led, rt_uint8_t state) {
    if(led == RT_NULL)
        return -RT_ERROR;

    rt_pin_write(led->pin, (state == 1) ? led->activeLevel : !led->activeLevel);
    led->state = state;

    return RT_EOK;
}


static rt_err_t rt_led_get_state(ikled_device_ptr led, rt_uint8_t *state) {
    if(led == RT_NULL || state == RT_NULL)
        return -RT_ERROR;

    *state = led->state;
    return RT_EOK;
}


static rt_err_t rt_led_toggle(ikled_device_ptr led) {
    if(led == RT_NULL)
        return -RT_ERROR;

    return rt_led_set_state(led, !led->state);
}


//为blink功能设计A函数
static void led_on(void* dev) {
    ikled_device_ptr led = (ikled_device_ptr)dev;
    rt_led_set_state(led, 1);
}
//为blink功能设计B函数
static void led_off(void* dev) {
    ikled_device_ptr led = (ikled_device_ptr)dev;
    rt_led_set_state(led, 0);
}





/*************************************2>规范多态接口:对接设备驱动框架***********************************************/
static rt_err_t led_init(rt_device_t dev) {
    ikled_device_ptr led = (ikled_device_ptr)dev;

    rt_pin_mode(led->pin, PIN_MODE_OUTPUT);
    rt_pin_write(led->pin, !led->activeLevel);
    led->state = 0;

    return RT_EOK;
}

static rt_err_t led_open(rt_device_t dev, rt_uint16_t oflag) {
    ikled_device_ptr led = (ikled_device_ptr)dev;

    return rt_led_set_state(led, 1);
}

static rt_err_t led_close(rt_device_t dev) {
    ikled_device_ptr led = (ikled_device_ptr)dev;

    return rt_led_set_state(led, 0);
}

static rt_ssize_t led_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size) {
    if(buffer == RT_NULL || size == 0)
        return -RT_ERROR;

    ikled_device_ptr led = (ikled_device_ptr)dev;

    rt_uint8_t state;
    state = *(rt_uint8_t *)buffer;
    if(rt_led_set_state(led, state) == RT_EOK)
        return size;
    else
        return -RT_ERROR;
}

static rt_err_t led_control(rt_device_t dev, int cmd, void *args) {
    ikled_device_ptr led = (ikled_device_ptr)dev;

    rt_err_t result = RT_EOK;
    led_blink_cfg_t *blinkCfg;

    if(led == RT_NULL)
        return -RT_ERROR;

    switch(cmd) {
    case LED_CMD_SET_STATE:
        if(args != RT_NULL)
            result = rt_led_set_state(led, (rt_uint8_t)(uintptr_t)args);
        else
            result = -RT_ERROR;
        break;

    case LED_CMD_GET_STATE:
        if(args != RT_NULL)
            result = rt_led_get_state(led, (rt_uint8_t *)args);
        else
            result = -RT_ERROR;
        break;

    case LED_CMD_TOGGLE:
        result = rt_led_toggle(led);
        break;

    case LED_CMD_BLINK:
        if(args != RT_NULL){
            blinkCfg = (led_blink_cfg_t *)args;
            result = twinkler.start(led->blink, blinkCfg->count, blinkCfg->period);
        }else
            result = -RT_ERROR;
        break;

    default:
        result = -RT_ENOSYS;
        break;
    }

    return result;
}



#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops ops =
{
    led_init,
    led_open,
    led_close,
    RT_NULL,
    led_write,
    led_control,
	RT_NULL;
};

#endif


/*****************************************3>挂到设备驱动框架 register对应find*****************************************************/

rt_err_t rt_hw_led_init(const char *name, rt_base_t pin, rt_uint8_t activeLevel) {
    ikled_device_ptr devLed = rt_malloc(sizeof(ikled_device_t));
    rt_err_t result;
    rt_memset(devLed, 0, sizeof(ikled_device_t));


#ifdef RT_USING_DEVICE_OPS
    device->ops         = &ops;
#else
    devLed->parent.init    = led_init;
    devLed->parent.open    = led_open;
    devLed->parent.close   = led_close;
    devLed->parent.read    = RT_NULL;
    devLed->parent.write   = led_write;
    devLed->parent.control = led_control;
    devLed->parent.user_data = RT_NULL;
#endif
    /* 设置设备类型和标志 */
    devLed->parent.type = RT_Device_Class_Char;
    devLed->parent.flag = RT_DEVICE_FLAG_RDWR;

    /* LED特有参数 */
    devLed->pin = pin;
    devLed->activeLevel = activeLevel;
    devLed->state = 0;
    devLed->blink = twinkler.creat(500, 500 * 2, led_on, led_off);
    devLed->blink->dev = devLed;
    /* 注册设备 */
    result = rt_device_register(&devLed->parent, name, RT_DEVICE_FLAG_RDWR);

    return result;
}






#define LED_NAME              "led1"
#define LED_PIN                rt_pin_get("PB.12") 
#define LED_ACIVE_LEV          0
static void ledDev_sample(void) {
    rt_uint8_t state;
    led_blink_cfg_t cfg = {3, 200};

    rt_hw_led_init(LED_NAME, LED_PIN,LED_ACIVE_LEV);

    /* 查找LED设备 */
    rt_device_t ledDev = rt_device_find(LED_NAME);

    /* 打开LED设备(会点亮LED) */
    rt_kprintf("LED open \n");
    rt_device_open(ledDev, RT_DEVICE_OFLAG_RDWR);
    rt_thread_mdelay(1000);

    /* 关闭LED */
    rt_kprintf("LED close \n");
    rt_device_close(ledDev);
    rt_thread_mdelay(1000);

    /* 获取LED状态并打印 */
    rt_device_control(ledDev, LED_CMD_GET_STATE, &state);
    rt_kprintf("LED state: %d \n", state);

   /* 翻转LED */
    rt_device_control(ledDev, LED_CMD_TOGGLE, NULL);
    rt_device_control(ledDev, LED_CMD_GET_STATE, &state);
    rt_kprintf("LED state: %d \n", state);
  

    rt_device_control(ledDev, LED_CMD_BLINK,  (void *)&cfg);
    rt_thread_mdelay(5000);
}

MSH_CMD_EXPORT(ledDev_sample, ledDev_sample);



/***************************************** 二次封装*****************************************************/
/***************************************** 二次封装*****************************************************/

int ikled_init(const char *name, rt_base_t pin, rt_uint8_t activeLevel) {
    return rt_hw_led_init(name, pin, activeLevel);
}

void ikled_blink(const char* name, rt_uint16_t count, rt_uint16_t period){
    rt_device_t ikDev = rt_device_find(name);
    led_blink_cfg_t blinkCfg;
    blinkCfg.count = count;
    blinkCfg.period = period;
    rt_device_control(ikDev, LED_CMD_BLINK,  (void *)&blinkCfg);
}

void ikled_on(const char* name){
    rt_device_t ikDev = rt_device_find(name);
    rt_device_open(ikDev, RT_DEVICE_OFLAG_RDWR);
}

void ikled_off(const char* name){
    rt_device_t ikDev = rt_device_find(name);
    rt_device_close(ikDev);
}


