#include <WiFi.h>
#include <WiFiManager.h>
// La librería WiFi.h se incluye automáticamente con WiFiManager
#include <ModbusIP_ESP8266.h>  // modbus-esp compatible con ESP32

ModbusIP mb;

const int LED_PIN = 2;     // Pin GPIO para el LED
const int BUTTON_PIN = 4;  // Pin GPIO para el botón (ej. GPIO4)

// ===================================
// 1. DEFINICIÓN DE REGISTROS MODBUS
// ===================================

// A. Coils (0x): Lectura/Escritura de Bit
const uint16_t COIL_LED = 1;  // Controlar el LED del ESP32

// B. Discrete Inputs (1x): Lectura de Bit (Estado de un Botón)
const uint16_t DIN_BUTTON = 100;  // Estado del botón

// C. Input Registers (3x): Lectura de Palabra (Sensor simulado)
const uint16_t IREG_SENSOR = 300;  // Valor de un sensor (solo lectura)

// D. Holding Registers (4x): Lectura/Escritura de Palabra (Temperatura y Setpoint)
const uint16_t HREG_TEMP = 400;      // Temperatura (escrita por el esclavo, leída por el maestro)
const uint16_t HREG_SETPOINT = 401;  // Setpoint (escrita por el maestro, leída por el esclavo)

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1000;  // 1.5 segundo

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  delay(3000);
  Serial.println("\nIniciando WiFiManager...");

  // --- 1. Conexión WiFi con Portal Cautivo ---
  WiFiManager wm;
  wm.resetSettings();  // Descomentar para borrar credenciales y forzar el portal en cada arranque

  // Inicia la conexión o el portal de configuración
  // Si no se conecta, crea el AP llamado "ESP32-Modbus"
  bool res = wm.autoConnect("ESP32-Modbus");
  if (!res) {
    Serial.println("❌ No se pudo conectar y no se configuró WiFi");
    while (true)
      ;  // Detener ejecución
  }

  // Éxito: El ESP32 ya está conectado a la red
  Serial.print("Conectado a la red. IP: ");
  Serial.println(WiFi.localIP());

  // --- 2. Inicialización y Mapa Modbus ---
  mb.server();  // ESP32 como esclavo Modbus TCP

  // A. Coils (0x) - R/W Bit
  mb.addCoil(COIL_LED, false);

  // B. Discrete Inputs (1x) - R Bit (Solo Lectura)
  mb.addIsts(DIN_BUTTON, true);  // Ists = Input Status

  // C. Input Registers (3x) - R Word (Solo Lectura)
  mb.addIreg(IREG_SENSOR, 0);  // Ireg = Input Register

  // D. Holding Registers (4x) - R/W Word
  mb.addHreg(HREG_TEMP, 250);      // 25.0 °C
  mb.addHreg(HREG_SETPOINT, 300);  // Setpoint inicial 30.0 °C
}

void loop() {
  // Tu código de aplicación (lectura de temperatura, publicación MQTT, etc.)
  // Ya puedes asumir que la conexión Wi-Fi está activa.
  mb.task();  // Procesa peticiones Modbus TCP

  bool state = mb.Coil(COIL_LED);
  digitalWrite(LED_PIN, state ? HIGH : LOW);

  unsigned long now = millis();
  // Simula temperatura cada 1s
  if (now - lastUpdate >= updateInterval) {
    lastUpdate = now;

    int temp = random(320, 330);
    if (state == true) {
      mb.Hreg(HREG_TEMP, temp * 0.5);
    } else {
      mb.Hreg(HREG_TEMP, temp);
    }

    int temp1 = random(1000, 1500);
    mb.Hreg(HREG_SETPOINT, temp1);
  }
  delay(100);
}
