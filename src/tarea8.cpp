#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <ArduinoJson.h> // Librería para crear el JSON

// Datos de red WiFi
const char* ssid = "DIGIFIBRA-SGDT";
const char* password = "3X6x2CfudG";

// Configuración NTP (para la marca temporal)
const char* ntpServer = "pool.ntp.org";
const char* tzInfo = "CET-1CEST,M3.5.0,M10.5.0/3";

unsigned long ultimoTiempo = 0;

void setup() {
  Serial.begin(115200);

  // --- CONEXIÓN WIFI ---
  Serial.print("\nConectando al WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡WiFi Conectado!");

  // --- SINCRONIZACIÓN NTP ---
  Serial.print("Sincronizando reloj para la marca temporal");
  configTime(0, 0, ntpServer);
  setenv("TZ", tzInfo, 1);
  tzset();

  time_t now = 0;
  while (time(&now) < 1577836800) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Hora sincronizada con éxito!\n");
}

void loop() {
  // Ejecutar cada 10 segundos (10000 milisegundos)
  if (millis() - ultimoTiempo >= 10000) {
    ultimoTiempo = millis();

    // Generar dato de temperatura inventado 
    float tempInventada = random(1800, 3001) / 100.0;

    // Obtener la marca temporal actual (Epoch Unix en segundos)
    time_t timestamp;
    time(&timestamp);

    // Crear la estructura JSON siguiendo el estándar SenML 
    JsonDocument doc;
    
    // Se crea un objeto 
    JsonObject lectura = doc.add<JsonObject>();
    lectura["n"] = "temperatura";   // Nombre del sensor
    lectura["u"] = "Cel";           // Unidad SenML para Grados Celsius
    lectura["v"] = tempInventada;   // Valor de temperatura
    lectura["t"] = (long)timestamp; // Marca temporal (Epoch)

    // Generar el fichero/texto JSON y mostrarlo por pantalla
    String jsonSalida;
    serializeJsonPretty(doc, jsonSalida);

    Serial.println("--- Fichero JSON SenML generado ---");
    Serial.println(jsonSalida);
    Serial.println("-----------------------------------\n");
  }
}