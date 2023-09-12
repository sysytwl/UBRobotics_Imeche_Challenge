#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp32-hal-ledc.h"
#include "sdkconfig.h"
#include "camera_index.h"





void setupLedFlash(int pin) {
    ledcSetup(LED_LEDC_CHANNEL, 5000, 8);
    ledcAttachPin(pin, LED_LEDC_CHANNEL);
}

void enable_led(bool en){ // Turn LED On or Off
    int duty = en ? led_duty : 0;
    if (en && isStreaming && (led_duty > CONFIG_LED_MAX_INTENSITY))
    {
        duty = CONFIG_LED_MAX_INTENSITY;
    }
    ledcWrite(LED_LEDC_CHANNEL, duty);
    //ledc_set_duty(CONFIG_LED_LEDC_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL, duty);
    //ledc_update_duty(CONFIG_LED_LEDC_SPEED_MODE, CONFIG_LED_LEDC_CHANNEL);
    log_i("Set LED intensity to %d", duty);
}

void color_detection(){
    camera_fb_t *fb = NULL;
    enable_led(true);
    vTaskDelay(150 / portTICK_PERIOD_MS); // The LED needs to be turned on ~150ms before the call to esp_camera_fb_get()
    fb = esp_camera_fb_get();
    enable_led(false);

    if (fb) {
        uint32_t totalWeight = 0;
        uint32_t totalR = 0, totalG = 0, totalB = 0;

        int centerX = fb->width / 2;
        int centerY = fb->height / 2;

        for (int y = 0; y < fb->height; y++) {
            for (int x = 0; x < fb->width; x++) {
                int distanceToCenter = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY)); // Calculate distance to the center
                int weight = map(distanceToCenter, 0, fb->width / 2, 255, 64);                             // Weight inversely proportional to distance

                uint8_t *pixel = fb->buf + (y * fb->width + x) * 3; // Calculate pixel position

                // Extract color components
                uint8_t r = pixel[0];
                uint8_t g = pixel[1];
                uint8_t b = pixel[2];

                // Accumulate weighted color values
                totalR += r * weight;
                totalG += g * weight;
                totalB += b * weight;
                totalWeight += weight;
            }
        }

        // Calculate the weighted average color
        uint8_t avgR = totalR / totalWeight;
        uint8_t avgG = totalG / totalWeight;
        uint8_t avgB = totalB / totalWeight;

        // Use avgR, avgG, and avgB as the weighted average color
        Serial.print(avgR); Serial.print(", ");
        Serial.print(avgG); Serial.print(", ");
        Serial.println(avgB);
    }

    // Don't forget to free the frame buffer when you're done with it
    esp_camera_fb_return(fb);
}