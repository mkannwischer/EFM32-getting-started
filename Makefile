# Author: Matthias J. Kannwischer <matthias@kannwischer.eu>

PREFIX ?= arm-none-eabi
CC      = $(PREFIX)-gcc
LD      = $(PREFIX)-gcc
AR      = $(PREFIX)-ar
OBJCOPY = $(PREFIX)-objcopy

ARCH_FLAGS  = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
DEFINES     = -DEFM32GG11B820F2048GL192

INCLUDES    = -Iefm32-base/device/EFM32GG11B/Include \
              -Iefm32-base/cmsis/Include -Iefm32-base/emlib/inc

CFLAGS += -O3 \
          -Wall -Wextra -Wimplicit-function-declaration \
          -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
          -Wundef -Wshadow \
          -fno-common $(ARCH_FLAGS) -MD $(DEFINES) $(INCLUDES)

EFM32GG11BOBJ    = GCC/startup_efm32gg11b.o system_efm32gg11b.o
LIBEFM32GG11BOBJ = $(addprefix build/efm32-base/device/EFM32GG11B/Source/,$(EFM32GG11BOBJ))
LIBEFM32GG11B    = build/efm32-base/device/EFM32GG11B/libdevice.a

EMLIBSRC = $(wildcard efm32-base/emlib/src/*.c)
EMLIBOBJ = $(addprefix build/,$(EMLIBSRC:.c=.o))
EMLIB    = build/efm32-base/emlib/emlib.a

LDSCRIPT = efm32-base/device/EFM32GG11B/Source/GCC/efm32gg11b.ld
LDFLAGS  =  $(ARCH_FLAGS) -fno-builtin -ffunction-sections -fdata-sections \
           -fomit-frame-pointer -T$(LDSCRIPT) -lgcc -lc -lnosys

SRCS = hal-efm32gg.c hal-efm32gg-aes.c hal-efm32gg-sha2.c hal-efm32gg-mul.c
OBJS = $(addprefix build/,$(SRCS:.c=.o))

all: efm32-test.bin efm32-aes.bin efm32-sha2.bin efm32-mul.bin

efm32-%.elf: %.o $(LIBEFM32GG11B) $(EMLIB) $(OBJS)
	$(LD) -o $@ $(OBJS) $< $(LIBEFM32GG11B) $(EMLIB) $(LDFLAGS)


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

$(EMLIB): $(EMLIBOBJ)
	$(AR) qc $@ $?

$(LIBEFM32GG11B): $(LIBEFM32GG11BOBJ)
	$(AR) qc $@ $?

build/%.o: %.S
	mkdir -p $(@D)
	$(CC) -o $@ -c $(CFLAGS) $<

build/%.o: %.c
	mkdir -p $(@D)
	$(CC) -o $@ -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rf build
	rm -rf *.elf
	rm -rf *.bin
	rm -rf *.o
	rm -rf *.d

