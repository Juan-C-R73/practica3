// Librerías nativas del ESP32 para WiFi y gestión del tiempo
#include <WiFi.h>
#include <time.h>

// Sustituye por los datos de tu WiFi
const char* ssid = "PORTÁTIL 3145";
const char* password = "+18874bP";

// Servidores NTP (Como muestra el esquema de la práctica)
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";

// Cadena POSIX para la Zona Horaria de España (Península)
// CET-1 (UTC+1 en invierno) | CEST (UTC+2 en verano)
// M3.5.0 (Cambio en marzo) | M10.5.0/3 (Cambio en octubre)
const char* tzInfo = "CET-1CEST,M3.5.0,M10.5.0/3";

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando al WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡WiFi Conectado!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());
}

void syncTime() {
  Serial.println("Sincronizando con el servidor NTP...");
  
  // Configuramos los servidores de donde extraeremos la hora
  configTime(0, 0, ntpServer1, ntpServer2);

  // Aplicamos nuestra zona horaria (España)
  setenv("TZ", tzInfo, 1);
  tzset();

  // Esperamos hasta que el ESP32 reciba una fecha lógica 
  // (Mayor al 1 de Enero de 2020)
  time_t now = 0;
  while (time(&now) < 1577836800) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n¡Hora sincronizada con éxito!");
}

void printDateTime() {
  struct tm timeinfo;
  
  // getLocalTime extrae la hora del reloj interno del ESP32
  if (!getLocalTime(&timeinfo, 2000)) {
    Serial.println("Error al obtener la hora local");
    return;
  }
  
  char formattedTime[80];
  // Formateamos la hora: "Día de la semana, Mes Día Año Hora:Min:Seg"
  strftime(formattedTime, sizeof(formattedTime), "%A, %d %B %Y %H:%M:%S", &timeinfo);
  Serial.println(formattedTime);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectWiFi();
  syncTime(); // Sincroniza la hora a través de Internet
}

void loop() {
  printDateTime();
  delay(1000); // Imprime la hora cada segundo
}