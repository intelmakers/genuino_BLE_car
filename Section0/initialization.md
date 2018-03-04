# Initialization of the working environment

## Installing Arduino IDE

1. Make sure that the Arduino IDE is installed. If it is, [you may skip to installing drivers](##Installing-drivers)
1. Install the Arduino IDE from, https://www.arduino.cc/en/Main/Software choose the correct operating system.
1. Follow the installation instructions, allow to install all drivers.

## Installing drivers

1. Open Tools->Boards->Boards manager...
1. Search for Intel Curie, and install the latest version.
1. After the installation is finished, connect the Genuino 101 to your computer. **make sure nothing is connected to the Genuino, only the USB cable**
1. Select the correct COM port (Tools->Port).
1. Select the correct board (Tools->Boards->Arduino/Genuino 101). The board will probably be at the bottom.
1. Open the Blink example (File->Examples->01.Basics->Blink)
1. Upload, it will take quite a while to compile and upload. At the end you will see a message like this: ![SUCCESS: Sketch will execute in about 5 seconds.](./pictures/success.png?raw=true).
1. After 5 seconds, the led on the PCB will blink. If so go grab a :cookie: