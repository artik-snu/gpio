SDKDIR=${HOME}/tizen-studio
DEVICE_CORE=${SDKDIR}/platforms/tizen-3.0/mobile/rootstraps/mobile-3.0-device.core/

LINK=-L${DEVICE_CORE}/usr/lib -L${DEVICE_CORE}/lib

INCLUDE=-I${DEVICE_CORE}/usr/include

# EDIT KERNEL CODE PATH
INCLUDE+=-I/home/wwee3631/Projects/samsung/linux-exynos/include



SYSROOT=${DEVICE_CORE}

GCCROOT=${SDKDIR}/tools/arm-linux-gnueabi-gcc-4.9/bin

CFLAGS=$(INCLUDE) $(LINK) --sysroot=$(SYSROOT) --std=c++11

CC=$(GCCROOT)/arm-linux-gnueabi-g++

all: gpio_service

DEPS = gpio.h

%.o: %.cpp ${DEPS}
	$(CC) -c -o $@ $< $(CFLAGS)

OBJ = gpio.o

gpio_service: ${OBJ}
	@echo [Arm-cc] $<...
	@$(CC) -o $@ $^ ${CFLAGS}

clean:
	@rm gpio_service
