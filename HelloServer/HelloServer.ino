#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Dedar Test"
#define STAPSK  "25802580"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char index_html[] PROGMEM = {"<!DOCTYPE html>\n"
"<html lang=\"en\">\n"
"<head>\n"
"    <meta charset=\"UTF-8\">\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
"    <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n"
"    <title>Lan Messenger</title>\n"
"</head>\n"
"<body>\n"
"    <div id=\"getMessenger\">\n"
"        <div style=\"width:400px; margin:0 auto;\">\n"
"            <object data=\"https://lanchat-1bdd3.firebaseapp.com/public/\" width=\"400\" height=\"600\">\n"
"                <embed src=\"https://lanchat-1bdd3.firebaseapp.com/public/\" width=\"400\" height=\"600\"> </embed>\n"
"            </object>\n"
"        </div>\n"
"    </div>\n"
"\n"
"    <div id=\"ErrorMessage\" hidden>\n"
"        <img src=\"http://gg.gg/frn1r\" alt=\"\" srcset=\"\">\n"
"    </div>\n"
"</body>\n"
"</html>\n"
"\n"
"<script>\n"
"    var currentLocation = window.location.href;\n"
"    var requiredLocation = \"http://192.168.0.105/\";\n"
"\n"
"    if (currentLocation == requiredLocation) {\n"
"        document.getElementById(\"getMessenger\").hidden = false;\n"
"    } else {\n"
"        document.getElementById(\"ErrorMessage\").hidden = false;\n"
"    }\n"
"</script>"};



ESP8266WebServer server(80);

const int led = 13;

void handleHtml(){
  server.send_P(200,"text/html",index_html);
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleHtml);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
