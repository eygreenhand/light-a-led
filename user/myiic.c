#include "stm32f10x.h"                  // Device header
#include "delay.h"
void myiic_w_scl(uint8_t value)
{
    
    GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)value);
    Delay_us(10);

}
void myiic_w_sda(uint8_t value)
{
    
    GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)value);
    Delay_us(10);

}

uint8_t myiic_r_sda()
{
    uint8_t value;
    value=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
    Delay_us(10);
    return value;

}


void myiic_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    
    
    GPIO_InitTypeDef gpio_initstructure;
    gpio_initstructure.GPIO_Mode=GPIO_Mode_Out_OD;
    gpio_initstructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
    gpio_initstructure.GPIO_Speed=GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB,&gpio_initstructure);
    
    
    GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
}

void myiic_start()
{
    myiic_w_sda(1);
    myiic_w_scl(1);
    myiic_w_sda(0);
    myiic_w_scl(0);
}
void myiic_stop()
{
    myiic_w_sda(0);
    myiic_w_scl(1);
    myiic_w_sda(1);
    
}
void myiic_sendbyte(uint8_t byte)
{
    int i;
    for(i=0;i<8;i++)
    {
        myiic_w_sda(byte&(0x80>>i));
        myiic_w_scl(1);
        myiic_w_scl(0);
    }
    
}
uint8_t myiic_recievebyte()
{   
    int i;
    uint8_t byte=0x00;
    myiic_w_sda(1);
    
    for(i=0;i<8;i++)
    {
        myiic_w_scl(1);
        if(myiic_r_sda()==1)
    {
        byte|=(0x80>>i);
    
    }
        myiic_w_scl(0);
    
    }
    return byte;

}
void myiic_sendack(uint8_t byte)
{
    myiic_w_sda(byte);
    myiic_w_scl(1);
    myiic_w_scl(0);
 
    
}
uint8_t myiic_recieveack()
{   
    uint8_t byte;
    //这里不能理解为置sda为高电平，而是释放sda
    myiic_w_sda(1);

    myiic_w_scl(1);
    byte=myiic_r_sda();
    myiic_w_scl(0);

    return byte;

}
