#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

// 1. Datos de tu red WiFi
const char* ssid = "DIGIFIBRA-SGDT";
const char* password = "3X6x2CfudG";

// 2. Datos del servidor (Tu PC con SocketTest)
const char* pc_ip = "192.168.1.131"; // IP del PC
const int pc_port = 455;            // El puerto que configuraste en SocketTest

// 3. Configuración NTP (Hora de España peninsular)
const char* ntpServer = "pool.ntp.org";
const char* tzInfo = "CET-1CEST,M3.5.0,M10.5.0/3"; 

WiFiClient cliente;

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
  Serial.print("Sincronizando hora con Internet");
  configTime(0, 0, ntpServer);
  setenv("TZ", tzInfo, 1);
  tzset();
  
  time_t now = 0;
  while (time(&now) < 1577836800) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Hora sincronizada con éxito!");
}

void loop() {
  // 1. Extraer la hora interna del ESP32
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al leer la hora");
    return;
  }
  
  // 2. Darle formato de texto legible
  char horaFormateada[80];
  strftime(horaFormateada, sizeof(horaFormateada), "Hora ESP32: %H:%M:%S", &timeinfo);

  // 3. Comprobamos si la puerta ya está abierta. Si no, conectamos.
  if (!cliente.connected()) {
    cliente.connect(pc_ip, pc_port);
  }

  // 4. Si el canal está abierto y funcionando, enviamos el dato
  if (cliente.connected()) {
    cliente.println(horaFormateada);
    Serial.println("Enviado al PC: " + String(horaFormateada));
  } else {
    Serial.println("Fallo de conexión. ¿Está SocketTest encendido?");
  }

  // 4. Cumplir con el requisito: esperar 1 segundo exacto
  delay(1000);
}