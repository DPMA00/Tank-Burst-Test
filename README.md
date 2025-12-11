# Tank-Burst-Test

This repository contains software for a remote burst test for a student rocket team's pressure vessel.

| Hardware | Description| 
|:---:|:--:|
|![alt text](image.png)| ESP32 |
|![alt text](image-2.png) | 5V DC Pressure Transducer (0-300PSI)|
|![alt text](image-3.png)| $3 \times 10k\Omega$ resistors for a 2:3 voltage divider circuit|


## Circuit

![alt text](image-5.png)

## Software

The ESP32 acts as a TCP server which publishes pressure readings continously while a client is connected remotely through Wifi. 

Because the ADC pins on the ESP32 exhibit nonlinear behvaior, especially near 0V and 3.3V, calibration data is collected and a 3rd-order polyfit is used to estimate the voltage readings.

$$V_{ADC} = \frac{ADC_{raw}}{4095} \times 3.3V$$
$$V_{sensor} = V_{ADC} \times \frac{3}{2}$$

![alt text](image-6.png)

