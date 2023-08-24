#ifndef __led_H
#define __led_H

void led_init();
void led_showflow_clock();


void led_showflow_counterclock();
void led_up(int i);
void led_down(int i);

void led_left(int i);
void led_right(int i);


void led_forward(int *brightness);

void led_backward(int *brightness);
#endif
