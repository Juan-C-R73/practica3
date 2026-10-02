#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid = "TU_WIFI";
const char* password = "TU_CONTRASEÑA";

WiFiUDP ntpUDP;
// Cambiamos a time.google.com que suele ser más rápido y permisivo
NTPClient timeClient(ntpUDP, "time.google.com", 7200, 60000);

void setup() {
  Serial.begin(115200);
  delay(2000); // Pausa para que el Monitor Serie arranque limpio

  Serial.print("\nConectando a: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡WiFi Conectado!");
  
  // Pausa de seguridad para que el router termine de asignar las rutas
  delay(2000);

  timeClient.begin();
  
  Serial.print("Forzando petición de hora al servidor NTP");
  // forceUpdate() insiste hasta que el servidor responde de verdad
  while(!timeClient.forceUpdate()) {
    Serial.print(".");
    delay(1500); // Espera 1.5s entre intentos para no saturar al servidor
  }
  Serial.println("\n¡Hora capturada con éxito!");
}

void loop() {
  // Ahora el loop solo se dedica a imprimir la hora que ya hemos conseguido
  Serial.print("Hora actual: ");
  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}