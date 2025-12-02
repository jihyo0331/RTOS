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

.PHONY: all clean run debug gdb

all: $(navilos)

clean:
	@rm -fr $(OUTDIR)
	
MONITOR_PORT ?= 5555

run: $(navilos)
	( stty raw -echo; qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -nographic -serial stdio -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait; stty sane )
	
debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 -m 1 -kernel $(navilos) -nographic -serial stdio -monitor telnet:127.0.0.1:$(MONITOR_PORT),server,nowait -S -gdb tcp::1234,ipv4
	
gdb:
	gdb-multiarch $(navilos)

kill:
	kill -9 `ps aux | grep 'qemu' | awk 'NR==1{print $$2}'`
	
$(navilos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	mkdir -p $(OUTDIR)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS) $(C_OBJS) -Wl,-Map=$(MAP_FILE) $(LDFLAGS)
	$(OD) -t $(navilos) > $(SYM_FILE)
	$(OC) -O binary $(navilos) $(navilos_bin)
	
$(OUTDIR)/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -marm $(INC_DIRS) $(CFLAGS) -o $@ $<
	
$(OUTDIR)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -marm $(INC_DIRS) $(CFLAGS) -o $@ $<
	
