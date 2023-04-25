Arduino-time-weather
====================

This is an Arduino script that displays the date, time, weather conditions, and temperature.

Required Components
-------------------

-   LCD with I2C module
-   Wemo D1 board with WiFi
-   GND to GND connection
-   VCC to 5V connection
-   SDA to D15 connection
-   SCL to D14 connection

Usage
-----

1.  Connect the required components as described above.
2.  Upload the `Arduino-time-weather` sketch to your Arduino board.
3.  Open the serial monitor and wait for it to display the IP address.
4.  Open a web browser and navigate to the IP address.
5.  The display will show the current time, date, weather conditions, and temperature.

Note: You will need to modify the code with your own OpenWeatherMap API key and location information to get the weather information.

![image](https://user-images.githubusercontent.com/26854208/234379754-0dbd3e6b-a821-4c04-a32b-0b5738105970.png)
