#include "stm32f10x.h"                  // Device header
#include "myiic.h"
#include "delay.h"

#define mpu6050_address 0xd0
#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75

void mpu6050_writereg(uint8_t reg,uint8_t data)
{
    myiic_start();
    myiic_sendbyte(mpu6050_address);
    myiic_recieveack();
    myiic_sendbyte(reg);
    myiic_recieveack();

    myiic_sendbyte(data);
    myiic_recieveack();
    myiic_stop();

}
uint8_t mpu6050_readreg(uint8_t reg)
{
    uint8_t data;
    myiic_start();
    myiic_sendbyte(mpu6050_address);
    myiic_recieveack();
    myiic_sendbyte(reg);
    myiic_recieveack();

    myiic_start();
    myiic_sendbyte(mpu6050_address|0x01);
    myiic_recieveack();

    data=myiic_recievebyte();
    myiic_sendack(1);
    Delay_us(10);
    myiic_stop();

    return data;

}
void mpu6050_init()
{

    myiic_init();
    //电源配置
    mpu6050_writereg(MPU6050_PWR_MGMT_1,0x01);
    mpu6050_writereg(MPU6050_PWR_MGMT_2,0x00);
    //时钟分频
    mpu6050_writereg(MPU6050_SMPLRT_DIV,0x09);
    //最大滤波
    mpu6050_writereg(MPU6050_CONFIG,0x06);
    mpu6050_writereg(MPU6050_GYRO_CONFIG,0x18);
    mpu6050_writereg(MPU6050_ACCEL_CONFIG,0x18);
}
void mpu6050_getdata(int16_t *accx,int16_t *accy,int16_t *accz,int16_t *gyrox,int16_t *gyroy,int16_t *gyroz)
{
    uint8_t dath,datl;
    
    
    dath=mpu6050_readreg(MPU6050_ACCEL_XOUT_H);
    datl=mpu6050_readreg(MPU6050_ACCEL_XOUT_L);
    *accx=(dath<<8)|datl;        
                                 
    dath=mpu6050_readreg(MPU6050_ACCEL_YOUT_H);
    datl=mpu6050_readreg(MPU6050_ACCEL_YOUT_L);
    *accy=(dath<<8)|datl;        
                                 
    dath=mpu6050_readreg(MPU6050_ACCEL_ZOUT_H);
    datl=mpu6050_readreg(MPU6050_ACCEL_ZOUT_L);
    *accz=(dath<<8)|datl;        
                                 
    dath=mpu6050_readreg(MPU6050_GYRO_XOUT_H);
    datl=mpu6050_readreg(MPU6050_GYRO_XOUT_L);
    *gyrox=(dath<<8)|datl;
    
    dath=mpu6050_readreg(MPU6050_GYRO_YOUT_H);
    datl=mpu6050_readreg(MPU6050_GYRO_YOUT_L);
    *gyroy=(dath<<8)|datl;
    
    
    dath=mpu6050_readreg(MPU6050_GYRO_ZOUT_H);
    datl=mpu6050_readreg(MPU6050_GYRO_ZOUT_L);
    *gyroz=(dath<<8)|datl;
    
}
    