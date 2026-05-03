# 🧩 Entendiendo `linker.ld` en proyectos STM32

El archivo `linker.ld` (linker script) es el responsable de decirle al enlazador **cómo organizar el código y los datos** en la memoria del microcontrolador STM32F103C8T6 (Blue Pill).

Este script trabaja en conjunto con `startup.c` y es esencial para el correcto funcionamiento de cualquier sistema embebido, **ya sea bare-metal o basado en un sistema operativo**.

---

## 🔹 ¿Qué define este script?

1. **Las regiones de memoria disponibles**:

   - `FLASH`: código del programa y datos iniciales (64 KB desde `0x08000000`)
   - `RAM`: datos en tiempo de ejecución, pila, variables (20 KB desde `0x20000000`)

2. **Dónde se ubica cada sección del programa**:

   - `.vectors`: vector de interrupciones (inicio de la FLASH)
   - `.text`: funciones y código ejecutable
   - `.data`: variables globales con valor inicial (copiadas desde FLASH a RAM en el arranque)
   - `.bss`: variables globales no inicializadas (se llenan con ceros al iniciar)

3. **Símbolos útiles que exporta para el \*\*\*\*****`startup.c`**:

   - `__reset_stack_pointer`, `_sdata`, `_edata`, `_sbss`, `_ebss`, `_load_address`

---

## 📦 Regiones de memoria

```ld
MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}
```

Esto define el mapa de memoria del microcontrolador.

- `rx` → lectura + ejecución (para código en FLASH)
- `rwx` → lectura + escritura + ejecución (para variables en RAM)

También se define:

```ld
__reset_stack_pointer = ORIGIN(RAM) + LENGTH(RAM);
```

> Es el valor inicial del Stack Pointer, apuntando al final de la RAM.

---

## 🗂️ Organización de secciones

### 📌 ¿Qué es el contador de ubicación `.`?

El símbolo `.` representa la **posición actual de memoria** que está siendo asignada. El linker lo va moviendo a medida que va colocando datos o código. También se usa para definir símbolos como `_sdata = .;`, es decir: “desde este punto comienza la sección `.data`”.

---

### 🧭 Vector de interrupciones

```ld
.vectors : {
    *(.isr_vector);
} > FLASH
```

- Contiene el stack pointer inicial y los punteros a los handlers.
- Es lo primero que ejecuta el micro tras un reset.

### ⚙️ Código ejecutable

```ld
.text : {
    *(.text*)
} > FLASH
```

- Todas las funciones, como `main`, van acá.

### 📦 Variables con valor inicial

```ld
_sdata = .;
.data : {
    *(.data*)
} > RAM AT > FLASH
_edata = .;
```

- En tiempo de ejecución, `.data` vive en RAM.
- Pero los valores iniciales están en FLASH, y se copian con `startup.c`

El símbolo especial:

```ld
_load_address = LOADADDR(.data);
```

> Le indica al programa desde qué dirección en FLASH debe copiarse `.data`.

### 🧽 Variables no inicializadas

```ld
_sbss = .;
.bss : {
    *(.bss*)
} > RAM
_ebss = .;
```

- `.bss` ocupa RAM pero no tiene valores en FLASH.
- `startup.c` debe llenarla con ceros al iniciar.

---

## 🗺️ Mapa visual de la memoria

```
Memoria RAM (20 KB)

0x20005000                                 ← valor inicial del Stack Pointer (SP), fuera del rango de RAM
            +---------------------------+   
0x20004FFF  |                           |  ← dirección final de la RAM
            |     STACK (pila)          |
            |     (crece hacia abajo)   |   
            |             ↓             |  ← Stack Pointer (SP): apunta a último valor apilado
            +---------------------------+   
            |                           |  
            |     (espacio libre)       | 
            |                           |  
            +---------------------------+
            | .bss                      |  ← variables no inicializadas
            +---------------------------+
            | .data (en RAM)            |  ← variables inicializadas (copiadas desde FLASH)
0x20000000  |                           |  ← dirección de inicio de RAM
            +---------------------------+    
        
            ...
            
            Memoria FLASH
            +---------------------------+  
0x0800FFFF  |                           |  ← fin de FLASH: dirección final de la memoria FLASH
            |     (espacio libre)       |  
            |                           |
            +---------------------------+
            | .data                     |  ← valores originales de las variables inicializadas
            +---------------------------+
            | .text                     |  ← funciones y código ejecutable
            +---------------------------+
            | .vectors                  |  ← tabla de vectores de excepción e interrupción  (Stack Pointer inicial, Reset, NMI, etc.)
            |                           |  
0x08000000  |                           |  ← dirección de inicio de FLASH
            +---------------------------+
```
---

## 🧠 Conclusiones

- El linker script **define la realidad física de tu programa en memoria**.
- Es imprescindible para todo tipo de proyecto embebido, ya sea bare-metal o con RTOS.
- Junto con `startup.c`, permite que el `main()` arranque en un entorno controlado.
- Con pequeños cambios, podés adaptarlo a otros micros STM32 (cambiando tamaños y direcciones).

---

## 📌 Info adicional

> ¿Querés seguir profundizando?
> - [startup.md](startup.md): cómo se inicializa el sistema y se salta a `main()`.
> - [main.md](main.md): análisis del programa blink y acceso directo a registros.
> - [toolchain.md](toolchain.md): recorrido completo por las herramientas de compilación, enlace y flasheo.
> - [README.md](../README.md): introducción general al proyecto.
