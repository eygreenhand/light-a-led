#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "myiic.h"
#include "oled.h"
#include "led.h"



#define PAJ7620_ID           0x73<<1 //设备地址
#define PAJ_REGITER_BANK_SEL 0XEF    //BANK选择寄存器
#define PAJ_BANK0            0X00    //BANK0
#define PAJ_BANK1            0X01    //BANK1

//BANK0 寄存器组
#define PAJ_SUSPEND_CMD            0X03 //设置设备挂起
#define PAJ_SET_INT_FLAG1          0X41 //设置手势检测中断寄存器1
#define PAJ_SET_INT_FLAG2          0X42 //设置手势检测中断寄存器2
#define PAJ_GET_INT_FLAG1          0X43 //获取手势检测中断标志寄存器1(获取手势结果)
#define PAJ_GET_INT_FLAG2          0X44 //获取手势检测中断标志寄存器2(获取手势结果)
#define PAJ_GET_STATE              0X45 //获取手势检测工作状态
#define PAJ_SET_HIGH_THRESHOLD     0x69 //设置滞后高阀值（仅在接近检测模式下）
#define PAJ_SET_LOW_THRESEHOLD     0X6A //设置滞后低阀值
#define PAJ_GET_APPROACH_STATE     0X6B //获取接近状态 （1：PS data>= PS threshold ,0:PS data<= Low threshold）
#define PAJ_GET_GESTURE_DATA       0X6C //获取接近数据
#define PAJ_GET_OBJECT_BRIGHTNESS  0XB0 //获取被照物体亮度（最大255）
#define PAJ_GET_OBJECT_SIZE_1      0XB1 //获取被照物体大小低八位（bit7:0）(最大900)
#define PAJ_GET_OBJECT_SIZE_2      0XB2 //获取被照物体大小高四位（bit3:0）

//BANK1 寄存器组
#define PAJ_SET_PS_GAIN         0X44 //设置检测增益大小 (0:1x gain 1:2x gain)
#define PAJ_SET_IDLE_S1_STEP_0  0x67 //设置S1的响应因子
#define PAJ_SET_IDLE_S1_STEP_1  0x68 
#define PAJ_SET_IDLE_S2_STEP_0  0X69 //设置S2的响应因子
#define PAJ_SET_IDLE_S2_STEP_1  0X6A 
#define PAJ_SET_OP_TO_S1_STEP_0 0X6B //设置OP到S1的过度时间
#define PAJ_SET_OP_TO_S1_STEP_1 0X6C
#define PAJ_SET_S1_TO_S2_STEP_0 0X6D //设置S1到S2的过度时间
#define PAJ_SET_S1_TO_S2_STEP_1 0X6E
#define PAJ_OPERATION_ENABLE    0X72 //设置PAJ7620U2使能寄存器

//手势识别效果
#define BIT(x) 1<<(x)

#define GES_UP              BIT(0) //向上
#define GES_DOWM            BIT(1) //向下
#define GES_LEFT            BIT(2) //向左
#define GES_RIGHT           BIT(3) //向右
#define GES_FORWARD         BIT(4) //向前
#define GES_BACKWARD        BIT(5) //向后
#define GES_CLOCKWISE       BIT(6) //顺时针
#define GES_COUNT_CLOCKWISE BIT(7) //逆时针
#define GES_WAVE            BIT(8) //挥动



#define GESTURE_SIZE sizeof(gesture_arry)/2
//手势识别初始化数组
const unsigned char gesture_arry[][2]={
	
	{0xEF,0x00},
	{0x41,0x00},
	{0x42,0x00},
	{0xEF,0x00},
	{0x48,0x3C},
	{0x49,0x00},
	{0x51,0x10},
	{0x83,0x20},
	{0x9F,0xF9},
	{0xEF,0x01},
	{0x01,0x1E},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x41,0x40},
	{0x43,0x30},
	{0x65,0x96},
	{0x66,0x00},
	{0x67,0x97},
	{0x68,0x01},
	{0x69,0xCD},
	{0x6A,0x01},
	{0x6B,0xB0},
	{0x6C,0x04},
	{0x6D,0x2C},
	{0x6E,0x01},
	{0x74,0x00},
	{0xEF,0x00},
	{0x41,0xFF},
	{0x42,0x01},

};

