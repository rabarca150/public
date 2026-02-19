/*
 * Programa: Blink con Soporte WiFi para ESP32
 * Objetivo: Controlar el LED interno y asegurar la conectividad a red.
 */

#include <WiFi.h> // Librería necesaria para funciones de red

// 1. Configuración de Credenciales
const char* ssid = "NOMBRE_DE_TU_RED";
const char* password = "CLAVE_DE_TU_RED";

// 2. Definición del Pin del LED (GPIO 2 en la mayoría de DevKits)
const int ledPin = 2;

void setup() {
  // Inicializamos comunicación serial a alta velocidad
  Serial.begin(115200);
  delay(10); 

  // Configuramos el pin del LED como salida
  pinMode(ledPin, OUTPUT);

  // 3. Proceso de Conexión WiFi
  Serial.println();
  Serial.print("Intentando conectar a: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Bucle de espera: mientras no esté conectado, imprime un punto
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // 4. Confirmación de Conexión
  Serial.println("");
  Serial.println("¡WiFi Conectado exitosamente!");
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP()); // Muestra la IP para futuros proyectos SCADA
}

void loop() {
  // 5. Lógica de parpadeo (Blink)
  digitalWrite(ledPin, HIGH);   // Enciende el LED
  Serial.println("Estado: LED Encendido");
  delay(1000);                  // Pausa de 1 segundo

  digitalWrite(ledPin, LOW);    // Apaga el LED
  Serial.println("Estado: LED Apagado");
  delay(1000);                  // Pausa de 1 segundo
}
