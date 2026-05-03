# 🧠 Entendiendo `startup.c` en proyectos bare-metal STM32

Este archivo es esencial en cualquier proyecto bare-metal. Se encarga de preparar el entorno mínimo para que tu programa (el `main()`) pueda ejecutarse correctamente. No hay sistema operativo ni entorno de runtime: lo hacemos todo nosotros.

---

## 🔹 ¿Qué contiene el archivo `startup.c`?

### 1. **La tabla de vectores (`.isr_vector`)**
Es un array de punteros a funciones que indica a la CPU:
- Dónde está el stack pointer inicial (SP)
- Qué función ejecutar ante un reset (Reset_Handler)
- Qué función ejecutar ante interrupciones y excepciones

```c
__attribute__((section(".isr_vector")))
void (*const vector_table[])(void) = {
    (void (*)(void))(&__reset_stack_pointer), // Valor inicial del SP
    Reset_Handler,                            // Punto de entrada tras reset
    NMI_Handler,
    HardFault_Handler,
    ...
};
```

> 📘 Esta tabla se ubica en la dirección 0x08000000 (inicio de la Flash del STM32), definida en el linker script.

---

### 2. **El `Reset_Handler`**
Esta función se ejecuta automáticamente luego del reset del microcontrolador. Su trabajo es:

- Copiar la sección `.data` desde Flash a RAM
- Inicializar en cero la sección `.bss` (RAM sin inicializar)
- Llamar a la función `main()`

```c
void Reset_Handler(void) {
    // Copia .data desde Flash a RAM
    uint32_t *src = &_load_address;
    for (uint32_t *dest = &_sdata; dest < &_edata;) {
        *dest++ = *src++;
    }

    // Inicializa .bss con ceros
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }

    // Llama al main
    main();

    while (1); // Seguridad por si main retorna
}
```

---

### 3. **Los demás handlers**

Son funciones que responden a interrupciones. Si no están implementadas, caen por defecto en `Default_Handler`:

```c
void Default_Handler(void) {
    while (1); // Bucle infinito si se dispara una interrupción no manejada
}
```

Podés redefinir cualquier handler en tu código, por ejemplo:
```c
void EXTI0_IRQHandler(void) {
    // Código que se ejecuta cuando se activa la interrupción externa 0
}
```

---

## 🧠 ¿Por qué es importante entender esto?

Porque en bare-metal:
- Vos decidís qué pasa desde que el micro se resetea.
- No hay sistema operativo que “inicialice todo por vos”.
- Entender `startup.c` te da control total y facilita la integración con librerías como CMSIS, FreeRTOS o tu propio bootloader.

---

## 📌 Info adicional

El archivo `startup.c` trabaja en conjunto con el `linker.ld`. Este último ubica las secciones `.isr_vector`, `.data`, `.bss`, etc. en memoria. Entender ambos es clave para dominar proyectos embebidos.

> ¿Querés seguir profundizando?
> - [linker.md](linker.md): cómo se organiza la memoria y qué hace el linker script.
> - [main.md](main.md): explicación detallada del programa de ejemplo.
> - [toolchain.md](toolchain.md): cómo funciona cada componente del entorno de compilación.
> - [README.md](../README.md): introducción general del proyecto.


