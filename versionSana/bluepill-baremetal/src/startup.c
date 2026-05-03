#include <stdint.h>

// Declaración de símbolos definidos en el linker script
extern uint32_t __reset_stack_pointer;  // Valor inicial de stack pointer
extern uint32_t _sdata;                 // Dirección inicial para la sección .data
extern uint32_t _edata;                 // Dirección final para la sección .data
extern uint32_t _load_address;          // Load address para la sección .data en la memoria flash
extern uint32_t _sbss;                  // Dirección inical para la sección .bss
extern uint32_t _ebss;                  // Dirección final  para la sección .bss

int main(void);

// Declaración de los handlers de interrupciones
// Si no se definen explícitamente, todos apuntan a Default_Handler
void Reset_Handler(void);
void SVC_Handler(void)               __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)            __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)           __attribute__((weak, alias("Default_Handler")));
void NMI_Handler(void)               __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)         __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler(void)__attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void)   __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));
void USBWakeUp_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));

// Tabla de vectores: ubicada en la sección .isr_vector
// Contiene las direcciones de los handlers e inicialización del stack
__attribute__((section(".isr_vector")))
void (*const vector_table[])(void) = {
    (void (*)(void))(&__reset_stack_pointer), // Stack pointer inicial
    Reset_Handler,                    // Reset handler: program counter inicial
    NMI_Handler,                      // NMI handler
    HardFault_Handler,                // Hard fault handler
    MemManage_Handler,                // Memory management fault handler
    BusFault_Handler,                 // Bus fault handler
    UsageFault_Handler,               // Usage fault handler
    0,                                // Reserved
    0,                                // Reserved
    0,                                // Reserved
    0,                                // Reserved
    SVC_Handler,                      // SVC handler
    DebugMon_Handler,                 // Debug monitor handler
    0,                                // Reserved
    PendSV_Handler,                   // PendSV handler
    SysTick_Handler,                  // SysTick handler
     // Interrupciones externas
    EXTI0_IRQHandler,                 // External interrupt 0 handler
    EXTI1_IRQHandler,                 // External interrupt 1 handler
    EXTI2_IRQHandler,                 // External interrupt 2 handler
    EXTI3_IRQHandler,                 // External interrupt 3 handler
    EXTI4_IRQHandler,                 // External interrupt 4 handler
    DMA1_Channel1_IRQHandler,         // DMA1 channel 1 handler
    DMA1_Channel2_IRQHandler,         // DMA1 channel 2 handler
    DMA1_Channel3_IRQHandler,         // DMA1 channel 3 handler
    DMA1_Channel4_IRQHandler,         // DMA1 channel 4 handler
    DMA1_Channel5_IRQHandler,         // DMA1 channel 5 handler
    DMA1_Channel6_IRQHandler,         // DMA1 channel 6 handler
    DMA1_Channel7_IRQHandler,         // DMA1 channel 7 handler
    ADC1_2_IRQHandler,                // ADC1 and ADC2 handler
    USB_HP_CAN1_TX_IRQHandler,        // USB high priority or CAN1 TX handler
    USB_LP_CAN1_RX0_IRQHandler,       // USB low priority or CAN1 RX0 handler
    CAN1_RX1_IRQHandler,              // CAN1 RX1 handler
    CAN1_SCE_IRQHandler,              // CAN1 SCE handler
    EXTI9_5_IRQHandler,               // External interrupt 9 to 5 handler
    TIM1_BRK_IRQHandler,              // TIM1 break handler
    TIM1_UP_IRQHandler,               // TIM1 update handler
    TIM1_TRG_COM_IRQHandler,          // TIM1 trigger and commutation handler
    TIM1_CC_IRQHandler,               // TIM1 capture compare handler
    TIM2_IRQHandler,                  // TIM2 handler
    TIM3_IRQHandler,                  // TIM3 handler
    TIM4_IRQHandler,                  // TIM4 handler
    I2C1_EV_IRQHandler,               // I2C1 event handler
    I2C1_ER_IRQHandler,               // I2C1 error handler
    I2C2_EV_IRQHandler,               // I2C2 event handler
    I2C2_ER_IRQHandler,               // I2C2 error handler
    SPI1_IRQHandler,                  // SPI1 handler
    SPI2_IRQHandler,                  // SPI2 handler
    USART1_IRQHandler,                // USART1 handler
    USART2_IRQHandler,                // USART2 handler
    USART3_IRQHandler,                // USART3 handler
    EXTI15_10_IRQHandler,             // External interrupt 15 to 10 handler
    RTC_Alarm_IRQHandler,             // RTC alarm handler
    USBWakeUp_IRQHandler              // USB wakeup handler
};

// Handler por defecto: entra en bucle infinito si no hay otro definido
void Default_Handler(void) {
    while (1);
}

// Reset Handler: punto de entrada tras un reset
void Reset_Handler(void) {
    // Copiar la sección .data de la memoria flash a la RAM
    uint32_t *src = &_load_address;
    for (uint32_t *dest = &_sdata; dest < &_edata;) {
        *dest++ = *src++;
    }

    // Inicializa la sección .bss con ceros (RAM no inicializada)
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }

    // Llama a la función main
    main();

    // Si main retorna, queda en bucle infinito.
    while (1);
}