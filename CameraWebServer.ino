#include "CameraWebServer.h"



WebServer server(80);
espcamera ESP32Camera(4); // pin

void setup() {
  //ESP32CrashHandler::disableCoreDumps(); // disable core dumps
  Serial.begin(115200);

  // AP name,passwd
  WiFi.softAP(host, password, channel);
  /*  Available ESP32 RF power parameters:
    WIFI_POWER_19_5dBm    // 19.5dBm (For 19.5dBm of output, highest. Supply current ~150mA)
    WIFI_POWER_19dBm      // 19dBm
    WIFI_POWER_18_5dBm    // 18.5dBm
    WIFI_POWER_17dBm      // 17dBm
    WIFI_POWER_15dBm      // 15dBm
    WIFI_POWER_13dBm      // 13dBm
    WIFI_POWER_11dBm      // 11dBm
    WIFI_POWER_8_5dBm     //  8dBm
    WIFI_POWER_7dBm       //  7dBm
    WIFI_POWER_5dBm       //  5dBm
    WIFI_POWER_2dBm       //  2dBm
    WIFI_POWER_MINUS_1dBm // -1dBm( For -1dBm of output, lowest. Supply current ~120mA) */
  WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  /*use mdns for host name resolution*/
  //if (!MDNS.begin(host)) { //http://esp32.local
  //Serial.println("Error setting up MDNS responder!");
  //  while (1) {
  //    delay(1000);
  //  }
  //}
  //Serial.println("mDNS responder started");

  server.on("/color_output", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", color_output);
  });
  server.on("/jquery.min.js", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", _js);
  });
  server.on("/OTA", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "alert('FAIL');" : "alert('OK');");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();

  ESP32Camera.LEDInit();
  ESP32Camera.CameraInit();
}
int i;
void loop() {
  server.handleClient();
  if (i==2000){
    i = 0;
    ESP32Camera.color_detection();
  } else {
    i++;
  }
  delay(1);
}
