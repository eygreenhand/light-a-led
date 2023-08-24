#ifndef __myiic_H
#define __myiic_H


void myiic_w_scl(uint8_t value);

void myiic_w_sda(uint8_t value);
uint8_t myiic_r_sda();


void myiic_init();

void myiic_start();
void myiic_stop();
void myiic_sendbyte(uint8_t byte);
uint8_t myiic_recievebyte();
void myiic_sendack(uint8_t byte);
uint8_t myiic_recieveack();





#endif
