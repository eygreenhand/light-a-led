#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "myiic.h"
#include "oled.h"
#include "led.h"



#define PAJ7620_ID           0x73<<1 //�豸��ַ
#define PAJ_REGITER_BANK_SEL 0XEF    //BANKѡ��Ĵ���
#define PAJ_BANK0            0X00    //BANK0
#define PAJ_BANK1            0X01    //BANK1

//BANK0 �Ĵ�����
#define PAJ_SUSPEND_CMD            0X03 //�����豸����
#define PAJ_SET_INT_FLAG1          0X41 //�������Ƽ���жϼĴ���1
#define PAJ_SET_INT_FLAG2          0X42 //�������Ƽ���жϼĴ���2
#define PAJ_GET_INT_FLAG1          0X43 //��ȡ���Ƽ���жϱ�־�Ĵ���1(��ȡ���ƽ��)
#define PAJ_GET_INT_FLAG2          0X44 //��ȡ���Ƽ���жϱ�־�Ĵ���2(��ȡ���ƽ��)
#define PAJ_GET_STATE              0X45 //��ȡ���Ƽ�⹤��״̬
#define PAJ_SET_HIGH_THRESHOLD     0x69 //�����ͺ�߷�ֵ�����ڽӽ����ģʽ�£�
#define PAJ_SET_LOW_THRESEHOLD     0X6A //�����ͺ�ͷ�ֵ
#define PAJ_GET_APPROACH_STATE     0X6B //��ȡ�ӽ�״̬ ��1��PS data>= PS threshold ,0:PS data<= Low threshold��
#define PAJ_GET_GESTURE_DATA       0X6C //��ȡ�ӽ�����
#define PAJ_GET_OBJECT_BRIGHTNESS  0XB0 //��ȡ�����������ȣ����255��
#define PAJ_GET_OBJECT_SIZE_1      0XB1 //��ȡ���������С�Ͱ�λ��bit7:0��(���900)
#define PAJ_GET_OBJECT_SIZE_2      0XB2 //��ȡ���������С����λ��bit3:0��

//BANK1 �Ĵ�����
#define PAJ_SET_PS_GAIN         0X44 //���ü�������С (0:1x gain 1:2x gain)
#define PAJ_SET_IDLE_S1_STEP_0  0x67 //����S1����Ӧ����
#define PAJ_SET_IDLE_S1_STEP_1  0x68 
#define PAJ_SET_IDLE_S2_STEP_0  0X69 //����S2����Ӧ����
#define PAJ_SET_IDLE_S2_STEP_1  0X6A 
#define PAJ_SET_OP_TO_S1_STEP_0 0X6B //����OP��S1�Ĺ���ʱ��
#define PAJ_SET_OP_TO_S1_STEP_1 0X6C
#define PAJ_SET_S1_TO_S2_STEP_0 0X6D //����S1��S2�Ĺ���ʱ��
#define PAJ_SET_S1_TO_S2_STEP_1 0X6E
#define PAJ_OPERATION_ENABLE    0X72 //����PAJ7620U2ʹ�ܼĴ���

//����ʶ��Ч��
#define BIT(x) 1<<(x)

#define GES_UP              BIT(0) //����
#define GES_DOWM            BIT(1) //����
#define GES_LEFT            BIT(2) //����
#define GES_RIGHT           BIT(3) //����
#define GES_FORWARD         BIT(4) //��ǰ
#define GES_BACKWARD        BIT(5) //���
#define GES_CLOCKWISE       BIT(6) //˳ʱ��
#define GES_COUNT_CLOCKWISE BIT(7) //��ʱ��
#define GES_WAVE            BIT(8) //�Ӷ�



#define GESTURE_SIZE sizeof(gesture_arry)/2
//����ʶ���ʼ������
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
//�ϵ��ʼ������
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
//	GS_WakeUp();//����PAJ7620U2
//	delay_ms(5);//����ʱ��>400us
//	GS_WakeUp();//����PAJ7620U2
//	delay_ms(5);//����ʱ��>400us
//	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
//	data = GS_Read_Byte(0x00);//��ȡ״̬
//	if(data!=0x20) return 0; //����ʧ��
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
        paj_writereg(init_Array[i][0],init_Array[i][1]);//��ʼ�� PAJ7620U2
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
        paj_writereg(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
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
   

////    paj_writereg(PAJ_SET_INT_FLAG1,0X00);//�ر�����ʶ���ж����
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
