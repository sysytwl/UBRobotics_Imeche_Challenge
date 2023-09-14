//#include "esp_timer.h"
#include "esp_camera.h"
//#include "img_converters.h"
//#include "fb_gfx.h"
//#include "esp32-hal-ledc.h"
//#include "sdkconfig.h"
//#include "camera_index.h"
//#include "CameraWebServer.h"
#include "camera_pins.h"
#include "Arduino.h"



class espcamera{
public:
  espcamera(int pin){
    _pin = pin;
    
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_96X96; // 96x96 resolution
    //config.pixel_format = PIXFORMAT_JPEG,
    config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    //config.fb_location = CAMERA_FB_IN_PSRAM;
    config.fb_location = CAMERA_FB_IN_DRAM;
    //config.jpeg_quality = 12;
    config.fb_count = 1;
    //config.grab_mode = CAMERA_GRAB_LATEST; // ?
  }

  void CameraInit(){
    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      return;
    }
  }

  void LEDInit(){
    // LED flash light
    ledcSetup(_LED_LEDC_CHANNEL, 5000, 8);
    ledcAttachPin(_pin, _LED_LEDC_CHANNEL);
  }

  void enable_led(bool en){ // Turn LED On or Off
    //int duty = en ? _led_duty : 0;
    ledcWrite(_LED_LEDC_CHANNEL, _led_duty * (int)en);
  }

  void color_detection(){
    camera_fb_t *fb = NULL;
    enable_led(true);
    //vTaskDelay(150 / portTICK_PERIOD_MS); // The LED needs to be turned on ~150ms before the call to esp_camera_fb_get()
    delay(150);
    fb = esp_camera_fb_get();
    enable_led(false);

    if (fb) {
      // Initialize color accumulators
      uint16_t total_red = 0;
      uint16_t total_green = 0;
      uint16_t total_blue = 0;
      float total_weight = 0.0;

      // Calculate the weighted average color
      for (int x = 0; x < fb->width; x++) {
        int pixel_index = (_line_number * fb->width + x) * 2; // Each pixel is 2 bytes (RGB565)
        uint16_t pixel_color = *((uint16_t *)&fb->buf[pixel_index]);

        // Calculate the weight for this pixel based on its distance from the center
        float weight = _middle_weight - abs(int(x - fb->width / 2));

        // Accumulate weighted color values
        total_red += ((pixel_color >> 11) & 0x1F) * weight;
        total_green += ((pixel_color >> 5) & 0x3F) * weight;
        total_blue += (pixel_color & 0x1F) * weight;

        // Accumulate total weight
        total_weight += weight;
      }

      // Calculate the final weighted average color
      RGBColor_.Red = total_red / total_weight;
      RGBColor_.Green = total_green / total_weight;
      RGBColor_.Blue = total_blue / total_weight;

      // Output the result
      Serial.printf("Weighted Average Color (R,G,B): (%d, %d, %d)\n", RGBColor_.Red, RGBColor_.Green, RGBColor_.Blue);
    }
    esp_camera_fb_return(fb);
  }

  camera_config_t config;

  struct RGBColor{
    int Red, Green, Blue;
  } RGBColor_;

private:
  int _LED_LEDC_CHANNEL = 2; //Using different ledc channel/timer than camera
  int _led_duty = 255;
  int _pin;
  int _line_number = 47;
  float _middle_weight = 2.0;
  String _colorString;
};
