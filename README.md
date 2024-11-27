## Raspberry Pi Pico W IMU Data Logger

**Introduction**

This project provides an Arduino IDE code for collecting and storing real-time data from an IMU (Inertial Measurement Unit) on a Raspberry Pi Pico W. The data is saved on an SD card, making it ideal for applications involving off-road vehicles, drones, or robots operating in uneven terrain.

**Hardware Requirements:**

* Raspberry Pi Pico W
* MicroSD card
* IMU sensor (e.g., MPU6050)
* Breadboard and jumper wires

**Software Requirements:**

* Arduino IDE (version 1.8.19 or later)
* Adafruit MPU6050 library

**Instructions**

1. **Install Libraries:**
    * Download the Adafruit MPU6050 library from [https://github.com/adafruit/Adafruit_MPU6050](https://github.com/adafruit/Adafruit_MPU6050) and install it in your Arduino IDE library manager.
    * Download the Raspberry Pico W Support from [https://github.com/earlephilhower/arduino-pico/](https://github.com/earlephilhower/arduino-pico/).

2. **Connect the Hardware:**
    * Refer to the specific wiring diagrams for your IMU sensor and connect it to the Raspberry Pi Pico W according to the pinout. You can find many online resources for wiring diagrams based on your specific IMU model.

3. **Upload the Code:**
    * Open the `main.ino` (or your main code file) in Arduino IDE.
    * Select the appropriate board and serial port for your Raspberry Pi Pico W.
    * Upload the code to your Raspberry Pi Pico W.

**Functionality**

* The code initializes the SD card and the IMU sensor.
* It creates a header row in a CSV file on the SD card with labels for accelerometer and gyroscope data.
* Three buttons are used to define the state of the system: idle, movement, and fall. Based on the pressed button, the code collects data and stores it in the SD card with a corresponding label.
* An LED connected to each button indicates the current state.
* You can choose to either write data to the SD card or read existing data from the SD card using a dedicated variable `write`.

**Image of Hardware Setup**
![Sistema montado](https://github.com/marcos-bah/offroad-collector/blob/main/system.jpg)

**Additional Notes**

* The code can be further customized to adjust the sampling rate, data format, and other parameters. 
* Make sure to calibrate your IMU sensor for accurate data readings.

**Troubleshooting**

* If the SD card is not recognized, ensure it is properly formatted and inserted correctly. 
* Double-check your wiring connections between the IMU sensor and the Raspberry Pi Pico W.
* Verify that you have installed the Adafruit MPU6050 library correctly.

**License**

This project is provided under an open-source license (specify the license you are using). You are free to modify and use the code for your own projects.
