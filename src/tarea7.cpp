#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include <sys/time.h> 

// Datos de red WiFi
const char* ssid = "DIGIFIBRA-SGDT";
const char* password = "3X6x2CfudG";

// 2. Configuración NTP (Hora de España peninsular)
const char* ntpServer = "pool.ntp.org";
const char* tzInfo = "CET-1CEST,M3.5.0,M10.5.0/3";

// Servidor web en el puerto 80 (puerto estándar de los navegadores)
WebServer server(80);

// Mostrar la página principal ("/") 
void mostrarPagina() {
  struct tm timeinfo;
  char horaStr[16] = "00:00:00";
  //Se guarda la hora
  if (getLocalTime(&timeinfo)) {
    strftime(horaStr, sizeof(horaStr), "%H:%M:%S", &timeinfo);
  }

  // Construcción de la página HTML 
  String html = "<!DOCTYPE html><html lang='es'><head>";
  html += "<meta charset='UTF-8'>";
  // Esta etiqueta recarga la página cada 1 segundo para ver avanzar el reloj
  html += "<meta http-equiv='refresh' content='1; url=/'>";
  html += "<title>Reloj ESP32</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin-top: 80px; background-color: #f4f4f9; }";
  html += "h1 { font-size: 64px; color: #222; margin: 20px 0; }";
  html += "button { font-size: 20px; padding: 12px 28px; background-color: #d9534f; color: white; border: none; border-radius: 8px; cursor: pointer; }";
  html += "button:hover { background-color: #c9302c; }";
  html += "</style></head><body>";
  
  html += "<h2>Servidor Web ESP32</h2>";
  html += "<h1>" + String(horaStr) + "</h1>";
  //Para enviar la orden de reset si se toca el botón
  html += "<form action='/reset' method='POST'>";
  html += "<button type='submit'>Resetear hora a 0:00</button>";
  html += "</form>";
  
  html += "</body></html>";
  //Manda el texto en formato html a la web
  server.send(200, "text/html", html);
}

// Poner el reloj a las 0:00 
void resetearHora() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    // Ponemos horas, minutos y segundos a cero manteniendo la fecha actual
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Convertimos la estructura a segundos y actualizamos el reloj interno del ESP32
    time_t nuevoTiempo = mktime(&timeinfo);
    struct timeval tv = { .tv_sec = nuevoTiempo, .tv_usec = 0 };
    settimeofday(&tv, NULL);

    Serial.println("[!] Reloj reseteado a las 00:00:00 desde la web.");
  }

  // Redirigimos el navegador de vuelta a la página principal 
  server.sendHeader("Location", "/");
  server.send(303);
}

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

  // --- SINCRONIZACIÓN NTP INICIAL ---
  Serial.print("Sincronizando hora inicial con Internet");
  configTime(0, 0, ntpServer);
  setenv("TZ", tzInfo, 1);
  tzset();

  time_t now = 0;
  while (time(&now) < 1577836800) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Hora sincronizada!");

  // --- CONFIGURACIÓN DE RUTAS DEL SERVIDOR WEB ---
  server.on("/", HTTP_GET, mostrarPagina);
  server.on("/reset", HTTP_POST, resetearHora);
  server.begin();

  Serial.println("=========================================");
  Serial.print("Servidor Web listo. Abre en tu navegador: http://");
  Serial.println(WiFi.localIP());
  Serial.println("=========================================");
}

void loop() {
  // Mantiene al servidor escuchando peticiones de tu navegador constantemente
  server.handleClient();
}