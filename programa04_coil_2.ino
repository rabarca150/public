#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

// 1. Datos de red
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

// 2. Registro Modbus
// const int LED_COIL = 100; 

// Objeto Modbus
ModbusIP mb;

const int ledPin = 2; // LED integrado

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
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());

  // 3. Inicializar Modbus
  mb.server();         
  mb.addCoil(200); // Registro 100 iniciado en 0 (OFF)
}

void loop() {
  // 4. Gestión de peticiones Modbus
  // Esta función se encarga de recibir los comandos del SCADA
  mb.task();

  // 5. Lógica de Control Remoto
  // El ESP32 lee su propia memoria Modbus para ver si el SCADA escribió algo
  bool estadoScada = mb.Coil(200);
  
  // Aplicamos el estado del registro al LED físico
  digitalWrite(ledPin, estadoScada);

  // Opcional: Monitor serial para depuración
  static bool estadoAnterior = false;
  if (estadoScada != estadoAnterior) {
    Serial.print("Cambio detectado por Modbus. LED: ");
    Serial.println(estadoScada ? "ENCENDIDO" : "APAGADO");
    estadoAnterior = estadoScada;
  }
}
