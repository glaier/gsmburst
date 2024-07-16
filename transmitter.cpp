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

// Length of GSM burst in samples
const int GSM_BURST_LENGTH = 177;
const int GSM_BURST_SAMPLES[GSM_BURST_LENGTH] = {
    1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0,
    1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1,
    0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1,
    0, 1, 1, 0, 1, 1, 0, 1, 0
};

// GPIO register offsets
#define GPIO_SET *(gpio + 7)  // Set GPIO high
#define GPIO_CLR *(gpio + 10) // Set GPIO low

volatile unsigned *gpio;

void transmit_gsm_burst(int gpioPin);

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

    std::cout << "Transmitting GSM burst at frequency " << frequency << " MHz" << std::endl;

    // Transmit GSM burst
    transmit_gsm_burst(14); // GPIO14 (UART TX)

    return 0;
}

void transmit_gsm_burst(int gpioPin) {
    int gpio_set = 1 << gpioPin;
    int gpio_clr = 1 << (gpioPin + 10);

    // Transmit burst samples
    while (true) {
        for (int i = 0; i < GSM_BURST_LENGTH; ++i) {
            if (GSM_BURST_SAMPLES[i]) {
                GPIO_SET = gpio_set;
            } else {
                GPIO_CLR = gpio_clr;
            }
            usleep(0.5); // Adjust timing as needed defaulted to 10 due to setting in fm_transmitter project
        }
    }
}
