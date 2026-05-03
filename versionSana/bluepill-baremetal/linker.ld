/* 
 * Script de linker para STM32F103C8T6 (Blue Pill)
 *
 * Este archivo le indica al enlazador cómo organizar el código y los datos
 * dentro de la memoria del microcontrolador. Define:
 * - Regiones de memoria disponibles (FLASH y RAM).
 * - Cómo se asignan las secciones del programa (.text, .data, .bss, etc.) a esas regiones.
 * - Símbolos útiles como el Stack Pointer inicial y los límites de .data y .bss.
 *
 * El STM32F103C8T6 cuenta con:
 * - 64 KB de memoria FLASH en 0x08000000
 * - 20 KB de RAM en 0x20000000
 * 
 * La memoria FLASH almacena el código del programa y los datos iniciales.
 * La RAM se usa en tiempo de ejecución para variables, pila y otros datos temporales.
 *
 * Secciones principales:
 * - `.vectors`: vector de interrupciones (primera tabla que lee el micro al encender).
 * - `.text`: código ejecutable (funciones, main, etc.).
 * - `.data`: variables globales con valor inicial (se copian de FLASH a RAM en el startup).
 * - `.bss`: variables no inicializadas (se llenan con ceros en el startup).
 * 
 * Este script también define:
 * - El Stack Pointer inicial (`__reset_stack_pointer`), que apunta al final de la RAM.
 * - Direcciones especiales como _sdata, _edata, _sbss, _ebss y _load_address, que se usan en el código de arranque (`startup.c`).
 *
 * Organización de las secciones en memoria:
 *             
 *               Memoria RAM
 *
 *   0x20005000                                 ← valor inicial del Stack Pointer (SP), fuera del rango de RAM
 *               +---------------------------+   
 *   0x20004FFF  |                           |  ← dirección final de la RAM
 *               |     STACK (pila)          |
 *               |     (crece hacia abajo)   |   
 *               |             ↓             |  ← Stack Pointer (SP): apunta a último valor apilado
 *               +---------------------------+   
 *               |                           |  
 *               |     (espacio libre)       | 
 *               |                           |  
 *               +---------------------------+
 *               | .bss                      |  ← variables no inicializadas
 *               +---------------------------+
 *               | .data (en RAM)            |  ← variables inicializadas (copiadas desde FLASH)
 *   0x20000000  |                           |  ← dirección de inicio de RAM
 *               +---------------------------+    
 *           
 *               ...
 *
 *               Memoria FLASH
 *               +---------------------------+  
 *   0x0800FFFF  |                           |  ← fin de FLASH: dirección final de la memoria FLASH
 *               |     (espacio libre)       |  
 *               |                           |
 *               +---------------------------+
 *               | .data                     |  ← valores originales de las variables inicializadas
 *               +---------------------------+
 *               | .text                     |  ← funciones y código ejecutable
 *               +---------------------------+
 *               | .vectors                  |  ← tabla de vectores de excepción e interrupción  (Stack Pointer inicial, Reset, NMI, etc.)
 *               |                           |  
 *   0x08000000  |                           |  ← dirección de inicio de FLASH
 *               +---------------------------+
 *       
 */

MEMORY
{
    /* Región de memoria FLASH: 64 KB a partir de 0x08000000 */
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K

    /* Región de memoria RAM: 20 KB a partir de 0x20000000 */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* Dirección inicial de la pila: al final de la RAM */
__reset_stack_pointer = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Sección de vectores de interrupciones (va en FLASH al principio) */
    .vectors : {
        *(.isr_vector);  /* Contiene el puntero de pila inicial y el Reset_Handler */
    } > FLASH

    /* Sección de código: todas las funciones y código ejecutable */
    .text : {
        *(.text*)         /* Captura todas las subsecciones que empiecen con .text */
    } > FLASH

    /* Sección BSS: variables no inicializadas */
    .bss : {
        _sbss = .;            /* Inicio de BSS */
        *(.bss*)          /* Variables globales sin valor inicial */
        _ebss = .;            /* Fin de BSS */
    } > RAM
    
    /* Sección de datos inicializados */
    
    /* El símbolo '.' representa el contador de ubicación: la dirección actual 
       en la que el linker está ubicando contenido. Se usa para marcar direcciones. */
    
    .data : {
        _sdata = .;           /* Marca el inicio de la sección .data en RAM */
        *(.data*)         /* Variables globales con valor inicial */
        _edata = .;           /* Marca el fin de la sección .data */    
    } > RAM AT > FLASH     /* Se colocan en RAM pero se cargan desde FLASH */
    
    
    
    /* Dirección de donde se debe copiar .data al iniciar */
    _load_address = LOADADDR(.data);
}
