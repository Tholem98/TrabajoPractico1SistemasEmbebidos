# Trabajo Práctico — Arranque de Sistema en STM32 (Bare Metal)

## 1. Objetivo
Se logró comprender los conceptos bases del funcionamiento de los cortex C3 con la intencion de poder entender cómo afectan los archivos startup, linker, y main en la memoria y ejecucion

## 2. Estructura del Proyecto

### Archivos principales

- `startup.c` → Inicialización del sistema
- `linker.ld` → Organización de memoria
- `main.c` → Lógica principal

---

## 3. Análisis del Arranque

### Tabla de componentes

| Elemento | Ubicación | Descripción |
|----------|----------|------------|
| Reset_Handler | startup.c | Función inicial tras reset |
| Stack Pointer | linker.ld | Dirección inicial de pila |
| Vector Table | startup.c | Tabla de interrupciones |
| .data | linker.ld | Variables inicializadas |
| .bss | linker.ld | Variables en cero |

---

### 📸 Evidencia

#### Vector Table en startup

![Vector Table](capturas/vector_table.png)

---

#### Sección .data en linker

![.data](capturas/data_section.png)

---

### Explicación del arranque

El `Reset_Handler` es la primera función que se ejecuta luego de un reset del microcontrolador.

Su función principal es inicializar el entorno de ejecución antes de llamar a `main()`. Para ello:

- Copia la sección `.data` desde FLASH a RAM, asegurando que las variables inicializadas tengan su valor correcto.
- Inicializa en cero la sección `.bss`, correspondiente a variables no inicializadas.
- Finalmente, llama a la función `main()`, donde comienza la lógica del programa.

De esta forma, se garantiza que el sistema esté correctamente preparado antes de ejecutar el código principal.

---

## 4. Ejecución Normal

### Compilación

```bash
make clean  //Para limpiar los binarios generados
make        //Para generar los binarios desde el main y el startup
```

### Uso de memoria

Ejecutar:

```bash
make size
```

Captura:

![Uso de memoria](capturas/03_size.png)

El programa ocupa 436 bytes de memoria FLASH (.text) y no utiliza memoria para variables inicializadas (.data) ni no inicializadas (.bss).

---

### Análisis del archivo `.map`

Se abrió el archivo generado en:

```
bin/blink_minimal.elf.map
```

Se agrupó las siguientes secciones para sacarles una screenshot:

- `.isr_vector`
- `.text`
- `.data`
- `.bss`

Captura:

![Secciones en .map](capturas/04_map_sections.png)

En el archivo `.map` se observa que:

- La sección `.vectors` se ubica al inicio de la memoria FLASH (0x08000000), donde el microcontrolador busca la tabla de vectores al arrancar
- La sección `.text`, que contiene el código ejecutable, se encuentra en FLASH a partir de la dirección 0x080000d4
- Las secciones `.data` y `.bss` se ubican en RAM (0x20000000)
- En este caso, tanto `.data` como `.bss` tienen tamaño 0, lo que indica que no hay variables globales utilizadas en el programa

---

### Explicación

- `.text`: contiene el código ejecutable
- `.data`: variables inicializadas, copiadas de FLASH a RAM
- `.bss`: variables no inicializadas, las cuales se inicializan en 0 durante la ejecucion
- `.isr_vector`: tabla de vectores de interrupción

---

## 5. Falla A — Vector Table Incorrecta

### Modificación realizada

Se modificó la ubicación de la vector table en el archivo `linker.ld`.
De la siguiente manera
```ld
.vectors 0x08001000 : {
    *(.isr_vector)
} > FLASH
```

---

### 📸 Evidencia

![Falla A - linker](capturas/05_fallaA_linker.png)

![Falla A - map](capturas/06_fallaA_map.png)

---

### Predicción

Al ejecutarlo, el microcontrolador no podrá encontrar correctamente el `Reset_Handler`, ya que la tabla de vectores no estará en la dirección esperada al inicio de la memoria FLASH.

---

### Resultado observado



---

### Corrección

Restaurar la ubicación original de `.vectors` en el linker.

---

## 6. Falla B — Inicialización incorrecta de `.data`

### 🔧 Modificación realizada

Se modificó la copia de `.data` en `startup.c`.

Ejemplo incorrecto:

```c
uint32_t *src = &_sdata; // incorrecto
```

---

### 📸 Evidencia


![Falla B - código](capturas/07_fallaB_codigo.png)

---

### Predicción

Las variables inicializadas no recibirán sus valores correctos, ya que no se copiarán desde FLASH a RAM correctamente.

---

### Resultado observado

Actualmente el programa no utiliza la seccion .data, por lo que se agregó una variable para visibilizar esta modificacion.

Se le añadió

```c
uint32_t a = 1;
```

Las variables contienen valores incorrectos o basura.

---

### Corrección

Restaurar:

```c
uint32_t *src = &_load_address;
```

---

## 7. Falla C — Configuración incorrecta de RAM

### Modificación realizada

Se modificó el tamaño de RAM en `linker.ld`.

Ejemplo:

```ld
RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 2K
```

---

### 📸 Evidencia

![Falla C - linker](capturas/08_fallaC_linker.png)  

![Falla C - map](capturas/09_fallaC_map.png)

---

### Predicción

Puede producirse:

- falta de memoria
- corrupción de datos
- comportamiento errático

---

### Resultado observado

Al reducir el tamaño de la RAM en el linker, se observa que las secciones del programa pueden exceder el espacio disponible.

Esto puede provocar errores de compilación, o bien comportamiento errático en ejecución debido a falta de memoria o solapamiento de secciones.

---

###  Corrección

Restaurar tamaño original de RAM:

```ld
LENGTH = 20K
```

---

##  8. Conclusión

Reflexión final:

Se pudo observar que el proceso real de arranque depende de múltiples componentes:

- la tabla de vectores
- el linker script
- el código de inicialización (`startup.c`)

Errores en cualquiera de estos elementos pueden impedir que el programa siquiera alcance la función `main()`.

##  9. Proximos pasos

Debido a inconvenientes en la blue pill que impidieron el correcto flasheo de la placa, quedó pendiente la recopilacion de datos en momento de ejecucion. Se pudo explorar las modificaciones en la compilacion, sin embargo el debuggeo y el analisis de las variables en runtime queda pendiente para ampliarlo en proximas versiones
