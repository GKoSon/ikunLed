#ifndef _TWINKER_H_
#define _TWINKER_H_
  
#include <rtthread.h>
 
 
typedef void   (*callBack)(void *dev);
   
  
 
//节点的结构体
typedef struct  _twinkle{
    uint8_t          start; //节点功能开关
   
    uint32_t         setCnt;//设定的次数 比如鸣叫2次 注:当设置为0表示无次数限制的循环模式
    uint32_t         realCnt;//实际的次数 比如当前叫第1次
   
    uint32_t         aTime;//设定的A计数值
    uint32_t         bTime;//设定的B计数值
    uint32_t         realTime;//实际的计数值
      
    callBack         funA;//[A区间的函数]
    callBack         funB;//[B区间的函数]
    void             *dev;//[AB区间的函数参数]
 
 
    rt_list_t list;
}twinkle_t;
   
   
  
typedef struct{
   twinkle_t*  (*creat) (uint32_t ATime ,uint32_t BTime,callBack Afun,callBack Bfun);
   uint8_t     (*start) (twinkle_t* vip,uint8_t setCnt,uint16_t time);
}twinkle_ops_type;
  
  
  
  
  
   
//对外提供的句柄
extern twinkle_ops_type twinkler;
   
//周期轮训业务 轮训链表的函数
void twinkler_list_loop( void );
   
   
   
   
#endif
