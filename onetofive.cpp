#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <cstring>

// Define base addresses and offsets for GPIO
#define GPIO_BASE 0x3F200000  // For Raspberry Pi 2/3
#define BLOCK_SIZE (4*1024)
#define GPIO_SET *(gpio + 7)
#define GPIO_CLR *(gpio + 10)
#define GPIO_LEV *(gpio + 13)
#define GPIO_FSEL1 *(gpio + 1)

volatile unsigned *gpio;

// Function to set GPIO14 as output
void setup_gpio() {
    GPIO_FSEL1 &= ~(7 << 12);  // Clear bits 12-14
    GPIO_FSEL1 |= (1 << 12);   // Set GPIO14 as output
}

// Function to initialize memory-mapped I/O
void init_gpio() {
    int mem_fd;
    void *gpio_map;

    mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd < 0) {
        std::cerr << "Unable to open /dev/mem" << std::endl;
        exit(EXIT_FAILURE);
    }

    gpio_map = mmap(
        NULL,             // Any address in our space will do
        BLOCK_SIZE,       // Map length
        PROT_READ | PROT_WRITE, // Enable reading & writing to mapped memory
        MAP_SHARED,       // Shared with other processes
        mem_fd,           // File to map
        GPIO_BASE         // Offset to GPIO peripheral
    );

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        std::cerr << "mmap error" << std::endl;
        exit(EXIT_FAILURE);
    }

    gpio = (volatile unsigned *)gpio_map;
    setup_gpio();
}

// Function to generate the sawtooth signal
void generate_sawtooth() {
    int high_time = 2; // 100 MHz frequency corresponds to 10 ns high time (200 MHz would be 5 ns)
    int low_time = 2;

    for (int i = 0; i < 5000000; ++i) {  // Running loop to create 1 MHz to 5 MHz signal
        if (i % 2 == 0) { // ON period
            for (int j = 0; j < 50; ++j) {
                GPIO_SET = 1 << 14;
                usleep(high_time); // Delay for high period
                GPIO_CLR = 1 << 14;
                usleep(low_time);  // Delay for low period
            }
        } else { // OFF period
            usleep(1000); // Adjust to create the desired 1 MHz to 5 MHz envelope
        }
    }
}

int main() {
    init_gpio();
    generate_sawtooth();
    return 0;
}
