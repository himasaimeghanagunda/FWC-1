#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "esp32_eoss3_spi.h"
//#include "credentials.h"

#define SW_MB_1 0x40005110

AsyncWebServer server(80);

const char *ssid = "Gunda";
const char *password = "987654320";
const char *PARAM_X1 = "x1";
const char *PARAM_Y1 = "y1";
const char *PARAM_X2 = "x2";
const char *PARAM_Y2 = "y2";
const char *PARAM_X3 = "x3";
const char *PARAM_Y3 = "y3";
//const char* PARAM_DIGIT = "digit";
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Triangle Midpoints</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.0rem; color: blue;}
  </style>
  <script>
    function updateResults(response) {
      document.getElementById("results").innerHTML = response;
    }

    function calculate() {
      var form = document.getElementById("triangleForm");
      var formData = new FormData(form);
      fetch('/mid', {
        method: 'POST',
        body: formData
      })
      .then(response => response.text())
      .then(data => updateResults(data));
      
      // Prevent form submission and page reload
      return false;
    }
  </script>
  </head>
  <body>
  <h2>Triangle Midpoints</h2>
  <form id="triangleForm" onsubmit="return calculate()">
    <label for="x1">Vertex 1 X:</label>
    <input type="text" name="x1" required>
    <label for="y1">Y:</label>
    <input type="text" name="y1" required><br>

    <label for="x2">Vertex 2 X:</label>
    <input type="text" name="x2" required>
    <label for="y2">Y:</label>
    <input type="text" name="y2" required><br>

    <label for="x3">Vertex 3 X:</label>
    <input type="text" name="x3" required>
    <label for="y3">Y:</label>
    <input type="text" name="y3" required><br>

    <input type="submit" value="Calculate">
  </form>
  <div id="results">%RESULT%</div>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  esp32_eoss3_spi_init();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(2000);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/mid", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/mid", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_DIGIT)) {
uint32_t digit = request->getParam(PARAM_X1)->value().toInt();
uint32_t digit = request->getParam(PARAM_Y1)->value().toInt();
uint32_t digit = request->getParam(PARAM_X2)->value().toInt();
uint32_t digit = request->getParam(PARAM_Y2)->value().toInt();
uint32_t digit = request->getParam(PARAM_X3)->value().toInt();
uint32_t digit = request->getParam(PARAM_Y3)->value().toInt();
        esp32_eoss3_spi_ahb_write(SW_MB_1, (uint8_t *)&digit, 4);
    }
    request->send(200, "text/html", index_html);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {}
