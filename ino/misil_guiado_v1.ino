// ======================================================
//  ROBOT SUMO - SEARCH CONE PRO (PRECISION TRACKING)
// ======================================================

const int ENA = 3; const int IN1 = 4; const int IN2 = 5;
const int ENB = 9; const int IN3 = 8; const int IN4 = 7;

const int TRIG_FRENTE = A1;
const int ECHO_FRENTE = A2;

// --- CONFIGURACIÓN AJUSTADA ---
const int DISTANCIA_MAXIMA = 30;  // Rango de visión total
const int VEL_BASE_ATAQUE = 110;  // Velocidad mínima de empuje
const int VEL_MAX_ATAQUE = 180;   // Velocidad máxima de embestida
const int VEL_RADAR = 55;         

// --- SISTEMA DE OPORTUNIDADES ---
int fallosConsecutivos = 0;       
const int MAX_OPORTUNIDADES = 5;  // Un poco más de margen para tracking

// --- VARIABLES DE ALGORITMO DE SEGUIMIENTO ---
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
  delay(5000); 
}

void loop() {
  int distancia = medirDistancia(TRIG_FRENTE, ECHO_FRENTE);

  // --- ESTADO 1: TRACKING ACTIVO (LO ESTAMOS VIENDO) ---
  if (distancia > 0 && distancia < DISTANCIA_MAXIMA) {
    combateIniciado = true;
    ultimaDistanciaVisto = distancia;
    tiempoUltimaVista = millis();
    fallosConsecutivos = 0;

    // MEJORA: Velocidad Proporcional. 
    // Si está lejos (40cm) va a 180. Si está cerca (5cm) va a 110 para tener control.
    int velocidadDinamica = map(distancia, 5, DISTANCIA_MAXIMA, VEL_BASE_ATAQUE, VEL_MAX_ATAQUE);
    moverMotores(velocidadDinamica, velocidadDinamica);
  } 
  
  // --- ESTADO 2: SEGUIMIENTO POR INERCIA (RECIÉN PERDIDO) ---
  else if (fallosConsecutivos < MAX_OPORTUNIDADES && combateIniciado) {
    fallosConsecutivos++;
    
    // MEJORA: Mientras está en "oportunidades", el robot hace un ligero giro 
    // hacia la última dirección donde giró el radar para intentar "re-enganchar"
    int correccion = (direccionBarrido == 1) ? 20 : -20; 
    moverMotores(VEL_BASE_ATAQUE + correccion, VEL_BASE_ATAQUE - correccion);
  } 

  // --- ESTADO 3: BÚSQUEDA DINÁMICA ---
  else {
    // Si han pasado más de 3.5 segundos, reseteamos a búsqueda 360
    if (!combateIniciado || (millis() - tiempoUltimaVista > 3500)) {
      combateIniciado = false;
      moverMotores(VEL_RADAR, -VEL_RADAR);
    } 
    else {
      // BÚSQUEDA PROPORCIONAL MEJORADA
      // Calculamos el ancho del cono según la última distancia
      int duracionGiro = map(ultimaDistanciaVisto, 5, DISTANCIA_MAXIMA, 150, 900);
      duracionGiro = constrain(duracionGiro, 150, 900);

      if (millis() - tiempoBarrido > duracionGiro) {
        direccionBarrido *= -1;      
        tiempoBarrido = millis();    
      }

      // Aplicamos el giro de radar suave
      if (direccionBarrido == 1) {
        moverMotores(VEL_RADAR, -VEL_RADAR); 
      } else {
        moverMotores(-VEL_RADAR, VEL_RADAR); 
      }
    }
  }
}

// --- CONTROL DE MOTORES ---
void moverMotores(int m1, int m2) {
  m1 = constrain(m1, -255, 255);
  m2 = constrain(m2, -255, 255);

  if (m1 > 0) { digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, abs(m1)); } 
  else if (m1 < 0) { digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, abs(m1)); } 
  else { digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0); }

  if (m2 > 0) { digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, abs(m2)); } 
  else if (m2 < 0) { digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, abs(m2)); } 
  else { digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0); }
}

// --- LECTURA ULTRASÓNICO ---
int medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reducimos el timeout para que el código corra más rápido (mejor tracking)
  long duracion = pulseIn(echoPin, HIGH, 12000); 
  if (duracion == 0) return 999; 
  return duracion * 0.034 / 2;
}