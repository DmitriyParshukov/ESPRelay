#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/*-------------CONFIG--------------------*/
const char*  WIFI_SSID      = "****";
const char*  WIFI_Password  = "****";
const String Relay_Password = "****";
const int    Relay_PIN      = 16;
/*-------------CONFIG--------------------*/

ESP8266WebServer server(80);

void setup(void) {
  pinMode(Relay_PIN, OUTPUT);
  digitalWrite(Relay_PIN, LOW);
  
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_Password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

void handleRoot() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    Serial.println(server.arg(0));
    Serial.println(server.arg(1));

    if(server.arg(0) == Relay_Password) {
       if(server.arg(1) == "ON") {
           server.send(200, "text/plain", "ON");
           digitalWrite(Relay_PIN, HIGH);
       }
       else if(server.arg(1) == "OFF") {
           server.send(200, "text/plain", "OFF");
           digitalWrite(Relay_PIN, LOW);
       }
       else {
           server.send(200, "text/plain", "UNKNOWN_COMMAND");
       }
    }
    else {
       server.send(401, "text/plain", "AUTH_FAIL");
    }
  }
}
