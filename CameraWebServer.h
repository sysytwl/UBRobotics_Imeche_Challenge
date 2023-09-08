#include "esp_camera.h"
#include <WiFi.h>
//#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>



// ===========================
// Enter your WiFi credentials
// ===========================
const char* host = "esp32_UBRobotics";
const char* password = "ubrobotics";

// ===================
// Select camera model
// ===================
#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include "camera_pins.h"



void startCameraServer();
void setupLedFlash(int pin);
