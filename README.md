# Tank-Burst-Test

This repository contains software for a remote burst test for a student rocket team's pressure vessel.

| Hardware | Description| 
|:---:|:--:|
|<img width="245" height="176" alt="Image" src="https://github.com/user-attachments/assets/455356e0-4941-4d91-9304-9fb3b31a5ae1" />| ESP32 |
|<img width="166" height="184" alt="Image" src="https://github.com/user-attachments/assets/cccf443b-98bf-49fc-99ef-c80e56a6eb62" />| 5V DC Pressure Transducer (0-300PSI)|
|<img width="193" height="176" alt="Image" src="https://github.com/user-attachments/assets/51575596-a885-4344-b4ed-e29a4757c386" />| $3 \times 10k\Omega$ resistors for a 2:3 voltage divider circuit|


## Circuit

<img width="586" height="354" alt="Image" src="https://github.com/user-attachments/assets/fc8dafdc-13c9-43d6-bf81-72823797b535" />

## Software

The ESP32 acts as a TCP server which publishes pressure readings continously while a client is connected remotely through Wifi. 

Because the ADC pins on the ESP32 exhibit nonlinear behvaior, especially near 0V and 3.3V, calibration data is collected and a 3rd-order polyfit is used to estimate the voltage readings.

$$V_{ADC} = \frac{ADC_{raw}}{4095} \times 3.3V$$
$$V_{sensor} = V_{ADC} \times \frac{3}{2}$$

<img width="420" height="297" alt="Image" src="https://github.com/user-attachments/assets/1c22a820-0954-449d-a5d9-86dfa12e96fc" />

