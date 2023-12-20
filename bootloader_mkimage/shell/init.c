#include "string.h"

void SystemInit(void)
{
	extern int * Image$$ER_IROM1$$Base;
	extern int * Image$$ER_IROM1$$Length;
	extern int * Load$$ER_IROM1$$Base;
	extern int * Image$$RW_IRAM1$$Base;
	extern int * Image$$RW_IRAM1$$Length;
	extern int * Load$$RW_IRAM1$$Base;
	extern int * Image$$RW_IRAM1$$ZI$$Base;
	extern int * Image$$RW_IRAM1$$ZI$$Length;	
	
	
	/* data relocate */
	memcpy((void *)&Image$$RW_IRAM1$$Base, (void *)&Load$$RW_IRAM1$$Base, (rt_ubase_t)&Image$$RW_IRAM1$$Length);
	
	/* bss clear */
	memset((void *)&Image$$RW_IRAM1$$ZI$$Base, 0, (rt_ubase_t)&Image$$RW_IRAM1$$ZI$$Length);
}
