#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

// Datos red WiFi
const char* ssid = "DIGIFIBRA-SGDT";
const char* password = "3X6x2CfudG";

// Datos del servidor Python (PC)
const char* pc_ip = "192.168.1.131"; // IP actual del PC
const int pc_port = 8080;            // Puerto para python

WiFiClient cliente;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("\nConectando al WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡WiFi Conectado! IP: " + WiFi.localIP().toString());
}

void loop() {
  // Mantener la conexión abierta con el servidor de Python
  if (!cliente.connected()) {
    Serial.println("Buscando servidor Python en " + String(pc_ip) + ":" + String(pc_port) + "...");
    cliente.connect(pc_ip, pc_port);
    delay(1000); // Pausa antes de reintentar si falla
    return;      // Salimos del loop hasta que consiga conectar
  }

  // Generar datos aleatorios medidos por un acelerómetro 
  float accelX = random(-200, 201) / 100.0;
  float accelY = random(-200, 201) / 100.0;
  float accelZ = random(80, 121) / 100.0;  

  // Formatear los datos en una cadena separada por comas 
  String payload = "X:" + String(accelX, 2) + ",Y:" + String(accelY, 2) + ",Z:" + String(accelZ, 2);

  // Enviar los datos por el socket TCP
  cliente.println(payload);
  Serial.println("Enviado: " + payload);

  // Enviar datos cada 500 milisegundos
  delay(500);
}