
                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
				EXPORT  __Vectors
				;IMPORT SVC_Handler
				IMPORT PendSV_Handler
				IMPORT SysTick_Handler
				IMPORT USART1_IRQHandler
				IMPORT USART3_IRQHandler
				IMPORT EXTI15_10_IRQHandler
				IMPORT TIM8_UP_IRQHandler
					
__Vectors       DCD     (0x20000000+0x10000)       ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     0 ;NMI_Handler                ; NMI Handler
                DCD     0 ;HardFault_Handler          ; Hard Fault Handler
                DCD     0 ;MemManage_Handler          ; MPU Fault Handler
                DCD     0 ;BusFault_Handler           ; Bus Fault Handler
                DCD     0 ;UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     0 ;DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     0 ;WWDG_IRQHandler            ; Window Watchdog
                DCD     0 ;PVD_IRQHandler             ; PVD through EXTI Line detect
                DCD     0 ;TAMPER_IRQHandler          ; Tamper
                DCD     0 ;RTC_IRQHandler             ; RTC
                DCD     0 ;FLASH_IRQHandler           ; Flash
                DCD     0 ;RCC_IRQHandler             ; RCC
                DCD     0 ;EXTI0_IRQHandler           ; EXTI Line 0
                DCD     0 ;EXTI1_IRQHandler           ; EXTI Line 1
                DCD     0 ;EXTI2_IRQHandler           ; EXTI Line 2
                DCD     0 ;EXTI3_IRQHandler           ; EXTI Line 3
                DCD     0 ;EXTI4_IRQHandler           ; EXTI Line 4
                DCD     0 ;DMA1_Channel1_IRQHandler   ; DMA1 Channel 1
                DCD     0 ;DMA1_Channel2_IRQHandler   ; DMA1 Channel 2
                DCD     0 ;DMA1_Channel3_IRQHandler   ; DMA1 Channel 3
                DCD     0 ;DMA1_Channel4_IRQHandler   ; DMA1 Channel 4
                DCD     0 ;DMA1_Channel5_IRQHandler   ; DMA1 Channel 5
                DCD     0 ;DMA1_Channel6_IRQHandler   ; DMA1 Channel 6
                DCD     0 ;DMA1_Channel7_IRQHandler   ; DMA1 Channel 7
                DCD     0 ;ADC1_2_IRQHandler          ; ADC1 & ADC2
                DCD     0 ;USB_HP_CAN1_TX_IRQHandler  ; USB High Priority or CAN1 TX
                DCD     0 ;USB_LP_CAN1_RX0_IRQHandler ; USB Low  Priority or CAN1 RX0
                DCD     0 ;CAN1_RX1_IRQHandler        ; CAN1 RX1
                DCD     0 ;CAN1_SCE_IRQHandler        ; CAN1 SCE
                DCD     0 ;EXTI9_5_IRQHandler         ; EXTI Line 9..5
                DCD     0 ;TIM1_BRK_IRQHandler        ; TIM1 Break
                DCD     0 ;TIM1_UP_IRQHandler         ; TIM1 Update
                DCD     0 ;TIM1_TRG_COM_IRQHandler    ; TIM1 Trigger and Commutation
                DCD     0 ;TIM1_CC_IRQHandler         ; TIM1 Capture Compare
                DCD     0 ;TIM2_IRQHandler            ; TIM2
                DCD     0 ;TIM3_IRQHandler            ; TIM3
                DCD     0 ;TIM4_IRQHandler            ; TIM4
                DCD     0 ;I2C1_EV_IRQHandler         ; I2C1 Event
                DCD     0 ;I2C1_ER_IRQHandler         ; I2C1 Error
                DCD     0 ;I2C2_EV_IRQHandler         ; I2C2 Event
                DCD     0 ;I2C2_ER_IRQHandler         ; I2C2 Error
                DCD     0 ;SPI1_IRQHandler            ; SPI1
                DCD     0 ;SPI2_IRQHandler            ; SPI2
                DCD     USART1_IRQHandler          ; USART1
                DCD     0 ;USART2_IRQHandler          ; USART2
                DCD     USART3_IRQHandler          ; USART3
                DCD     EXTI15_10_IRQHandler       ; EXTI Line 15..10
                DCD     0 ;RTC_Alarm_IRQHandler        ; RTC Alarm through EXTI Line
                DCD     0 ;USBWakeUp_IRQHandler       ; USB Wakeup from suspend
                DCD     0 ;TIM8_BRK_IRQHandler        ; TIM8 Break
                DCD     TIM8_UP_IRQHandler         ; TIM8 Update
                DCD     0 ;TIM8_TRG_COM_IRQHandler    ; TIM8 Trigger and Commutation
                DCD     0 ;TIM8_CC_IRQHandler         ; TIM8 Capture Compare
                DCD     0 ;ADC3_IRQHandler            ; ADC3
                DCD     0 ;FSMC_IRQHandler            ; FSMC
                DCD     0 ;SDIO_IRQHandler            ; SDIO
                DCD     0 ;TIM5_IRQHandler            ; TIM5
                DCD     0 ;SPI3_IRQHandler            ; SPI3
                DCD     0 ;UART4_IRQHandler           ; UART4
                DCD     0 ;UART5_IRQHandler           ; UART5
                DCD     0 ;TIM6_IRQHandler            ; TIM6
                DCD     0 ;TIM7_IRQHandler            ; TIM7
                DCD     0 ;DMA2_Channel1_IRQHandler   ; DMA2 Channel1
                DCD     0 ;DMA2_Channel2_IRQHandler   ; DMA2 Channel2
                DCD     0 ;DMA2_Channel3_IRQHandler   ; DMA2 Channel3
                DCD     0 ;DMA2_Channel4_5_IRQHandler ; DMA2 Channel4 & Channel5
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors


				AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler   PROC
				EXPORT  Reset_Handler             [WEAK]
                IMPORT  mymain
				IMPORT SystemInit

				BL SystemInit
				BL mymain

                ENDP

start_app   PROC
				EXPORT  start_app

				; set vector base address as 0x08040000
				;ldr r3, =0xE000ED08
				;str r0, [r3]
				
				ldr sp, [r0]      ; read val from new vector
				ldr r1, [r0, #4]  ; read val from "new vector + 4“

				BX r1

                ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler
                
				ldr r0, =(0x20000000 + 0x10000 - 4)
				ldr r0, [r0]  ; r0 = new_vector
				ldr r0, [r0, #0x2c]  ; new SVC_Handler
				bx r0
				
                ENDP


                 END

