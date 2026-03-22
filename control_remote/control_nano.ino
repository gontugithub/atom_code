#include <SoftwareSerial.h>

// RX del Arduino en 2 (conectado a TX del BT)
// TX del Arduino en 13 (conectado a RX del BT)
SoftwareSerial bt(2, 13); 

// --- Pines de Motores ---
const int ENA = 3; const int IN1 = 4; const int IN2 = 5;
const int ENB = 9; const int IN3 = 8; const int IN4 = 7;

const int potenciaMinima = 60; // Fuerza mínima

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  Serial.println("--- CEREBRO DE MOTORES LISTO ---");
  Serial.println("--- TEST DE MOTORES DE 2 SEGUNDOS ---");
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENB, 150);
  delay(2000); // Deben girar 2 segundos
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void loop() {
  if (bt.available()) {
    String cadena = bt.readStringUntil('\n'); 
    
    int posX = cadena.indexOf('X');
    int posY = cadena.indexOf('Y');
    int coma = cadena.indexOf(',');
    
    if (posX != -1 && posY != -1 && coma != -1) {
      int x = cadena.substring(posX + 1, coma).toInt();
      int y = cadena.substring(posY + 1).toInt();
      
      int m1 = y + x; // Motor Izquierdo (A)
      int m2 = y - x; // Motor Derecho (B)

      m1 = map(m1, -100, 100, -255, 255);
      m2 = map(m2, -100, 100, -255, 255);
      
      m1 = constrain(m1, -255, 255);
      m2 = constrain(m2, -255, 255);

      moverMotores(m1, m2);
    }
  }
}

void moverMotores(int m1, int m2) {
  // MOTOR A (Izquierdo)
  if (m1 > 10) { 
    if (m1 < potenciaMinima) m1 = potenciaMinima;
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    analogWrite(ENA, m1);
  } else if (m1 < -10) { 
    int m1_abs = abs(m1);
    if (m1_abs < potenciaMinima) m1_abs = potenciaMinima;
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENA, m1_abs);
  } else { 
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }

  // MOTOR B (Derecho)
  if (m2 > 10) { 
    if (m2 < potenciaMinima) m2 = potenciaMinima; 
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENB, m2);
  } else if (m2 < -10) { 
    int m2_abs = abs(m2);
    if (m2_abs < potenciaMinima) m2_abs = potenciaMinima;
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENB, m2_abs);
  } else { 
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}