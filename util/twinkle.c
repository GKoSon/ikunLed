/*
 * twinkle.c
 * Change Logs:
 * Date           Author            Notes
 * 2025-10-14     koson            first version
 */
#include "twinkle.h"
#include <rthw.h>
static rt_list_t twinkHeadList = RT_LIST_OBJECT_INIT(twinkHeadList);
 
 
static void twinkle_node_add(twinkle_t* new) {
    rt_list_insert_after(&twinkHeadList, &(new->list));
}
 
 
 
void func_exe(twinkle_t* target) {
    if(target->start) {
        target->realTime ++ ;
 
        if((target->realTime) == (target->aTime)) {
            if(target->funA) target->funA(target->dev) ;
        } else  if((target->realTime) == (target->bTime)) {
            if(target->funB) target->funB(target->dev) ;
        } else if((target->realTime) > (target->bTime)) {
            target->realTime = 0;   

            if(target->setCnt == 0) return;
            if((++target->realCnt) >= (target->setCnt)) {
                target->start = 0;
                target->realCnt = 0;
            }
        }
    }
}
 
 
void twinkler_list_loop(void) {
    rt_base_t level;
 
    twinkle_t *target = RT_NULL;
    struct rt_list_node *node = RT_NULL;
 
    level = rt_hw_interrupt_disable();
 
 
    if(!rt_list_isempty(&twinkHeadList)) {
        rt_list_for_each(node, &twinkHeadList) {
            target = rt_list_entry(node, twinkle_t, list);
            func_exe(target);
        }
    }
 
    rt_hw_interrupt_enable(level);
}
 
twinkle_t *search_slist(twinkle_t* vip) {
    twinkle_t *target = RT_NULL;
    struct rt_list_node *node = RT_NULL;
 
    if(!rt_list_isempty(&twinkHeadList)) {
        rt_list_for_each(node, &twinkHeadList) {
            target = rt_container_of(node, twinkle_t, list);
            if(target == vip) {
                return target;
            }
        }
    }
 
    return NULL;
}
uint8_t twinkler_set(twinkle_t* vip, uint8_t setCnt, uint16_t time) {
    twinkle_t *target = search_slist(vip);
    if(NULL == target) return -1;
 
    target->aTime = time;
    target->bTime = time * 2;
    target->setCnt = setCnt;
    target->start = 1;
    return 0;
}
 
 
 
 
 
twinkle_t *twinkler_register(uint32_t ATime, uint32_t BTime, callBack Afun, callBack Bfun) {
    twinkle_t *this = (twinkle_t *)rt_malloc(sizeof(twinkle_t));
    if(this == NULL)return NULL;
    this->start = 0;
    this->setCnt = 0;
    this->realCnt = 0;
 
    this->aTime = ATime;
    this->bTime = BTime;
    this->realTime = 0;
    this->funA = Afun;
    this->funB = Bfun;
    twinkle_node_add(this);
    return this;
}
twinkle_ops_type twinkler = {
    .creat = twinkler_register,
    .start   = twinkler_set,
};


/**********************内部定时驱动twinkler_list_loop********************************/

#define TIMER_DELAY_MS 1  


static void timerxMs_callback(void *parameter) {
    twinkler_list_loop();
}


static int twinkle_app_init(void) {
    rt_timer_t timer; 

    timer = rt_timer_create("timerxMs",          
                            timerxMs_callback,  
                            RT_NULL,             
                            TIMER_DELAY_MS,      
                            RT_TIMER_FLAG_PERIODIC); 

    if(timer == RT_NULL) {
        rt_kprintf("twinkle_app_init rt_timer_create Fail\n");
        return -RT_ERROR;
    }


    if(rt_timer_start(timer) != RT_EOK) {
        rt_kprintf("twinkle_app_init rt_timer_start Fail\n");
        rt_timer_delete(timer);  
        return -RT_ERROR;
    }

    rt_kprintf("twinkle_app_init ok\n");
    return RT_EOK;
}


INIT_APP_EXPORT(twinkle_app_init);
