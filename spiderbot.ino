#include <SoftwareSerial.h>

SoftwareSerial hc06(2, 3);  // RX, TX

String comando = "";

void setup() {

  Serial.begin(9600);
  hc06.begin(9600);

  delay(500);

  Serial.println();
  Serial.println("==============================");
  Serial.println("       TERMINAL HC-06");
  Serial.println("==============================");
  Serial.println("HC-06: 9600 baud");
  Serial.println("Digite AT e pressione ENTER.");
  Serial.println();
}

void loop() {

  // ==========================================
  // Monitor Serial -> Arduino
  // ==========================================

  while (Serial.available()) {

    char c = Serial.read();

    // ENTER = envia o comando
    if (c == '\n' || c == '\r') {

      if (comando.length() > 0) {

        Serial.print("Enviando: [");
        Serial.print(comando);
        Serial.println("]");

        // IMPORTANTE:
        // envia o comando inteiro de uma vez
        hc06.print(comando);

        comando = "";

        Serial.println("Aguardando resposta...");
      }
    }

    // Qualquer outro caractere
    else {

      comando += c;
    }
  }


  // ==========================================
  // HC-06 -> Monitor Serial
  // ==========================================

  while (hc06.available()) {

    char c = hc06.read();

    Serial.write(c);
  }
}
