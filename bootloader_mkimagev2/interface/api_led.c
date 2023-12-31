#include "api_led.h"
#include "bsp_led.h"

/**
  * @brief  init led
  * @param  None.
  * @retval None.
  */
void api_led_init(void)
{
  led_init();
}

/**
  * @brief  control led
  * @param  None.
  * @retval None.
  */
void api_led_control(LED_WHICH eled_which,LED_STATE eled_state)
{
   if((eled_which == eLED_R)&& (eled_state == eLED_ON)){
	     LED_R(1);
		}
	 else if((eled_which == eLED_R)&& (eled_state == eLED_OFF)){
			 
	     LED_R(0);
	  }
	 else if((eled_which == eLED_B)&& (eled_state == eLED_ON)){
			
	     LED_B(1);
	  }
	 else if((eled_which == eLED_B)&& (eled_state == eLED_OFF)){
	     LED_B(0);
	  }
	 else{
         /* */
		   }
	
}


api_led_driver sapi_led_driver = {

	.led_init    =  api_led_init,
	.led_control =  api_led_control,
};


void test_led_devive(api_led_driver * papi_led_driver)
{
   papi_led_driver->led_init();
	 papi_led_driver->led_control(eLED_R,eLED_ON);
	 papi_led_driver->led_control(eLED_B,eLED_ON);
}
 
