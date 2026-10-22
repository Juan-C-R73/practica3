#include <Arduino.h>
#include <EspMQTTClient.h>

// 1. Configuración del Cliente MQTT
EspMQTTClient client(
  "iPhone de Daniel",           // SSID del WiFi
  "12763020",               // Contraseña del WiFi
  "io.adafruit.com",          // Servidor MQTT de Adafruit
  "JuanCR",      // Nombre de usuario en Adafruit (en minúsculas)
  "aio_ekJm80kfUE7JGsp1lDjZpV2J1ids",     // AIO Key
  "ESP32_Cliente_1",          // Nombre para identificar al dispositivo (cualquiera vale)
  1883                        // Puerto estándar MQTT
);

// Variables para el envío periódico
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  // Esta línea le indica a la librería que imprima cada paso por pantalla
  client.enableDebuggingMessages();
}

// Esta función es OBLIGATORIA al usar EspMQTTClient. 
// Se ejecuta automáticamente en el momento en que el ESP32 logra conectar al servidor MQTT.
void onConnectionEstablished() {
  Serial.println("\n--- ¡Conectado al servidor MQTT de Adafruit! ---");

  //Suscribirse al feed para ESCUCHAR si el dato cambia
  // La ruta del feed siempre es: tu_usuario/feeds/nombre_del_feed
  String topicSuscripcion = "JuanCR/feeds/temperatura";
  
  client.subscribe(topicSuscripcion, [](const String & payload) {
    Serial.println("\n[!] AVISO: Alguien ha modificado el valor desde la nube.");
    Serial.println("-> Nuevo valor recibido: " + payload);
  });
}

void loop() {
  // Hay que llamar a esta función en cada vuelta para mantener la conexión viva y recibir mensajes
  client.loop();

  // Publicar datos cada 15 segundos
  if (client.isConnected() && (millis() - ultimoEnvio >= 15000)) {
    ultimoEnvio = millis();

    // Generamos una temperatura inventada
    float temp = random(1800, 3001) / 100.0;
    String datoStr = String(temp, 2);

    // Publicamos en el mismo canal
    String topicPublicacion = "JuanCR/feeds/temperatura";
    
    Serial.println("Enviando a la nube (MQTT): " + datoStr + " °C");
    client.publish(topicPublicacion, datoStr);
  }
}