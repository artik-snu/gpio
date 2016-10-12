/*
 * memory mapped gpio code for ARTIK 10
 * Source code adapted from http://odroid.com/dokuwiki/doku.php?id=en:xu3_gpio_register
 * modified by Taehee Jeong 13-10-2016
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

static volatile uint32_t *gpio;

int main(int argc, char **argv)
{
    int fd ;

    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        printf("Unable to open /dev/mem\n");
        return -1;
    }

    //gpio0 on ARTIK 10 has base address 0x13400000
    gpio = mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                                0x13400000);
    if (gpio < 0){
        printf("Mmap failed.\n");
        return -1;
    }

    // Print GPX2 configuration register.
    // GPX2 has offset 0x308
    printf("GPX2CON register : 0x%08x\n",
                *(unsigned int *)(gpio + 0x308));


    // Set direction of GPX2.7 configuration register as out.
    *(gpio + 0x308) |= (0x1 << 6);
    printf("GPX2CON register : 0x%08x\n",
                *(unsigned int *)(gpio + 0x308));
    int i;
    for (i = 0; i < 10; i++) {
    // GPX2.7 High
    *(gpio + 0x309) |= (1 << 6);
    printf("GPX2DAT register : 0x%08x\n",
                *(unsigned int *)(gpio + 0x309));
    sleep(1);
    // GPX2.7 Low
    *(gpio + 0x309) &= ~(1 << 6);
    printf("GPX2DAT register : 0x%08x\n",
                *(unsigned int *)(gpio + 0x309));
    sleep(1);
    }

    return 0;
}