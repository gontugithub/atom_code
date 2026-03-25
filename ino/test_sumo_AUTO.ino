// --- Pines de Sensores ---
const int IR_IZQ = A3;      // MH-Sensor Izquierdo (Pin DO)
const int TRIG_FRENTE = A1; // Ultrasónico Centro (Pin TRIG)
const int ECHO_FRENTE = A2; // Ultrasónico Centro (Pin ECHO)
const int IR_DER = A0;      // MH-Sensor Derecho (Pin DO)

void setup() {
  Serial.begin(9600);
  
  // Configurar los sensores infrarrojos
  pinMode(IR_IZQ, INPUT);
  pinMode(IR_DER, INPUT);
  
  // Configurar el sensor ultrasónico
  pinMode(TRIG_FRENTE, OUTPUT); 
  pinMode(ECHO_FRENTE, INPUT);

  Serial.println("--- TEST DE SENSORES INICIADO ---");
  Serial.println("Pon la mano delante del ultrasónico y mueve el robot por el borde del ring.");
  delay(2000);
}

void loop() {
  // 1. Leer los infrarrojos de las esquinas
  int valorIzq = digitalRead(IR_IZQ);
  int valorDer = digitalRead(IR_DER);

  // 2. Leer el ultrasónico central
  int distancia = medirDistancia(TRIG_FRENTE, ECHO_FRENTE);

  // --- MOSTRAR RESULTADOS EN PANTALLA ---
  
  // IR Izquierdo
  Serial.print("IR IZQ: ");
  if (valorIzq == LOW) {
    Serial.print("BLANCO (0) | ");
  } else {
    Serial.print("NEGRO (1)  | ");
  }

  // IR Derecho
  Serial.print("IR DER: ");
  if (valorDer == LOW) {
    Serial.print("BLANCO (0) | ");
  } else {
    Serial.print("NEGRO (1)  | ");
  }

  // Ultrasónico
  Serial.print("ULTRASÓNICO: ");
  if (distancia == 999) {
    Serial.println("> 68 cm (Nada cerca)");
  } else {
    Serial.print(distancia);
    Serial.println(" cm");
  }

  // Esperar medio segundo para poder leerlo bien
  delay(500);
}

// --- FUNCIÓN PARA LEER EL ULTRASÓNICO ---
int medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Timeout de 4000 microsegundos (~68 cm max)
  long duracion = pulseIn(echoPin, HIGH, 4000); 
  
  if (duracion == 0) return 999; 
  return duracion * 0.034 / 2;
}