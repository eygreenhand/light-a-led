#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "pwm.h"
void led_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;
    
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    GPIO_Write(GPIOA,0xffff);
}
void led_showflow_clock()
{ 
    int i;
    for(i=0;i<2;i++)
    {
        for(int cnt=0;cnt<8;cnt++)
        {
            GPIO_Write(GPIOA,~(0x0001<<cnt));
        //0000 0000 0000 0001
        //0000 0000 0000 0000
        //0000 0000 0000 0001
        //0000 0000 0000 0000
        //0000 0000 0000 0001
        //0000 0000 0000 0000
            Delay_ms(500);
      
        }
}
     

}


void led_showflow_counterclock()
{ 
    int i;
    for(i=0;i<2;i++)
    {
        for(int cnt=0;cnt<8;cnt++)
        {
            GPIO_Write(GPIOA,~(0x0080>>cnt));
        //0000 0000 0000 0001
        //0000 0000 0000 0010
        //0000 0000 0000 0100
        //0000 0000 0000 1000
        //0000 0000 0001 0000
        //0000 0000 0010 0000
        //0000 0000 0100 0000
        //0000 0000 1000 0000
            Delay_ms(500);
      
        }
}
     

}

void led_up(int i)
{
    switch(i)
    {
        case 0: GPIO_ResetBits(GPIOA,GPIO_Pin_0);break;
        case 1: GPIO_ResetBits(GPIOA,GPIO_Pin_1);break;
        case 2: GPIO_ResetBits(GPIOA,GPIO_Pin_2);break;
        case 3: GPIO_ResetBits(GPIOA,GPIO_Pin_3);break;
        case 4: GPIO_ResetBits(GPIOA,GPIO_Pin_4);break;
        case 5: GPIO_ResetBits(GPIOA,GPIO_Pin_5);break;
        case 6: GPIO_ResetBits(GPIOA,GPIO_Pin_6);break;
        case 7: GPIO_ResetBits(GPIOA,GPIO_Pin_7);break;
    
    
    
    }

}
void led_down(int i)
{

    switch(i)
    {
        case 0: break;
        case 1: GPIO_SetBits(GPIOA,GPIO_Pin_0);break;
        case 2: GPIO_SetBits(GPIOA,GPIO_Pin_1);break;
        case 3: GPIO_SetBits(GPIOA,GPIO_Pin_2);break;
        case 4: GPIO_SetBits(GPIOA,GPIO_Pin_3);break;
        case 5: GPIO_SetBits(GPIOA,GPIO_Pin_4);break;
        case 6: GPIO_SetBits(GPIOA,GPIO_Pin_5);break;
        case 7: GPIO_SetBits(GPIOA,GPIO_Pin_6);break;
    
    
    
    }

}

void led_left(int i)
{
    switch(i)
    {
        case 0: GPIO_Write(GPIOA,~(0x0001<<i));break;
        case 1: GPIO_Write(GPIOA,~(0x0001<<i));break;
        case 2: GPIO_Write(GPIOA,~(0x0001<<i));break;
        case 3: GPIO_Write(GPIOA,~(0x0001<<i));break;
        case 4: GPIO_Write(GPIOA,~(0x0001<<i));break;
        case 5: GPIO_Write(GPIOA,~(0x0001<<i));break;
        case 6: GPIO_Write(GPIOA,~(0x0001<<i));break;
        case 7: GPIO_Write(GPIOA,~(0x0001<<i));break;
    
    
    
    }
    

}
void led_right(int i)
{
    switch(i)
    {
        case 0: GPIO_Write(GPIOA,~(0x0001>>i));break;
        case 1: GPIO_Write(GPIOA,~(0x0001>>i));break;
        case 2: GPIO_Write(GPIOA,~(0x0001>>i));break;
        case 3: GPIO_Write(GPIOA,~(0x0001>>i));break;
        case 4: GPIO_Write(GPIOA,~(0x0001>>i));break;
        case 5: GPIO_Write(GPIOA,~(0x0001>>i));break;
        case 6: GPIO_Write(GPIOA,~(0x0001>>i));break;
        case 7: GPIO_Write(GPIOA,~(0x0001>>i));break;
    
    
    
    }
    

}




void led_forward(int *brightness)
{
    
    if(*brightness>=0&&*brightness<=100)
    {
    int location;
    location=GPIO_ReadInputData(GPIOA);
    pwm(location,*brightness);
    
    }
    else
    {
        *brightness=100;
    
    }



}

void led_backward(int *brightness)
{
    
    
    if(*brightness>=0&&*brightness<=100)
    {
    int location;
    location=GPIO_ReadInputData(GPIOA);
    pwm(location,*brightness);
    
    }
    else
    {
        *brightness=100;
    
    }


}















