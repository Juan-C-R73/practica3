# Práctica 3 

Este repositorio contiene la resolución de las tareas de la Práctica 3
## 🚀 Tareas implementadas

*   **Tarea 1:** Conexión a la red wifi del laboratorio o a una creada por el móvil como punto de acceso, extracción de la IP y comprobación de la conectividad con Google mediante un ping.
*   **Tarea 2:** Puesta en hora del módulo mediante un servidor NTP (Network Time Protocol).
*   **Tarea 4:** Configuración del módulo hardware como cliente para enviar cada segundo la hora local.
*   **Tarea 5:** Adición de una capa de control para que, al mandar el comando "start", el módulo empiece a mandar la hora hasta que se le mande el comando "stop".
*   **Tarea 6:** Adaptación del código del ESP32 para que se conecte a la wifi y a un servidor TCP para mandar los datos del acelerómetro. Además, se modifica el código en Python para recibir los datos a través de un servidor TCP en lugar de usar el puerto serie.
*   **Tarea 7:** Montaje de un servidor WEB que muestra la hora actual y tiene un botón para resetear la hora a las 0:00.
*   **Tarea 8:** Creación de un fichero JSON basado en el estándar SenML que se genera cada 10 segundos, conteniendo datos de temperatura inventados con sus unidades correspondientes y la marca temporal.
*   **Tarea 9:** Subida de los archivos a un servidor FTP del laboratorio con el formato de nombre `grupoXX_ddmmss.json`.
*   **Tarea 10:** Subida de datos a la nube utilizando el servicio gratuito de Adafruit. Se hace uso de la librería de Adafruit IO para subir datos al feed usando el protocolo MQTT y para suscribirse al feed, comprobando así que se reciben actualizaciones al escribir desde el navegador.

## 🛠️ Tecnologías y Librerías Utilizadas

*   **Plataforma:** ESP32.
*   **Protocolos de red:** ICMP (Ping), NTP, Sockets TCP/IP, HTTP (Servidor Web), FTP y MQTT.
*   **Estandarización de datos:** JSON (formato SenML).
*   **Plataformas Cloud:** Adafruit IO.



