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
const uint16_t COIL_0 = 0;  // Controlar el LED del ESP32
const uint16_t COIL_1 = 1;
const uint16_t COIL_2 = 2;
const uint16_t COIL_3 = 3;
const uint16_t COIL_4 = 4;

// B. Discrete Inputs (1x): Lectura de Bit
const uint16_t DIN_0 = 100;  // Estado del botón
const uint16_t DIN_1 = 101;
const uint16_t DIN_2 = 102;
const uint16_t DIN_3 = 103;
const uint16_t DIN_4 = 104;

// C. Input Registers (3x): (solo lectura)
const uint16_t IREG_0 = 300;  // Valor de un sensor (solo lectura)
const uint16_t IREG_1 = 301;
const uint16_t IREG_2 = 302;
const uint16_t IREG_3 = 303;
const uint16_t IREG_4 = 304;

// D. Holding Registers (4x): Lectura/Escritura de Palabra 
const uint16_t HREG_0 = 400;   
const uint16_t HREG_1 = 401;
const uint16_t HREG_2 = 402;
const uint16_t HREG_3 = 403;  
const uint16_t HREG_4 = 404; 

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 1300;  // 1.5 segundo

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

  // ---------------------------------------
  // --- 2. Inicialización y Mapa Modbus ---
  // ---------------------------------------
  mb.server();  // ESP32 como esclavo Modbus TCP

  // A. Coils (0x) - R/W Bit
  mb.addCoil(COIL_0, false);
  mb.addCoil(COIL_1, false);
  mb.addCoil(COIL_2, true);
  mb.addCoil(COIL_3, true);
  mb.addCoil(COIL_4, true);

  // B. Discrete Inputs (1x) - R Bit (Solo Lectura)
  mb.addIsts(DIN_0, true);  // Ists = Input Status
  mb.addIsts(DIN_1, false);
  mb.addIsts(DIN_2, true);  
  mb.addIsts(DIN_3, false);  
  mb.addIsts(DIN_4, true);  

  // C. Input Registers (3x) - R Word (Solo Lectura) Ireg = Input Register
  mb.addIreg(IREG_0, 250);  // Temperatura_1 valor inicial  25.0 °C
  mb.addIreg(IREG_1, 300);  // Temperatura_2 valor inicial 30.0 °C
  mb.addIreg(IREG_2, 220);  // Voltaje - valor inicial 220 VAC
  mb.addIreg(IREG_3, 100);  // Corriente
  mb.addIreg(IREG_4, 220*100);    // Potencia

  // D. Holding Registers (4x) - R/W Word
  mb.addHreg(HREG_0, 1000); // Temperatura alarma 01 - Set point = 1000 °C
  mb.addHreg(HREG_1, 1000); // Temperatura alarma 02 - Set point = 1000 °C
  mb.addHreg(HREG_2, 0);    // Voltaje alarma 01
  mb.addHreg(HREG_3, 0);    // Corriente alarma 01
  mb.addHreg(HREG_4, 0);    // Potencia alarma 01
}

void loop() {
  // Tu código de aplicación (lectura de temperatura, publicación MQTT, etc.)
  // Ya puedes asumir que la conexión Wi-Fi está activa.
  mb.task();  // Procesa peticiones Modbus TCP

  bool state = mb.Coil(COIL_0);
  digitalWrite(LED_PIN, state ? HIGH : LOW);

  unsigned long now = millis();
  // Simula temperatura cada 1s
  if (now - lastUpdate >= updateInterval) {
    lastUpdate = now;

    int tempeture_0 = random(250, 260);
    int variability = random(1,5);
    int current = random (50,600);
    if (state == true) {
      mb.Ireg(IREG_0, tempeture_0 * variability);
      mb.Ireg(IREG_3, current * variability);
    } else {
      mb.Ireg(IREG_0, tempeture_0);
      mb.Ireg(IREG_3, current);
    }

    int tempeture_1 = random(700, 800);
    int volt = random(216,224);
    mb.Ireg(IREG_1, tempeture_1 );
    mb.Ireg(IREG_2, volt);
    mb.Ireg(IREG_4, current*volt/1000);
  }

}
