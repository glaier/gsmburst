# gsmburst

Generate GSM burst on Raspberry Pi on GPIO 14 (UART TX)

The idea for the program came from a very old concept. There is an anecdote the old PC/Workstation platforms could be used to generate a jamming buzz on 2.5MHz or similar, combining the UART pin port (Universal Asynchronous Receiver/Transmitter) on the serial ports with the PWM (Pulse Width Modulator) on the soundboard and a suitable antenna, e.g. a wire to the chassis cover from the serial pin. This is basically what is going on in [PiFM](https://github.com/rm-hull/pifm), [fm_transmitter](https://github.com/markondej/fm_transmitter/), [rpitx](https://github.com/F5OEO/rpitx) or other Raspberry Pi radio projects that require an antenna on the UART TX port on the Raspberry Pi. Usually, the GPIO14 pin on the old models of Raspberry Pi is connected to a 50cm installation 1.5mm2 insulated copper cable, acting as antenna to maximize the effect of the output. It should also be possible to program modules based on Micro RP2040, Pololu A-Star 32U4 Micro, or Seeed Studio XIAO SAMD21 using the Arduino and IDEs like the Thonny IDE or Arduino IDE, and then add an antenna maybe even an attenuator (passive or active). 

> [!WARNING]
> WORK IN PROGRESS - OPENLAB VERSION - FRESH IDEA FROM THE WAFER

## Install necessary packages and libraries. 

The test environment is a Raspberry Pi 3 with Raspberry Pi OS 32 Bit

https://www.raspberrypi.com/software/operating-systems/#raspberry-pi-os-32-bit

### Libraries Used:

WiringPi: Install WiringPi library for GPIO control in C/C++.

Download package file from https://github.com/WiringPi/WiringPi/releases

```
sudo dpkg -i wiringpi_3.6_armhf.deb
```

## Explanation:

### WiringPi: 
Provides easy GPIO control on the Raspberry Pi.

### GPIO Setup:
wiringPiSetup() initializes WiringPi library.

pinMode(gpioPin, OUTPUT) sets the GPIO pin mode for transmission.




## Transmitting GSM Burst:

Inside the while loop, the burst samples are transmitted sequentially by toggling the GPIO pin (gpioPin) high and low.

Adjust usleep timings based on GSM burst rate and Raspberry Pi capabilities.


## Compilation and Execution:

Compile the C++ program (transmitter.cpp) on the Raspberry Pi:

```
sudo apt-get install git
git clone https://github.com/glaier/gsmburst
cd gsmburst
g++ -o transmitter transmitter.cpp -lwiringPi -lm
```

## Run:

Run the compiled program with a specified frequency (in MHz):

```
./transmitter 975  # Example for 975 MHz
```

## Notes:

Hardware Requirements: Ensure your Raspberry Pi setup includes appropriate RF hardware and antenna for transmitting GSM signals.

Timing and Calibration: Fine-tune usleep timings for GPIO control to match GSM burst timing (4.615 ms per GSM burst).

This standalone C++ program demonstrates how to generate and transmit GSM bursts on a Raspberry Pi, using C/C++ and GNU Radio (osmocom) libraries. Adjust the GPIO pin (gpioPin) and GSM parameters (arfcn, bs_power, etc.) based on your specific hardware setup and GSM network requirements.

>[!NOTE]
>Additional idea: Add an attenuator and an antenna to the circuit using alligator clips to SMA connector to the UART pin (remember to ground the RF cable). Alligator to SMA cables and other accessories can be bought on AliExpress. 