#define INIT_SIZE sizeof(init_Array)/2
//上电初始化数组
const unsigned char init_Array[][2] = {

    {0xEF,0x00},
	{0x37,0x07},
    {0x38,0x17},
	{0x39,0x06},
	{0x41,0x00},
	{0x42,0x00},
	{0x46,0x2D},
	{0x47,0x0F},
	{0x48,0x3C},
	{0x49,0x00},
	{0x4A,0x1E},
	{0x4C,0x20},
	{0x51,0x10},
	{0x5E,0x10},
	{0x60,0x27},
	{0x80,0x42},
	{0x81,0x44},
	{0x82,0x04},
	{0x8B,0x01},
	{0x90,0x06},
	{0x95,0x0A},
	{0x96,0x0C},
	{0x97,0x05},
	{0x9A,0x14},
	{0x9C,0x3F},
	{0xA5,0x19},
	{0xCC,0x19},
	{0xCD,0x0B},
	{0xCE,0x13},
	{0xCF,0x64},
	{0xD0,0x21},
	{0xEF,0x01},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x25,0x01},
	{0x27,0x39},
	{0x28,0x7F},
	{0x29,0x08},
	{0x3E,0xFF},
	{0x5E,0x3D},
	{0x65,0x96},
	{0x67,0x97},
	{0x69,0xCD},
	{0x6A,0x01},
	{0x6D,0x2C},
	{0x6E,0x01},
	{0x72,0x01},
	{0x73,0x35},
	{0x74,0x00},
	{0x77,0x01},
};


void paj_writereg(uint8_t reg,uint8_t data)
{
    myiic_start();
    myiic_sendbyte(PAJ7620_ID);
    myiic_recieveack();
    myiic_sendbyte(reg);
    myiic_recieveack();

    myiic_sendbyte(data);
    myiic_recieveack();
    myiic_stop();
    
}
uint8_t paj_readreg(uint8_t reg)
{
    uint8_t data;
    myiic_start();
    myiic_sendbyte(PAJ7620_ID);
    myiic_recieveack();
    myiic_sendbyte(reg);
    myiic_recieveack();

    myiic_start();
    myiic_sendbyte(PAJ7620_ID|0x01);
    myiic_recieveack();

    data=myiic_recievebyte();
    myiic_sendack(1);
    Delay_us(10);
    myiic_stop();

    return data;

}
void paj_wake()
{
        
        myiic_start();
        myiic_sendbyte(PAJ7620_ID);
        myiic_stop();
       

//    u8 data=0x0a;
//	GS_WakeUp();//唤醒PAJ7620U2
//	delay_ms(5);//唤醒时间>400us
//	GS_WakeUp();//唤醒PAJ7620U2
//	delay_ms(5);//唤醒时间>400us
//	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
//	data = GS_Read_Byte(0x00);//读取状态
//	if(data!=0x20) return 0; //唤醒失败
//	
//	return 1;

}
uint8_t paj_init()
{
    int i;
    uint8_t dt;
    myiic_init();
    paj_wake();
    Delay_ms(5);
    paj_wake();
    Delay_ms(5);
    paj_writereg(PAJ_OPERATION_ENABLE,0x01);
    dt=paj_readreg(0x00);
    if(dt!=0x20)return 0;
        paj_writereg(PAJ_REGITER_BANK_SEL,PAJ_BANK0);

    for(i=0;i<INIT_SIZE;i++)
    {
        paj_writereg(init_Array[i][0],init_Array[i][1]);//初始化 PAJ7620U2
    }
    paj_writereg(PAJ_REGITER_BANK_SEL,PAJ_BANK0);

    return 1;
//    paj_writereg(PAJ_REGITER_BANK_SEL,PAJ_BANK1);
    
   

}
//1000 0000
//0100 0000
//0010 0000
//0001 0000
//0000 1000
//0000 0100
//0000 0010
//0000 0001
void paj_position_init()
{
    int i;
 paj_writereg(PAJ_REGITER_BANK_SEL,PAJ_BANK0);
    for(i=0;i<GESTURE_SIZE;i++)
    {
        paj_writereg(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
    }
    paj_writereg(PAJ_REGITER_BANK_SEL,PAJ_BANK0);

}
static int i=-1;
static int cnt=-1;
static int pwm=100;
void read_position()
{
    
    
    
    
    //OLED_ShowString(4,1,"hello");
    uint8_t read;
   

////    paj_writereg(PAJ_SET_INT_FLAG1,0X00);//关闭手势识别中断输出
////    paj_writereg(PAJ_SET_INT_FLAG2,0X00);

    paj_writereg(PAJ_REGITER_BANK_SEL,PAJ_BANK0);
    read=paj_readreg(0x43);
    
    
    switch(read)
    {
        case 0x80: OLED_ShowString(2,1,"counter clock");     led_showflow_clock();         break;
        case 0x40: OLED_ShowString(2,1,"clock        ");     led_showflow_counterclock();  break;
        case 0x20: OLED_ShowString(2,1,"backward     ");  //  pwm=pwm-30; led_backward(&pwm);                              break;
        case 0x10: OLED_ShowString(2,1,"forward      ");  //  pwm=pwm+30; led_forward(&pwm);                               break;
        case 0x08: OLED_ShowString(2,1,"right        ");     cnt--;led_left(cnt);                              break;
        case 0x04: OLED_ShowString(2,1,"left         ");     cnt++;led_left(cnt);                            break;
        case 0x02: OLED_ShowString(2,1,"down         ");     led_down(i); i--;                            break;
        case 0x01: OLED_ShowString(2,1,"up           ");     led_up(i); i++;                            break;
        case 0x00: Delay_ms(500);  OLED_ShowString(2,1,"none         ");
    }




}
