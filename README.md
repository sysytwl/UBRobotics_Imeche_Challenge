# UBRobotics_Imeche_Challenge V2

## 1. Using Esp32-Cam module
1. Pin Out:
* ![ESP32-CAM-pinout-1](https://github.com/sysytwl/UBRobotics_Imeche_Challenge/assets/61526569/a11bcfe0-291e-4d84-baa4-a3f189bd8417)


2. The following pins are internally connected to the microSD card reader:
  - GPIO 14: CLK
  - GPIO 15: CMD
  - GPIO 2: Data 0
  - GPIO 4: Data 1 (also connected to the on-board LED)
  - GPIO 12: Data 2
  - GPIO 13: Data 3

2. I2C: https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/
3. SPI: https://randomnerdtutorials.com/esp32-spi-communication-arduino/

4. VL53L1X: 
* https://github.com/sparkfun/SparkFun_VL53L1X_Arduino_Library
* https://github.com/adafruit/Adafruit_VL53L1X

5. Drv8323s https://www.ti.com/lit/ds/symlink/drv8323.pdf?ts=1696583967631&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FDRV8323%252Fpart-details%252FDRV8323HRTAR%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Docb-tistore-promo-asc_opn_en-cpc-storeic-google-eu%2526utm_content%253DDevice%2526ds_k%253DDRV8323HRTAR%2526DCM%253Dyes%2526gclid%253DCjwKCAjw4P6oBhBsEiwAKYVkqxM8LNXa65d2P4P0Zs-43Y37ug8vBuUrvKhRZgitHXOVf0nLtY7RMBoCx1IQAvD_BwE%2526gclsrc%253Daw.ds

