/*
 * Programa: Blink ESP32
 * Descripción: Enciende y apaga el LED interno del DevKit cada segundo.
 */

// 1. Definición del Pin
// La mayoría de los ESP32 DevKit tienen el LED en el GPIO 2
const int ledPin = 2; 

void setup() {
  // 2. Configuración del Pin
  // Definimos el pin como SALIDA (OUTPUT) para poder enviar voltaje
  pinMode(ledPin, OUTPUT);
  
  // Iniciamos comunicación serial para monitorear el estado
  Serial.begin(115200);
  Serial.println("Sistema iniciado. El LED comenzará a parpadear.");
}

void loop() {
  // 3. Encender el LED
  digitalWrite(ledPin, HIGH);   // HIGH pone el pin en 3.3V
  Serial.println("LED: Encendido");
  delay(1000);                  // Espera 1000 milisegundos (1 segundo)

  // 4. Apagar el LED
  digitalWrite(ledPin, LOW);    // LOW pone el pin en 0V (GND)
  Serial.println("LED: Apagado");
  delay(1000);                  // Espera otro segundo
}
