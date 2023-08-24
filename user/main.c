#include "stm32f10x.h"   // Device header
#include "oled.h"   // Device header
#include "myiic.h"
#include "paj.h"
#include "led.h"

int main()
{
    OLED_Init();
    OLED_ShowString(1,1,"hand position");
    paj_init();
    paj_position_init();
    led_init();
  while(1)
  {
      
        read_position();
      
        


  }
    
    
}
