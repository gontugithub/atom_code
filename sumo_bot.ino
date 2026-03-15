// Pines Motores
const int ENA = 3;   // PWM velocidad Motor A
const int IN1 = 4;   // Dirección Motor A
const int IN2 = 5;   // Dirección Motor A

const int ENB = 9;   // PWM velocidad Motor B
const int IN3 = 8;   // Dirección Motor B
const int IN4 = 7;   // Dirección Motor B

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // MOTOR A: Sentido Horario
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 100); // Velocidad media-alta

  // MOTOR B: Sentido Anti-horario (Invertido respecto al A)
  // Para que gire al revés, IN3 debe ser LOW e IN4 debe ser HIGH
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 200); 
}