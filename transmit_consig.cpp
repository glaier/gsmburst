#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <regex>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// Base address for GPIO registers
#define GPIO_BASE 0x3F200000

// GPIO register offsets
#define GPIO_SET *(gpio + 7)  // Set GPIO high
#define GPIO_CLR *(gpio + 10) // Set GPIO low

volatile unsigned *gpio;

void emit_constant_signal(int gpioPin, int delayMicroseconds);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <frequency (XXX.X format)>" << std::endl;
        return 1;
    }

    std::string freqStr = argv[1];
    std::regex freqRegex("^\\d{3}\\.\\d$");

    if (!std::regex_match(freqStr, freqRegex)) {
        std::cerr << "Error: Frequency must be in the format XXX.X" << std::endl;
        return 1;
    }

    float frequency = std::stof(freqStr);

    int mem_fd;
    void *gpio_map;

    // Open /dev/mem
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        std::cerr << "Failed to open /dev/mem." << std::endl;
        return 1;
    }

    // Map GPIO
    gpio_map = mmap(
        NULL,             // Any address in our space will do
        4096,             // Map length
        PROT_READ | PROT_WRITE, // Enable reading and writing to mapped memory
        MAP_SHARED,       // Shared with other processes
        mem_fd,           // File descriptor to map
        GPIO_BASE         // Offset to GPIO peripheral
    );

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        std::cerr << "mmap error." << std::endl;
        return 1;
    }

    // Always use volatile pointer!
    gpio = (volatile unsigned *)gpio_map;

    std::cout << "Emitting constant signal at frequency " << frequency << " MHz" << std::endl;

    // Calculate delay based on frequency
    int delayMicroseconds = static_cast<int>(1e6 / (2 * frequency * 1e6));

    // Emit constant signal
    emit_constant_signal(14, delayMicroseconds); // GPIO14 (UART TX)

    return 0;
}

void emit_constant_signal(int gpioPin, int delayMicroseconds) {
    int gpio_set = 1 << gpioPin;
    int gpio_clr = 1 << (gpioPin + 10);

    // Emit constant signal
    while (true) {
        GPIO_SET = gpio_set;
        usleep(delayMicroseconds); // Half-period delay
        GPIO_CLR = gpio_clr;
        usleep(delayMicroseconds); // Half-period delay
    }
}
