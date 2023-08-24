#include "stm32f10x.h"                  // Device header
#include "delay.h"
void pwm(int location,int brightness)
{
    
    for(int i=0;i<100000;i++)
    {
    GPIO_Write(GPIOA,0xffff);
    Delay_us(100-brightness);
    GPIO_Write(GPIOA,location);
    Delay_us(brightness);
    
    
    }
    




}