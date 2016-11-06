SDKDIR=${HOME}/tizen-studio
DEVICE_CORE=${SDKDIR}/platforms/tizen-2.4/mobile/rootstraps/mobile-2.4-device.core/

LINK=-L${DEVICE_CORE}/usr/lib -L${DEVICE_CORE}/lib

INCLUDE=-I${DEVICE_CORE}/usr/include

# EDIT KERNEL CODE PATH
INCLUDE+=-I/home/wwee3631/Projects/samsung/linux-exynos/include

SYSROOT=${DEVICE_CORE}

GCCROOT=${SDKDIR}/tools/arm-linux-gnueabi-gcc-4.9/bin

CFLAGS=$(INCLUDE) $(LINK) --sysroot=$(SYSROOT) -lcapi-system-sensor

CC=$(GCCROOT)/arm-linux-gnueabi-gcc

all: test

DEPS = gpio.h

%.o: %.c ${DEPS}
	$(CC) -c -o $@ $< $(CFLAGS)

OBJ = read.o gpio.o

test: ${OBJ}
	@echo [Arm-cc] $<...
	@$(CC) -o $@ $^ ${CFLAGS}

clean:
	@rm test
