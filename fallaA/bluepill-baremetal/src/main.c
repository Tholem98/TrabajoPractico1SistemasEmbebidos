#include <stdint.h>

// Definición de registros y direcciones de periféricos
#define RCC_APB2ENR     (*((volatile uint32_t*)0x40021018U))        // Registro de habilitación de clocks (p. 112)
#define GPIOC_BASE      (0x40011000U)                               // Dirección base del GPIOC
#define GPIOC_CRH       (*((volatile uint32_t*)(GPIOC_BASE + 0x4U)))// Configuración de pines 8..15 (p. 172)
#define GPIOC_ODR       (*((volatile uint32_t*)(GPIOC_BASE + 0xCU)))// Registro de salida (p. 173)

int main()
{
  // Habilitar el clock del periférico GPIOC
  // RCC_APB2ENR, bit 4 → IOPCEN = 1 → habilita GPIOC
  RCC_APB2ENR |= (1U << 4);

  // Configurar el pin PC13 como salida push-pull, velocidad máxima 50 MHz
  // En GPIOC_CRH (para pines 8..15), cada pin usa 4 bits.
  // PC13 → bits 23:20 → MODE13[1:0] = 10 (50 MHz), CNF13[1:0] = 00 (salida push-pull)

  GPIOC_CRH &= ~(0xF << 20);       // Limpia los 4 bits de configuración de PC13
  GPIOC_CRH |=  (0x2 << 20);       // MODE13 = 10 (output, 50 MHz)
                                   // CNF13  = 00 (push-pull)

  while (1)
  {
    // Apagar LED (PC13 = 1, LED conectado a masa en muchas Blue Pill)
    GPIOC_ODR |= (1U << 13);

    // Delay por software
    for (int i = 0; i < 500000; i++);

    // Encender LED (PC13 = 0)
    GPIOC_ODR &= ~(1U << 13);

    // Delay por software
    for (int i = 0; i < 500000; i++);
  }

  return 0;
}
