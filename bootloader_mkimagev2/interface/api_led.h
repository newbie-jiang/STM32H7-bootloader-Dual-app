#ifndef _API_LED_H_
#define _API_LED_H_

#include "stdint.h"
#include "stdbool.h"


typedef enum LED_STATE{
   eLED_ON = 0,
	 eLED_OFF
}LED_STATE;

typedef enum LED_WHICH{
   eLED_R = 0,
	 eLED_B
}LED_WHICH;
/********************************************************************************/
/* led device */
typedef struct api_led_driver{
  void (*led_init)(void);
  void (*led_control)(LED_WHICH eled_which,LED_STATE eled_state);
}api_led_driver;

extern api_led_driver sapi_led_driver;

void test_led_devive(api_led_driver * papi_led_driver);




#endif /* _API_LED_H_ */
