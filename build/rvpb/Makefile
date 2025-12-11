ARCH = armv7-a

TARGET = rvpb
ARM_ARCH = cortexAR

TOP := ../..
SHELL := /bin/bash

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump

OUTDIR := ./out

LINKER_SCRIPT = navilos.ld
MAP_FILE = $(OUTDIR)/navilos.map
SYM_FILE = $(OUTDIR)/navilos.sym

ASM_SRCS := $(wildcard $(TOP)/boot/$(TARGET)/*.S)
ASM_OBJS := $(patsubst $(TOP)/boot/$(TARGET)/%.S, $(OUTDIR)/%.os, $(ASM_SRCS))

VPATH = $(TOP)/boot/$(TARGET) \
        $(TOP)/hal/$(TARGET)  \
        $(TOP)/lib            \
        $(TOP)/lib/$(ARM_ARCH) \
        $(TOP)/kernel

C_SRCS  := $(notdir $(wildcard $(TOP)/boot/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard $(TOP)/hal/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard $(TOP)/lib/*.c))
C_SRCS += $(notdir $(wildcard $(TOP)/lib/$(ARM_ARCH)/*.c))
C_SRCS += $(notdir $(wildcard $(TOP)/kernel/*.c))
C_OBJS = $(patsubst %.c, $(OUTDIR)/%.o, $(C_SRCS))

INC_DIRS  = -I$(TOP)/boot/$(TARGET) \
            -I$(TOP)/include       \
            -I$(TOP)/hal           \
            -I$(TOP)/hal/$(TARGET) \
            -I$(TOP)/lib           \
            -I$(TOP)/lib/$(ARM_ARCH) \
            -I$(TOP)/kernel

CFLAGS = -c -g -std=c11 -mthumb-interwork -Os -ffunction-sections -fdata-sections

LDFLAGS = -nostartfiles -nostdlib -nodefaultlibs -static -lgcc -Wl,--gc-sections

navilos = $(OUTDIR)/navilos.axf
navilos_bin = $(OUTDIR)/navilos.bin

DEBUG_OUTDIR := $(OUTDIR)/debug
debug_axf = $(DEBUG_OUTDIR)/debug.axf
debug_map = $(DEBUG_OUTDIR)/debug.map
debug_sym = $(DEBUG_OUTDIR)/debug.sym

ASM_OBJS_DEBUG := $(patsubst $(TOP)/boot/$(TARGET)/%.S, $(DEBUG_OUTDIR)/%.os, $(ASM_SRCS))
C_OBJS_DEBUG = $(patsubst %.c, $(DEBUG_OUTDIR)/%.o, $(C_SRCS))

DISPLAY_OPTS ?= sdl

.PHONY: all clean run run-gui run-gui-vc debug debug-gui debug-gui-vc gdb
.PHONY: debug-build

all: $(navilos)

clean:
	@rm -fr $(OUTDIR)
	
MONITOR_PORT ?= 5555

run: $(navilos)
	( stty raw -echo; qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -nographic -serial stdio -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait; stty sane )
	
run-gui: $(navilos)
	qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -display $(DISPLAY_OPTS) -serial stdio -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait

# GUI 창 안에 별도 터미널(virtual console)로 UART를 표시
run-gui-vc: $(navilos)
	qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -display $(DISPLAY_OPTS) -serial vc -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait
	
debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -nographic -serial stdio -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait -S -gdb tcp::1234,ipv4

debug-gui: $(navilos)
	qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -display $(DISPLAY_OPTS) -serial stdio -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait -S -gdb tcp::1234,ipv4

# GUI 창 안에 virtual console 사용 + GDB 대기
debug-gui-vc: $(navilos)
	qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -display $(DISPLAY_OPTS) -serial vc -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait -S -gdb tcp::1234,ipv4
	
gdb:
	gdb-multiarch $(navilos)

kill:
	kill -9 `ps aux | grep 'qemu' | awk 'NR==1{print $$2}'`

# Build an unstripped, O0 debug image
debug-build: $(debug_axf)
	
$(navilos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	mkdir -p $(OUTDIR)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS) $(C_OBJS) -Wl,-Map=$(MAP_FILE) $(LDFLAGS)
	$(OD) -t $(navilos) > $(SYM_FILE)
	$(OC) -O binary $(navilos) $(navilos_bin)

$(debug_axf): $(ASM_OBJS_DEBUG) $(C_OBJS_DEBUG) $(LINKER_SCRIPT)
	mkdir -p $(DEBUG_OUTDIR)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(debug_axf) $(ASM_OBJS_DEBUG) $(C_OBJS_DEBUG) -Wl,-Map=$(debug_map) $(LDFLAGS)
	$(OD) -t $(debug_axf) > $(debug_sym)
	
$(OUTDIR)/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -marm $(INC_DIRS) $(CFLAGS) -o $@ $<
	
$(OUTDIR)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -marm $(INC_DIRS) $(CFLAGS) -o $@ $<

$(DEBUG_OUTDIR)/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -marm $(INC_DIRS) $(CFLAGS) -O0 -o $@ $<

$(DEBUG_OUTDIR)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -marm $(INC_DIRS) $(CFLAGS) -O0 -o $@ $<
	
