# 💡 Análisis del `main.c` – Blink LED con registros STM32

Este archivo `main.c` es un ejemplo clásico de programa bare-metal: no usa ninguna librería externa (ni HAL, ni CMSIS) y accede directamente a registros del microcontrolador STM32F103C8T6 (Blue Pill).

---

## 🔧 Qué hace este programa

- Habilita el reloj del puerto GPIOC.
- Configura el pin PC13 como salida push-pull.
- Enciende y apaga un LED conectado a ese pin, con retardo por software.

---

## 📥 Definiciones de registros

📌 **Acceso a registros en microcontroladores**  
En sistemas embebidos, los periféricos se controlan mediante registros mapeados en memoria. Para acceder a ellos en C, usamos punteros a direcciones fijas que apuntan a esos registros.


```c
#define RCC_APB2ENR     (*((volatile uint32_t*)0x40021018U))
#define GPIOC_BASE      (0x40011000U)
#define GPIOC_CRH       (*((volatile uint32_t*)(GPIOC_BASE + 0x4U)))
#define GPIOC_ODR       (*((volatile uint32_t*)(GPIOC_BASE + 0xCU)))
```

- `RCC_APB2ENR`: habilita el reloj para periféricos conectados al bus APB2.
- `GPIOC_CRH`: configura los pines PC8 a PC15.
- `GPIOC_ODR`: permite escribir la salida (estado alto o bajo) del pin.

> 📘 Todos estos registros están mapeados en memoria, según el manual de referencia [RM0008](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).


🧠 **¿Qué significa esta expresión?**

`(*((volatile uint32_t*)0x40021018U))` es una forma de acceder a un registro del microcontrolador ubicado en una dirección fija de memoria.

- `uint32_t*` indica que es un puntero a una posición de 32 bits.
- `volatile` le dice al compilador que no optimice el acceso, ya que el valor puede cambiar fuera del control del programa.
- El operador `*` desreferencia el puntero, permitiendo leer o escribir en esa dirección.

Esta técnica es la base del acceso a periféricos en sistemas embebidos sin librerías.

ℹ️ **¿Por qué usamos `volatile`?**
 
La palabra clave `volatile` le indica al compilador que el valor del registro puede cambiar en cualquier momento, fuera del control del programa (por ejemplo, por acción del hardware). Sin `volatile`, el compilador podría optimizar de forma incorrecta las lecturas o escrituras, asumiendo que el valor no cambia. Es imprescindible cuando accedemos directamente a periféricos.

---

## ⚙️ Habilitar GPIOC

```c
RCC_APB2ENR |= (1U << 4);
```

- Bit 4 = IOPCEN → activa el reloj del GPIOC.
- Sin esta línea, no funcionaría el acceso a los registros del puerto C.

---

## ⚙️ Configurar PC13 como salida push-pull

```c
GPIOC_CRH &= ~(0xF << 20);  // Limpia los bits 23:20 (PC13)
GPIOC_CRH |=  (0x2 << 20);  // MODE13 = 10 → salida a 2 MHz
                            // CNF13  = 00 → salida push-pull
```

Cada pin se configura con 4 bits. En este caso:

- Bits 23:22 → CNF13 = 00 (salida push-pull)
- Bits 21:20 → MODE13 = 10 (salida a 2 MHz)

> 📘 Esta configuración está explicada en el manual de referencia [RM0008](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf) para STM32F1.
>
> &#x20;



---

## 🔁 Bucle principal

```c
while (1) {
    GPIOC_ODR |=  (1U << 13); // Apagar LED (salida alta)
    for (int i = 0; i < 500000; i++); // Retardo simple

    GPIOC_ODR &= ~(1U << 13); // Encender LED (salida baja)
    for (int i = 0; i < 500000; i++);
}
```

- En muchas placas Blue Pill, el LED está conectado de modo que se enciende al poner `PC13 = 0`.
- El retardo es por software: no usa timers ni interrupciones, solo un bucle.

---

## 🧠 ¿Por qué es importante este ejemplo?

- Es el punto de partida ideal para entender cómo se controla hardware sin HAL.
- Enseña acceso directo a registros.
- Sirve como base para trabajar luego con otras herramientas y bibliotecas más avanzadas como:
  - **CMSIS**: estándar de ARM para acceder a periféricos de forma estructurada.
  - **HAL**: biblioteca oficial de STMicroelectronics que abstrae el acceso a hardware.
  - **libopencm3**: alternativa open-source minimalista a la HAL.
  - **FreeRTOS**: sistema operativo en tiempo real para aplicaciones multitarea.

> Este tipo de ejemplos ayudan a comprender qué hay "debajo" de las abstracciones que ofrecen las librerías más avanzadas.

---

## 📌 Info adicional

> ¿Querés seguir explorando?
> - [startup.md](startup.md): cómo arranca el programa desde el reset.
> - [linker.md](linker.md): cómo se organiza la memoria con el linker script.
> - [toolchain.md](toolchain.md): cómo compilar, enlazar y flashear el binario.
> - [README.md](../README.md): introducción general al proyecto.

