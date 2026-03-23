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
  delay(2000); 
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void loop() {
  if (bt.available()) {
    String cadena = bt.readStringUntil('\n'); 
    cadena.trim(); // Limpia espacios o saltos de línea invisibles
    
    // --- DETECTOR DE MACROS (ATAQUES ESPECIALES) ---
    if (cadena.startsWith("M")) {
      int macroId = cadena.substring(1).toInt();
      ejecutarMacro(macroId);
    } 
    // --- MOVIMIENTO NORMAL (JOYSTICK) ---
    else {
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
}

// --- FUNCIÓN DE ATAQUES ESPECIALES ---
void ejecutarMacro(int id) {
  if (id == 1) { 
    // MACRO 1: EL TORERO (Esquive derecho y empuje)
    // 1. Gira bruscamente a la derecha para salir de la trayectoria
    moverMotores(255, -255); 
    delay(250); // Ajusta este tiempo para que gire unos 60-90 grados
    
    // 2. Avanza rápido para apartarse
    moverMotores(255, 255);
    delay(400); 
    
    // 3. Gira a la izquierda para mirar a la retaguardia del enemigo
    moverMotores(-255, 255);
    delay(300);
    
    // 4. Embestida a tope aprovechando la inercia
    moverMotores(255, 255);
    delay(800);
    
    // Parar al terminar la macro
    moverMotores(0, 0);
  }
  else if (id == 2) { 
    // MACRO 2: TORNADO (Zafarse de enganches)
    moverMotores(255, -255); // Gira sobre sí mismo a máxima velocidad
    delay(1000); // Durante 1 segundo
    moverMotores(0, 0);
  }
  else if (id == 3) { 
    // MACRO 3: EMBESTIDA RELÁMPAGO (Ataque frontal)
    moverMotores(255, 255); // Todo adelante
    delay(800);
    moverMotores(0, 0);
  }
  else if (id == 4) { 
    // MACRO 4: FRENO DE ANCLA (Frenada de emergencia)
    moverMotores(-255, -255); // Marcha atrás a tope
    delay(300); // Medio segundo suele ser suficiente para clavar el robot
    moverMotores(0, 0);
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