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

/*
 * OTA test file
 */
const char* test PROGMEM = R"HTMLTESTPAGE(
    <dir>V2.2</dir>
)HTMLTESTPAGE";

/*
 * JavaScript
 */
const char* _js PROGMEM = R"HTMLHOMEPAGE()HTMLHOMEPAGE";

/*
 * Server Index Page
 */
const char* serverIndex PROGMEM = R"HTMLHOMEPAGE(
<script>
</script>
<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
   <input type='file' name='update'>
        <input type='submit' value='Update'>
    </form>
 <div id='prg'>progress: 0%</div>
 <script>
  $('form').submit(function(e){
  e.preventDefault();
  var form = $('#upload_form')[0];
  var data = new FormData(form);
   $.ajax({
  url: '/update',
  type: 'POST',
  data: data,
  contentType: false,
  processData:false,
  xhr: function() {
  var xhr = new window.XMLHttpRequest();
  xhr.upload.addEventListener('progress', function(evt) {
  if (evt.lengthComputable) {
  var per = evt.loaded / evt.total;
  $('#prg').html('progress: ' + Math.round(per*100) + '%');
  }
  }, false);
  return xhr;
  },
  success:function(d, s) {
  console.log('success!')
 },
 error: function (a, b, c) {
 }
 });
 });
 </script>
 )HTMLHOMEPAGE";