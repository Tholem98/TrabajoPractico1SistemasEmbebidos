# 🔧 La toolchain bare-metal para Blue Pill explicada paso a paso

Esta guía explica en detalle cada componente del entorno de desarrollo (toolchain) usado para compilar, enlazar, flashear y depurar proyectos bare-metal sobre la Blue Pill (STM32F103C8T6).

Está pensada para estudiantes que buscan entender **qué hace cada herramienta**, en qué orden se usan, y cómo se relacionan entre sí.

---

## 🧱 ¿Qué es una toolchain?

Una toolchain es el conjunto de herramientas que transforma tu código fuente en un binario que el microcontrolador puede ejecutar. En este proyecto usamos:

- `arm-none-eabi-gcc`: compilador para microcontroladores ARM sin sistema operativo (bare-metal)
- `make`: automatiza el proceso de compilación
- `OpenOCD`: graba el binario en la placa
- `gdb-multiarch`: permite debuggear el programa

---

## ⚙️ Flujo de trabajo

```mermaid
graph TD
    A[main.c] --> B[Compilador (arm-none-eabi-gcc)]
    B --> C[Archivos .o (objeto)]
    C --> D[Linker (arm-none-eabi-gcc + linker.ld)]
    D --> E[Archivo .elf (con debug info)]
    E --> F[Objcopy → .bin]
    F --> G[OpenOCD → Flash en Blue Pill]
    E --> H[GDB → Debug por ST-Link]
```

---

## 🧩 Componentes explicados

### 1. `main.c`, `startup.c`, etc.

Tu código fuente. Escrito en C, sin HAL ni librerías externas. También puede incluir `startup.c` y definiciones de registros.

---

### 2. `arm-none-eabi-gcc` (compilador y linker)

Herramienta principal. Tiene dos roles:

#### a) Compilar:
Convierte `.c` → `.o` (archivo objeto intermedio):
```bash
arm-none-eabi-gcc -c -mcpu=cortex-m3 -g -o obj/main.o src/main.c
```

#### b) Linkear:
Une todos los `.o` y genera el ejecutable `.elf`, respetando el `linker.ld`:
```bash
arm-none-eabi-gcc -T linker.ld -nostartfiles -Wl,-Map=bin/blink.map -o bin/blink.elf obj/*.o
```

> El `.elf` contiene código, datos y símbolos de depuración.

---

### 3. `linker.ld` (linker script)

Define cómo se organiza el programa en memoria:
- Dónde va la tabla de interrupciones
- Dónde empieza `.text`, `.data`, `.bss`
- Cuánto mide la Flash y la RAM

> Es fundamental para cualquier tipo de proyecto embebido, especialmente aquellos sin sistema operativo. Más info en [linker.md](linker.md)

---

### 4. `arm-none-eabi-objcopy` → `.bin`

Extrae el binario “crudo” desde el `.elf`, listo para flashear:
```bash
arm-none-eabi-objcopy -O binary bin/blink.elf bin/blink.bin
```

> El `.bin` es más liviano: no tiene info de debug.

---

### 5. `OpenOCD`

Programa el binario en la memoria Flash del microcontrolador. Se conecta al ST-Link:
```bash
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program bin/blink.elf verify reset exit"
```

También puede quedar “esperando” para debug con GDB:
```bash
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
```

---

### 6. `gdb-multiarch`

Permite inspeccionar el programa en ejecución:
```bash
gdb-multiarch bin/blink.elf
```
En GDB:
```gdb
(gdb) target remote localhost:3333
(gdb) break main
(gdb) continue
```

Podés ver registros, memoria, variables, o ejecutar paso a paso.

---

### 7. `Makefile`

Automatiza todo el proceso:
- Compila todos los archivos fuente
- Linkea con las flags adecuadas
- Genera `.elf`, `.bin`, `.map`
- Permite flashear o debuggear con un comando

Ejemplos:
```bash
make           # Compila todo
make flash     # Flashea con OpenOCD
make gdb       # Abre GDB
make clean     # Limpia archivos generados
```

---

## 🧠 ¿Por qué entender la toolchain?

Porque te permite:
- Diagnosticar errores de compilación o linker
- Modificar el flujo según lo que necesitás (ej: agregar un RTOS)
- Valorar el control total sobre el entorno
- Dominar lo que muchos simplemente usan "como magia"

---

> ¿Querés seguir aprendiendo? Explorá los documentos relacionados:
> - [startup.md](startup.md): qué hace el código de arranque y cómo se inicializa el sistema.
> - [linker.md](linker.md): cómo se organiza la memoria y qué hace el linker script.
> - [main.md](main.md): análisis del programa de ejemplo (blink) paso a paso.
> - [README.md](../README.md): introducción general al proyecto.

