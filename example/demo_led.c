#include <rtthread.h>
#include <rtdevice.h>
#include "ikLed.h"





static void ikunLed_sample(void) {
    ikled_init(IKUNLED_DEMO_NAME, rt_pin_get(IKUNLED_DEMO_PIN),IKUNLED_DEMO_ACTIVE_LEVEL);
    ikled_on(IKUNLED_DEMO_NAME);
    rt_thread_mdelay(1000);
    ikled_off(IKUNLED_DEMO_NAME);
    rt_thread_mdelay(3000);
    ikled_blink(IKUNLED_DEMO_NAME, 3, 200);
}

MSH_CMD_EXPORT(ikunLed_sample, ikunLed_sample);
