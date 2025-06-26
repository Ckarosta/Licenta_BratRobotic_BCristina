#include <Servo.h>

#define PIN_SERVO1 3   // Umar
#define PIN_SERVO2 5   // Cot
#define PIN_SERVO3 6   // Incheietura
#define PIN_SERVO4 9 

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  Serial.begin(9600);
  while (!Serial) 

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);
  servo4.attach(PIN_SERVO4);  

  Serial.println(F("Calibrare manuala brat robotic"));
  Serial.println(F("Format comanda: <servo> <unghi>"));
  Serial.println(F("Exemplu: 2 120  -> muta servo2 la 120°"));
  Serial.println();
}


void loop() {
  if (Serial.available() >= 2) {
    int s = Serial.parseInt();    // numarul servo-ului (1, 2 sau 3)
    int angle = Serial.parseInt(); // unghi in grade (0–180)
    Serial.readStringUntil('\n');  

    if ((s >= 1 && s <= 4) && (angle >= 0 && angle <= 180)) {
      switch (s) {
        case 1: servo1.write(angle); break;
        case 2: servo2.write(angle); break;
        case 3: servo3.write(angle); break;
        case 4: servo4.write(angle); break;
      }
      Serial.print(F("Servo "));
      Serial.print(s);
      Serial.print(F(" -> unghi setat: "));
      Serial.print(angle);
      Serial.println(F("°"));
    } else {
      Serial.println(F("Comanda invalida. Asigura-te ca servo∈{1..3}, unghi∈[0..180]"));
    }
  }
}
