# gsmburst

Generate GSM burst on Raspberry Pi on GPIO 14 (UART TX)

The idea for the program came from a very old concept. There is an anecdote the old PC/Workstation platforms could be used to generate a jamming buzz on 2.5MHz or similar, combining the UART pin port (Universal Asynchronous Receiver/Transmitter) on the serial ports with the PWM (Pulse Width Modulator) on the soundboard and a suitable antenna, e.g. a wire to the chassis cover from the serial pin. This is basically what is going on in [PiFM](https://github.com/rm-hull/pifm), [fm_transmitter](https://github.com/markondej/fm_transmitter/), [rpitx](https://github.com/F5OEO/rpitx) or other Raspberry Pi radio projects that require an antenna on the UART TX port on the Raspberry Pi. Usually, the GPIO14 pin on the old models of Raspberry Pi is connected to a 50cm installation 1.5mm2 insulated copper cable, acting as antenna to maximize the effect of the output. It should also be possible to program modules based on Micro RP2040, Pololu A-Star 32U4 Micro, or Seeed Studio XIAO SAMD21 using the Arduino and IDEs like the Thonny IDE or Arduino IDE, and then add an antenna maybe even an attenuator (passive or active). 

> [!WARNING]
> WORK IN PROGRESS - OPENLAB VERSION - FRESH IDEA FROM THE WAFER

## Install necessary packages and libraries. 

The test environment is a Raspberry Pi 3 with Raspberry Pi OS 32 Bit

https://www.raspberrypi.com/software/operating-systems/#raspberry-pi-os-32-bit

## Transmitting GSM Burst:
Inside the while loop, the burst samples are transmitted sequentially by toggling the GPIO pin high and low. The rate, i.e. transmission timing, determines the frequency of the transmitted signal.

## Explanation:

### Frequency Validation and Parsing: 
The frequency is validated and parsed as before.

### Delay Calculation: 
A delay is calculated based on the provided frequency. This delay is used to adjust the timing of the transmission.

### Adjusting Transmission Timing: 
The transmit_gsm_burst function takes an additional parameter delayMicroseconds, which is used to control the timing between each burst sample.

### Notes:
The delay calculation int delayMicroseconds = static_cast<int>(1e6 / (frequency * 1e6 / GSM_BURST_LENGTH)); is a simplified example. You may need to adjust this based on the actual timing requirements for your transmission.
Directly accessing GPIO through memory-mapped I/O is low-level and can be complex. Make sure to run this code with appropriate permissions (e.g., as root) since it accesses /dev/mem.
This implementation transmits the GSM burst while adjusting the timing based on the provided frequency parameter. This code achieves the desired functionality without relying on external libraries like WiringPi. It directly manipulates the GPIO registers for controlling the GPIO14 (UART TX) pin.

## Compilation and Execution:

Compile the C++ program (transmitter.cpp) on the Raspberry Pi:

```
sudo apt-get install git
git clone https://github.com/glaier/gsmburst
cd gsmburst
g++ -o transmitter transmitter.cpp -lstdc++
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

# Supplementary function signal generator 

## Compile
Compile the C++ program (transmitter.cpp) on the Raspberry Pi:

```
cd gsmburst
g++ -o transmit_consig transmit_consig.cpp -lstdc++
```

## Run:
Run the compiled program with a specified frequency (in MHz):

```
./transmit_consig 002  # Example for 2 MHz
```

## Relay Jammer
For lower frequencies, e.g. 2MHz, it may be advised to use the function signal generator on a car relay. The 30 connector is the COM port and should receive the signal from the raspberry pi, while the 85 and 86 connectors are for the (12V) power supply. 87(a) should be connected tight to a 50cm antenna, e.g. 1.5mm2 installation cable wire. There is a limit given by both the relay and the physically concepts involved in the relay jammer. Do not expect to be able to take down 4G, 5G, ... signals entirely with this solution. 
