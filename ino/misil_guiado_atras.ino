// ======================================================
//  ROBOT SUMO - SEARCH CONE PRO + RETROCESO EN LÍNEA NEGRA
// ======================================================

const int ENA = 3; const int IN1 = 4; const int IN2 = 5;
const int ENB = 9; const int IN3 = 8; const int IN4 = 7;

const int TRIG_FRENTE = A1;
const int ECHO_FRENTE = A2;

const int IR_IZQ = A3;
const int IR_DER = A0;

// --- CONFIGURACIÓN ---
const int DISTANCIA_MAXIMA = 30;  
const int VEL_BASE_ATAQUE = 110;  
const int VEL_MAX_ATAQUE = 180;   
const int VEL_RADAR = 55;         

// --- AJUSTE DE COLOR ---
const int NEGRO = HIGH;  // Cambiado a HIGH para detectar la línea negra

// --- SISTEMA DE OPORTUNIDADES ---
int fallosConsecutivos = 0;       
const int MAX_OPORTUNIDADES = 5;  

// --- VARIABLES DE SEGUIMIENTO ---
bool combateIniciado = false;        
int ultimaDistanciaVisto = 0;        
unsigned long tiempoUltimaVista = 0; 
unsigned long tiempoBarrido = 0;     
int direccionBarrido = 1;            

void setup() {
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG_FRENTE, OUTPUT); 
  pinMode(ECHO_FRENTE, INPUT);
  pinMode(IR_IZQ, INPUT);
  pinMode(IR_DER, INPUT);
  
  delay(5000); 
}

void loop() {
  int lineaIzq = digitalRead(IR_IZQ);
  int lineaDer = digitalRead(IR_DER);
  int distancia = medirDistancia(TRIG_FRENTE, ECHO_FRENTE);

  // --- PRIORIDAD 1: PROTECCIÓN DE LÍNEA NEGRA ---
  if (lineaIzq == NEGRO || lineaDer == NEGRO) {
    // Resetear memoria de combate para que al volver a buscar empiece de cero
    combateIniciado = false;
    fallosConsecutivos = 0;

    // "Meter una vez hacia atrás" - Retroceso seco y fuerte
    moverMotores(-220, -220); 
    delay(600);               // Tiempo de retroceso (aprox. 15-20cm según peso)
    
    moverMotores(0, 0);
    delay(200); // Pausa para no marear al sensor ultrasónico con el tirón
  }

  // --- PRIORIDAD 2: TRACKING ACTIVO (DETECTADO) ---
  else if (distancia > 0 && distancia < DISTANCIA_MAXIMA) {
    combateIniciado = true;
    ultimaDistanciaVisto = distancia;
    tiempoUltimaVista = millis();
    fallosConsecutivos = 0;

    int velocidadDinamica = map(distancia, 5, DISTANCIA_MAXIMA, VEL_BASE_ATAQUE, VEL_MAX_ATAQUE);
    moverMotores(velocidadDinamica, velocidadDinamica);
  } 
  
  // --- PRIORIDAD 3: SEGUIMIENTO POR INERCIA (RECIÉN PERDIDO) ---
  else if (fallosConsecutivos < MAX_OPORTUNIDADES && combateIniciado) {
    fallosConsecutivos++;
    // Pequeña corrección de ángulo mientras embiste "a ciegas"
    int correccion = (direccionBarrido == 1) ? 20 : -20; 
    moverMotores(VEL_BASE_ATAQUE + correccion, VEL_BASE_ATAQUE - correccion);
  } 

  // --- PRIORIDAD 4: BÚSQUEDA DINÁMICA ---
  else {
    // Si han pasado más de 3.5 segundos o nunca lo vimos, giro 360
    if (!combateIniciado || (millis() - tiempoUltimaVista > 3500)) {
      combateIniciado = false;
      moverMotores(VEL_RADAR, -VEL_RADAR); 
    } 
    else {
      // Búsqueda en abanico proporcional a la última distancia
      int duracionGiro = map(ultimaDistanciaVisto, 5, DISTANCIA_MAXIMA, 150, 900);
      duracionGiro = constrain(duracionGiro, 150, 900);

      if (millis() - tiempoBarrido > duracionGiro) {
        direccionBarrido *= -1;      
        tiempoBarrido = millis();    
      }

      if (direccionBarrido == 1) {
        moverMotores(VEL_RADAR, -VEL_RADAR); 
      } else {
        moverMotores(-VEL_RADAR, VEL_RADAR); 
      }
    }
  }
}

// --- FUNCIÓN DE CONTROL DE MOTORES ---
void moverMotores(int m1, int m2) {
  m1 = constrain(m1, -255, 255);
  m2 = constrain(m2, -255, 255);

  // Motor A
  if (m1 > 0) { digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, abs(m1)); } 
  else if (m1 < 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, abs(m1)); } 
  else { digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0); }

  // Motor B
  if (m2 > 0) { digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, abs(m2)); } 
  else if (m2 < 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, abs(m2)); } 
  else { digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0); }
}

// --- FUNCIÓN LECTURA ULTRASÓNICO ---
int medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duracion = pulseIn(echoPin, HIGH, 12000); 
  if (duracion == 0) return 999; 
  return duracion * 0.034 / 2;
}