#include <WiFi.h>
#include <ModbusIP_ESP8266.h>

// --- CONFIGURACIÓN WIFI ---
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

ModbusIP mb;

// ===================================
// 1. DEFINICIÓN DE REGISTROS HREG
// ===================================
// Los Holding Registers (4x) permiten Lectura y Escritura.
const uint16_t HREG_1 = 400; // Registro compartido 1
const uint16_t HREG_2 = 401; // Registro compartido 2
const uint16_t HREG_3 = 402; // Registro compartido 3

void setup() {
  Serial.begin(115200);

  // Conexión a la red local
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n--- SERVIDOR MODBUS TCP LISTO ---");
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());

  // --- 2. INICIALIZACIÓN DEL SERVIDOR ---
  mb.server(); 

  // Añadimos los registros a la memoria del ESP32
  // Inicializamos todos en 0
  mb.addHreg(HREG_1, 0); 
  mb.addHreg(HREG_2, 0);
  mb.addHreg(HREG_3, 0);
}

void loop() {
  // 3. GESTIÓN DE PETICIONES
  // mb.task() permite que múltiples SCADAs lean/escriban simultáneamente
  mb.task();

  // Opcional: Mostrar en el Monitor Serial si alguien cambia un valor
  static uint16_t v1, v2, v3;
  if (mb.Hreg(HREG_1) != v1 || mb.Hreg(HREG_2) != v2 || mb.Hreg(HREG_3) != v3) {
    v1 = mb.Hreg(HREG_1);
    v2 = mb.Hreg(HREG_2);
    v3 = mb.Hreg(HREG_3);
    
    Serial.printf("Cambio detectado -> H400: %d | H401: %d | H402: %d\n", v1, v2, v3);
  }

  delay(10); // Estabilidad del procesador
}
