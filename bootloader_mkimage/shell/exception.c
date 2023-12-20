

typedef void (*exception_func)(void);

struct vectors {
	unsigned int sp;
	exception_func     Reset_Handler              ; 
	exception_func     NMI_Handler                ; 
	exception_func     HardFault_Handler          ; 
	exception_func     MemManage_Handler          ; 
	exception_func     BusFault_Handler           ; 
	exception_func     UsageFault_Handler         ; 
	exception_func     dummy0                     ; 
	exception_func     dummy1                     ; 
	exception_func     dummy2                     ; 
	exception_func     dummy3                     ; 
	exception_func     SVC_Handler                ; 
	exception_func     DebugMon_Handler           ; 
	exception_func     dummy4                     ; 
	exception_func     PendSV_Handler             ; 
	exception_func     SysTick_Handler            ; 

	// External Interrupts
	exception_func     WWDG_IRQHandler            ; 
	exception_func     PVD_IRQHandler             ; 
	exception_func     TAMPER_IRQHandler          ; 
	exception_func     RTC_IRQHandler             ; 
	exception_func     FLASH_IRQHandler           ; 
	exception_func     RCC_IRQHandler             ; 
	exception_func     EXTI0_IRQHandler           ; 
	exception_func     EXTI1_IRQHandler           ; 
	exception_func     EXTI2_IRQHandler           ; 
	exception_func     EXTI3_IRQHandler           ; 
	exception_func     EXTI4_IRQHandler           ; 
	exception_func     DMA1_Channel1_IRQHandler   ; 
	exception_func     DMA1_Channel2_IRQHandler   ; 
	exception_func     DMA1_Channel3_IRQHandler   ; 
	exception_func     DMA1_Channel4_IRQHandler   ; 
	exception_func     DMA1_Channel5_IRQHandler   ; 
	exception_func     DMA1_Channel6_IRQHandler   ; 
	exception_func     DMA1_Channel7_IRQHandler   ; 
	exception_func     ADC1_2_IRQHandler          ; 
	exception_func     USB_HP_CAN1_TX_IRQHandler  ;  
	exception_func     USB_LP_CAN1_RX0_IRQHandler ;  
	exception_func     CAN1_RX1_IRQHandler        ; 
	exception_func     CAN1_SCE_IRQHandler        ; 
	exception_func     EXTI9_5_IRQHandler         ; 
	exception_func     TIM1_BRK_IRQHandler        ; 
	exception_func     TIM1_UP_IRQHandler         ; 
	exception_func     TIM1_TRG_COM_IRQHandler    ; 
	exception_func     TIM1_CC_IRQHandler         ; 
	exception_func     TIM2_IRQHandler            ; 
	exception_func     TIM3_IRQHandler            ; 
	exception_func     TIM4_IRQHandler            ; 
	exception_func     I2C1_EV_IRQHandler         ; 
	exception_func     I2C1_ER_IRQHandler         ; 
	exception_func     I2C2_EV_IRQHandler         ; 
	exception_func     I2C2_ER_IRQHandler         ; 
	exception_func     SPI1_IRQHandler            ; 
	exception_func     SPI2_IRQHandler            ; 
	exception_func     USART1_IRQHandler          ; 
	exception_func     USART2_IRQHandler          ; 
	exception_func     USART3_IRQHandler          ; 
	exception_func     EXTI15_10_IRQHandler       ; 
	exception_func     RTC_Alarm_IRQHandler       ;
	exception_func     USBWakeUp_IRQHandler       ; 
	exception_func     TIM8_BRK_IRQHandler        ; 
	exception_func     TIM8_UP_IRQHandler         ; 
	exception_func     TIM8_TRG_COM_IRQHandler    ;
	exception_func     TIM8_CC_IRQHandler         ; 
	exception_func     ADC3_IRQHandler            ; 
	exception_func     FSMC_IRQHandler            ; 
	exception_func     SDIO_IRQHandler            ; 
	exception_func     TIM5_IRQHandler            ; 
	exception_func     SPI3_IRQHandler            ; 
	exception_func     UART4_IRQHandler           ; 
	exception_func     UART5_IRQHandler           ; 
	exception_func     TIM6_IRQHandler            ; 
	exception_func     TIM7_IRQHandler            ; 
	exception_func     DMA2_Channel1_IRQHandler   ; 
	exception_func     DMA2_Channel2_IRQHandler   ; 
	exception_func     DMA2_Channel3_IRQHandler   ; 
	exception_func     DMA2_Channel4_5_IRQHandler ; 
};

#define RAM_END (0x20000000+0x10000-4)

void set_new_vector(unsigned int new_pos)
{
	//new_vector = (struct vectors *)new_pos;
	*((unsigned int *)RAM_END) = new_pos;  /* 0x20000000 */
}

//void SVC_Handler(void)
//{
//	/* 跳转去执行APP的SVC_Handler */
//	struct vectors *new_vector = (struct vectors *)(*((unsigned int *)RAM_END));
//	new_vector->SVC_Handler();
//}

void PendSV_Handler(void)
{
	struct vectors *new_vector = (struct vectors *)(*((unsigned int *)RAM_END));
	new_vector->PendSV_Handler();
}

void SysTick_Handler(void)
{
	struct vectors *new_vector = (struct vectors *)(*((unsigned int *)RAM_END));
	new_vector->SysTick_Handler();
}

void USART1_IRQHandler(void)
{
	struct vectors *new_vector = (struct vectors *)(*((unsigned int *)RAM_END));
	new_vector->USART1_IRQHandler();
}

void USART3_IRQHandler(void)
{
	struct vectors *new_vector = (struct vectors *)(*((unsigned int *)RAM_END));
	new_vector->USART1_IRQHandler();
}

void EXTI15_10_IRQHandler(void)
{
	struct vectors *new_vector = (struct vectors *)(*((unsigned int *)RAM_END));
	new_vector->EXTI15_10_IRQHandler();
}

void TIM8_UP_IRQHandler(void)
{
	struct vectors *new_vector = (struct vectors *)(*((unsigned int *)RAM_END));
	new_vector->TIM8_UP_IRQHandler();
}


