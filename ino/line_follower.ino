#include <QTRSensors.h>

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// --- Pines de Motores ---
const int ENA = 3; const int IN1 = 4; const int IN2 = 5;
const int ENB = 9; const int IN3 = 8; const int IN4 = 7;

// --- NUEVO: Pin del Botón ---
const int BOTON_PIN = 6; 

// --- Parámetros de Control ---
float Kp = 0.035; 

// --- Velocidades y Giro ---
const int velocidadBase = 60; 
const int velocidadMax = 100; 
const int limiteReversa = -60; 
const int potenciaMinima = 50; 

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Configurar el botón con la resistencia interna del Nano
  pinMode(BOTON_PIN, INPUT_PULLUP);

  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, 11, 10}, SensorCount);
  qtr.setEmitterPin(12);

  delay(500);
  
  // 1. Calibración (Mueve el robot sobre la línea)
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
  
  // 2. NUEVO: Stand-by (Espera a que presiones el botón)
  // Se queda aquí hasta que el pin D6 se conecte a GND mediante el botón
  while (digitalRead(BOTON_PIN) == HIGH) {
    delay(10);
  }
  
  // Pequeña pausa para poder soltar el robot antes de que arranque
  delay(1000);
}

void loop() {
  uint16_t position = qtr.readLineBlack(sensorValues);

  int error = 3500 - position;
  int ajuste = error * Kp;

  int velocidadMotorA = velocidadBase + ajuste;
  int velocidadMotorB = velocidadBase - ajuste;

  velocidadMotorA = constrain(velocidadMotorA, limiteReversa, velocidadMax);
  velocidadMotorB = constrain(velocidadMotorB, limiteReversa, velocidadMax);

  moverMotores(velocidadMotorA, velocidadMotorB);
}

// --- Función de Control de Motores ---
void moverMotores(int m1, int m2) {
  
  // Procesar Motor A
  if (m1 > 0) { 
    if (m1 < potenciaMinima) m1 = potenciaMinima; 
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    analogWrite(ENA, m1);
  } else if (m1 < 0) { // Corregido de < 20 a < 0 para detectar reversa real
    int m1_abs = abs(m1);
    if (m1_abs < potenciaMinima) m1_abs = potenciaMinima; 
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENA, m1_abs);
  } else { 
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }

  // Procesar Motor B
  if (m2 > 0) { 
    if (m2 < potenciaMinima) m2 = potenciaMinima; 
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENB, m2);
  } else if (m2 < 0) { // Corregido de < 20 a < 0
    int m2_abs = abs(m2);
    if (m2_abs < potenciaMinima) m2_abs = potenciaMinima;
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENB, m2_abs);
  } else { 
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}