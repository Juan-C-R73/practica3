#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <time.h>
#include <ArduinoJson.h>
#include <ESP32_FTPClient.h> // Librería para el cliente FTP

// Datos de red WiFi
const char* ssid = "DIGIFIBRA-SGDT";
const char* password = "3X6x2CfudG";

// Datos del servidor FTP del laboratorio
char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";


const char* NUMERO_GRUPO = "01";

// Instancia del cliente FTP (servidor, usuario, contraseña, timeout 5000ms, modo log 2)
ESP32_FTPClient ftp(ftp_server, ftp_user, ftp_pass, 5000, 2);

// Configuración NTP
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
  Serial.print("Sincronizando reloj por NTP");
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
  // Ejecutar cada 10 segundos (10000 ms)
  if (millis() - ultimoTiempo >= 10000) {
    ultimoTiempo = millis();

    // Obtener la fecha/hora actual
    time_t timestamp;
    time(&timestamp);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Error al obtener la hora local.");
      return;
    }

    // Crear el nombre del archivo: grupoXX_ddmmss.json (día, minuto, segundo)
    char nombreArchivo[40];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "grupo%s_%02d%02d%02d.json",
             NUMERO_GRUPO,
             timeinfo.tm_mday, // dd: Día del mes (01-31)
             timeinfo.tm_min,  // mm: Minutos (00-59)
             timeinfo.tm_sec); // ss: Segundos (00-59)

    // Generar dato de temperatura inventado y crear el JSON SenML
    float tempInventada = random(1800, 3001) / 100.0;

    JsonDocument doc;
    JsonObject lectura = doc.add<JsonObject>();
    lectura["n"] = "temperatura";
    lectura["u"] = "Cel";
    lectura["v"] = serialized(String(tempInventada, 2));
    lectura["t"] = (long)timestamp;

    String jsonSalida;
    serializeJsonPretty(doc, jsonSalida);

    Serial.print("Generado archivo: ");
    Serial.println(nombreArchivo);
    Serial.println(jsonSalida);

    // Subir el fichero al servidor FTP del laboratorio
    Serial.println("Conectando al servidor FTP y subiendo archivo...");
    ftp.OpenConnection();
    
    // Modo de transferencia ASCII ("Type A") para archivos de texto/JSON
    ftp.InitFile("Type A");
    ftp.NewFile(nombreArchivo);
    ftp.Write(jsonSalida.c_str());
    ftp.CloseFile();
    
    ftp.CloseConnection();
    Serial.println("¡Archivo subido al FTP con éxito!\n");
  }
}