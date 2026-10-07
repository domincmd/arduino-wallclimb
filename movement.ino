// Motor esquerdo
const int LEFT_IN1 = 2;
const int LEFT_IN2 = 3;

// Motor direito
const int RIGHT_IN1 = 4;
const int RIGHT_IN2 = 5;

// Joystick
const int JOY_X = A1;
const int JOY_Y = A0;

// Zona morta do joystick
const int DEAD_ZONE_MIN = 400;
const int DEAD_ZONE_MAX = 600;

int left = 0;
int right = 0;

// left/right:
//  1 = frente
//  0 = parado
// -1 = ré

void getMovementDir(int analogX, int analogY) {

  // Primeiro, define o movimento baseado no Y
  // Y para cima -> frente
  // Y para baixo -> ré
  // Centro -> parado

  if (analogY < DEAD_ZONE_MIN) {
    left = -1;
    right = -1;
  }
  else if (analogY > DEAD_ZONE_MAX) {
    left = 1;
    right = 1;
  }
  else {
    left = 0;
    right = 0;
  }

  // Depois aplica a direção do X
  // X para esquerda -> vira para esquerda
  // X para direita -> vira para direita

  if (analogX < DEAD_ZONE_MIN) {
    left = -1;
    right = 1;
  }
  else if (analogX > DEAD_ZONE_MAX) {
    left = 1;
    right = -1;
  }
}

void setMotor(int in1, int in2, int direction) {

  if (direction == 1) {
    // Frente
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if (direction == -1) {
    // Ré
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else {
    // Parado
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

void setup() {

  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);

  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  getMovementDir(x, y);

  // Controla os dois motores
  setMotor(LEFT_IN1, LEFT_IN2, left);
  setMotor(RIGHT_IN1, RIGHT_IN2, right);

  // Monitor serial
  Serial.print("X: ");
  Serial.print(x);

  Serial.print(" | Y: ");
  Serial.print(y);

  Serial.print(" | Left: ");
  Serial.print(left);

  Serial.print(" | Right: ");
  Serial.println(right);

  delay(20);
}
