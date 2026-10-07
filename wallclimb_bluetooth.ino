#include <SoftwareSerial.h>

// ==========================================
// WALLCLIMB - Controle por Bluetooth (HC-06)
// ==========================================

// Bluetooth HC-06
// (pinos 10 e 11 para não conflitar com os pinos da ponte H)
const int BT_RX = 10;  // Arduino RX <- TX do HC-06
const int BT_TX = 11;  // Arduino TX -> RX do HC-06 (usar divisor de tensão!)

SoftwareSerial hc06(BT_RX, BT_TX);

// Motor esquerdo
const int LEFT_IN1 = 2;
const int LEFT_IN2 = 3;

// Motor direito
const int RIGHT_IN1 = 4;
const int RIGHT_IN2 = 5;

// Segurança: se ficar este tempo (ms) sem receber nenhum comando,
// os motores param (evita o robô sair andando se a conexão cair)
const unsigned long COMMAND_TIMEOUT = 500;

unsigned long lastCommandTime = 0;

// left/right:
//  1 = frente
//  0 = parado
// -1 = ré
int left = 0;
int right = 0;

// Comandos aceitos (enviados como 1 caractere):
//  F = frente
//  B = ré
//  L = vira à esquerda
//  R = vira à direita
//  S = parar
void getMovementDir(char cmd) {

  switch (cmd) {

    case 'F':
      left = 1;
      right = 1;
      break;

    case 'B':
      left = -1;
      right = -1;
      break;

    case 'L':
      left = -1;
      right = 1;
      break;

    case 'R':
      left = 1;
      right = -1;
      break;

    case 'S':
      left = 0;
      right = 0;
      break;

    default:
      // Comando desconhecido (inclui \r, \n, espaços): ignora
      return;
  }

  lastCommandTime = millis();

  // Monitor serial
  Serial.print("Comando: ");
  Serial.print(cmd);
  Serial.print(" | Left: ");
  Serial.print(left);
  Serial.print(" | Right: ");
  Serial.println(right);
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

  // Começa com os motores parados
  setMotor(LEFT_IN1, LEFT_IN2, 0);
  setMotor(RIGHT_IN1, RIGHT_IN2, 0);

  Serial.begin(9600);
  hc06.begin(9600);

  lastCommandTime = millis();

  Serial.println("==============================");
  Serial.println("   WALLCLIMB - Bluetooth");
  Serial.println("==============================");
  Serial.println("Comandos: F, B, L, R, S");
  Serial.println();
}

void loop() {

  // ==========================================
  // Bluetooth -> define direção dos motores
  // ==========================================
  while (hc06.available()) {

    char c = hc06.read();

    // Aceita letras minúsculas também
    if (c >= 'a' && c <= 'z') {
      c = c - 32;
    }

    getMovementDir(c);
  }

  // ==========================================
  // Segurança: sem comando por muito tempo -> para
  // ==========================================
  if ((left != 0 || right != 0) && millis() - lastCommandTime > COMMAND_TIMEOUT) {
    left = 0;
    right = 0;
    Serial.println("Timeout: motores parados");
  }

  // Controla os dois motores
  setMotor(LEFT_IN1, LEFT_IN2, left);
  setMotor(RIGHT_IN1, RIGHT_IN2, right);
}
