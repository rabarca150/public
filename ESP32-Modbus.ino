#include <WiFi.h>
#include <WiFiManager.h>
// La librería WiFi.h se incluye automáticamente con WiFiManager
#include <ModbusIP_ESP8266.h>  // modbus-esp compatible con ESP32

ModbusIP mb;

const int LED_PIN = 2;
const uint16_t COIL_LED = 100;   // Dirección coil
const uint16_t HREG_TEMP = 300;  // Holding register en 300
const uint16_t HREG_301 = 301;   // Holding register en 301

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1000;  // 1.5 segundo

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  delay(3000);
  Serial.println("\nIniciando WiFiManager...");

  // Crea una instancia de WiFiManager
  WiFiManager wm;

  wm.resetSettings();  // Descomentar para borrar credenciales y forzar el portal en cada arranque

  // Inicia la conexión o el portal de configuración
  // Si no se conecta, crea el AP llamado "Config_ESP32"
  bool res = wm.autoConnect("ESP32-Modbus");
  if (!res) {
    Serial.println("❌ No se pudo conectar y no se configuró WiFi");
    while (true)
      ;  // Detener ejecución
  } else {
    // Éxito: El ESP32 ya está conectado a la red
    Serial.print("Conectado a la red. IP: ");
    Serial.println(WiFi.localIP());

    mb.server();                 // ESP32 como esclavo Modbus TCP
    mb.addCoil(COIL_LED);        // Añade coil en dirección 100
    mb.addHreg(HREG_TEMP, 250);  // Registro de temperatura (25.0 °C inicial)
    mb.addHreg(HREG_301, 1000);
  }
}

void loop() {
  // Tu código de aplicación (lectura de temperatura, publicación MQTT, etc.)
  // Ya puedes asumir que la conexión Wi-Fi está activa.
  mb.task();  // Procesa peticiones Modbus TCP

  bool state = mb.Coil(COIL_LED);
  digitalWrite(LED_PIN, state ? HIGH : LOW);

  unsigned long now = millis();
  if (now - lastUpdate >= updateInterval) {
    lastUpdate = now;

    // Simula temperatura cada 1s
    int temp = random(320, 330);
    if (state == true) {
      mb.Hreg(HREG_TEMP, temp * 0.5);
    } else {
      mb.Hreg(HREG_TEMP, temp);
    }

    int temp1 = random(1000, 1500);
    mb.Hreg(HREG_301, temp1);
  }
  delay(100);
}
