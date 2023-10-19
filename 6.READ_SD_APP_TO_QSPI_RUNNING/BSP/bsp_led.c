#include "bsp_led.h" 


void led_init(void)
{
 

  //hal库已经初始化过了
	

}




void bsp_led_init(void)
{
  led_init();
	for(int i=0;i<10;i++)
	{
  	LEDR_TOGGLE();
	  LEDB_TOGGLE();
		
	  delay_ms(200);
	}
 

}



