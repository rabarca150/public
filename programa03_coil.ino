#include <WiFi.h>
#include <ModbusIP_ESP8266.h> // Librería compatible con ESP32

// 1. Datos de red
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

// 2. Registro Modbus (Coil para el LED)
const int LED_COIL = 100; 

// Objeto Modbus
ModbusIP mb;

const int ledPin = 2;
unsigned long ultimoCambio = 0;
bool estadoLed = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // 3. Inicializar Modbus
  mb.config();         // En esta librería se usa .config() para arrancar el servidor
  mb.addCoil(LED_COIL); // Añadimos el registro 100
}

void loop() {
  // 4. Gestión de peticiones Modbus
  mb.task();

  // 5. Lógica de parpadeo (Cada 1 segundo)
  if (millis() - ultimoCambio >= 1000) {
    ultimoCambio = millis();
    estadoLed = !estadoLed;
    digitalWrite(ledPin, estadoLed);

    // 6. Actualizar el registro Modbus
    // El SCADA leerá este valor
    mb.Coil(LED_COIL, estadoLed);
  }
}
