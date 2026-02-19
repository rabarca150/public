#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

// --- CONFIGURACIÓN WIFI ---
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

ModbusIP mb;

// ===================================
// 1. DEFINICIÓN DE REGISTROS IREG (3x)
// ===================================
// Los Input Registers son de SOLO LECTURA para el SCADA.
const uint16_t IREG_SENSOR_TEMP = 300; 
const uint16_t IREG_SENSOR_VOLT = 301;

unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nServidor Modbus (IREG) Listo.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // --- 2. INICIALIZACIÓN ---
  mb.server(); 

  // Añadimos los Input Registers (300 y 301)
  mb.addIreg(IREG_SENSOR_TEMP); 
  mb.addIreg(IREG_SENSOR_VOLT);
}

void loop() {
  mb.task(); // Atender al SCADA

  // 3. ACTUALIZACIÓN DE DATOS (Cada 2 segundos)
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();

    // Simulamos la lectura de un sensor (Ej. Temperatura de 20.0°C a 30.0°C)
    // Multiplicamos por 10 para enviar un decimal (Modbus no usa flotantes nativos)
    uint16_t lecturaTemp = random(200, 301); 
    uint16_t lecturaVolt = random(210, 230);

    // El ESP32 escribe en su propia memoria para que el SCADA lo lea
    mb.Ireg(IREG_SENSOR_TEMP, lecturaTemp);
    mb.Ireg(IREG_SENSOR_VOLT, lecturaVolt);

    Serial.printf("Enviando a IREG -> Temp: %d | Volt: %d\n", lecturaTemp, lecturaVolt);
  }
}
