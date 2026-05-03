# Nombre del ejecutable
TARGET = blink_minimal.elf

# Compilador
CC = arm-none-eabi-gcc

# Flags de compilación y linkeo
CFLAGS = -Wall -Wextra -mcpu=cortex-m3 -g
LDFLAGS = -T linker.ld -nostartfiles -mcpu=cortex-m3 -Wl,-Map=$(BIN_DIR)/$(TARGET).map

# Directorios
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Archivos fuente y objeto
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Regla por defecto
.PHONY: all
all: $(BIN_DIR)/$(TARGET).bin

# Reglas para generar binarios
$(BIN_DIR)/$(TARGET).bin: $(BIN_DIR)/$(TARGET)
	arm-none-eabi-objcopy -O binary $< $@

$(BIN_DIR)/$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Compilación de archivos fuente
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Crear directorios si no existen
$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

# Limpieza
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*

# Flasheo con OpenOCD
.PHONY: flash
flash: $(BIN_DIR)/$(TARGET)
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $^ verify reset exit"

# Abrir OpenOCD
.PHONY: openocd
openocd:
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg

# Conexión con GDB
.PHONY: gdb
gdb: $(BIN_DIR)/$(TARGET)
	gdb-multiarch $^ -ex "target remote localhost:3333"

# Tamaño de la imagen
.PHONY: size
size: $(BIN_DIR)/$(TARGET)
	arm-none-eabi-size $^

# Ayuda
.PHONY: help
help:
	@echo "Comandos disponibles:"
	@echo "  make           → Compila y genera .elf y .bin"
	@echo "  make flash     → Programa el microcontrolador"
	@echo "  make gdb       → Lanza GDB"
	@echo "  make openocd   → Inicia OpenOCD"
	@echo "  make clean     → Borra archivos generados"
	@echo "  make size      → Muestra uso de memoria"
