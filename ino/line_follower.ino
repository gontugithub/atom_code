#include <QTRSensors.h>

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// --- Pines de Motores ---
const int ENA = 3; const int IN1 = 4; const int IN2 = 5;
const int ENB = 9; const int IN3 = 8; const int IN4 = 7;

// --- Parámetros de Control ---
// Subimos un pelín el Kp para que el ajuste en las curvas sea más agresivo
float Kp = 0.035; 

// --- Velocidades y Giro ---
const int velocidadBase = 60; 
const int velocidadMax = 100; 
const int limiteReversa = -60; 

// NUEVO: El umbral físico de tus motores. Si el PWM es menor que esto, 
// el motor hace ruido pero no gira. Esto le da el empujón necesario.
const int potenciaMinima = 50; 

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, 11, 10}, SensorCount);
  qtr.setEmitterPin(12);

  delay(500);
  
  // Calibración
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
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

// --- Función Corregida: Control de Zona Muerta ---
void moverMotores(int m1, int m2) {
  
  // Procesar Motor A (Izquierdo/Derecho)
  if (m1 > 0) { // Hacia adelante
    if (m1 < potenciaMinima) m1 = potenciaMinima; // Empujón mínimo
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    analogWrite(ENA, m1);
  } else if (m1 < 0) { // Hacia atrás
    int m1_abs = abs(m1);
    if (m1_abs < potenciaMinima) m1_abs = potenciaMinima; // Empujón mínimo hacia atrás
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENA, m1_abs);
  } else { // Parado exactamente (0)
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }

  // Procesar Motor B (Izquierdo/Derecho)
  if (m2 > 0) { // Hacia adelante
    if (m2 < potenciaMinima) m2 = potenciaMinima; 
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENB, m2);
  } else if (m2 < 0) { // Hacia atrás
    int m2_abs = abs(m2);
    if (m2_abs < potenciaMinima) m2_abs = potenciaMinima;
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENB, m2_abs);
  } else { // Parado
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}