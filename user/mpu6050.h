#ifndef __mpu6050_H
#define __mpu6050_H


void mpu6050_writereg(uint8_t reg,uint8_t data);
uint8_t mpu6050_readreg(uint8_t reg);
void mpu6050_init();
void mpu6050_getdata(int16_t *accx,int16_t *accy,int16_t *accz,int16_t *gyrox,int16_t *gyroy,int16_t *gyroz);


#endif