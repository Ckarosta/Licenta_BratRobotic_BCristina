#include <Servo.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(10, 11);

String culoareCos[3] = {"RED", "GREEN", "BLUE"};
int nrCosuri = 3;

Servo servo1, servo2, servo3, servo4;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_600MS,
  TCS34725_GAIN_1X
);

bool sortareActiva = false;

void moveServo(Servo &s, int from, int to, int stepDelay = 15);
void pozitieInitiala();
void preluareObiect();
void sorteazaInCos(String culoare);
String deduceCuloare(uint16_t r, uint16_t g, uint16_t b);

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);

  pozitieInitiala();

  if (tcs.begin()) {
    Serial.println("TCS34725 OK");
  } else {
    Serial.println("Eroare senzor");
    while (1);
  }

  Serial.println("Aștept configurare din Node-RED...");
}

void loop() {
  if (Serial.available()) {
    String linie = Serial.readStringUntil('\n');
    linie.trim();
    if (linie.length() == 0) return;

    if (linie == "START") {
      Serial.println("Sortarea a fost PORNITĂ!");
      sortareActiva = true;
      return;
    }

    if (linie == "STOP") {
      Serial.println("Sortarea a fost OPRITĂ!");
      sortareActiva = false;
      return;
    }

    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, linie);
    if (!error) {
      nrCosuri = doc["nrCosuri"];
      JsonArray culori = doc["culori"].as<JsonArray>();
      for (int i = 0; i < nrCosuri; i++) {
        culoareCos[i] = String((const char*)culori[i]);
        Serial.print("Coș "); Serial.print(i + 1);
        Serial.print(" → culoare: "); Serial.println(culoareCos[i]);
      }
      Serial.println("✔ CONFIGURARE COMPLETĂ!");
      sortareActiva = false;
      return;
    }

    Serial.print("⚠ Eroare: "); Serial.println(linie);
  }

  if (!sortareActiva) return;

 // MEDIA valorilor RGB
uint32_t rSum = 0, gSum = 0, bSum = 0;
for (int i = 0; i < 5; i++) {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  rSum += r;
  gSum += g;
  bSum += b;
  delay(50);
}
uint16_t r = rSum / 5;
uint16_t g = gSum / 5;
uint16_t b = bSum / 5;

float sum = r + g + b;
float rNorm = (r / sum) * 255.0;
float gNorm = (g / sum) * 255.0;
float bNorm = (b / sum) * 255.0;

// Trimite și valorile RAW + SUM + NORMALIZED
espSerial.print("RAW: R=");
espSerial.print(r);
espSerial.print(" G=");
espSerial.print(g);
espSerial.print(" B=");
espSerial.print(b);
espSerial.print(" SUM=");
espSerial.print(sum);
espSerial.print(" | NORMALIZED: R=");
espSerial.print((int)rNorm);
espSerial.print(" G=");
espSerial.print((int)gNorm);
espSerial.print(" B=");
espSerial.println((int)bNorm);

Serial.print("R="); Serial.print(r);
Serial.print(" G="); Serial.print(g);
Serial.print(" B="); Serial.println(b);

String culoareDetectata = deduceCuloare(r, g, b);
  
if (culoareDetectata == "BLACK") {
  Serial.println("-> Nicio culoare detectata, posibil fundal negru");
  espSerial.println("COLOR:BLACK");
  return;
}

  Serial.print("-> Obiect detectat: ");
  Serial.println(culoareDetectata);

  preluareObiect();
  sorteazaInCos(culoareDetectata);
  pozitieInitiala();
  delay(2000);
}

String deduceCuloare(uint16_t r, uint16_t g, uint16_t b) {
  float sum = r + g + b;
  if (sum < 100) return "BLACK"; 

  float rNorm = (r / sum) * 255.0;
  float gNorm = (g / sum) * 255.0;
  float bNorm = (b / sum) * 255.0;

  // Detectare extinsă după reguli
  if (rNorm > 60 && gNorm > 110 && bNorm < 80) return "YELLOW";
  if (rNorm > 100 && gNorm > 70 && gNorm < 120 && bNorm < 80) return "ORANGE";
  if (rNorm > 80 && gNorm < 100 && bNorm < 100) return "RED";
  if (bNorm > 115 && gNorm > 60 && gNorm < 110 && rNorm < 70) return "VIOLET";
  if (gNorm > 105 && rNorm < 85 && bNorm < 85) return "GREEN";
  if (bNorm > 105 && gNorm > 85 && gNorm < 125 && rNorm < 60) return "BLUE";

  if (rNorm >= gNorm && rNorm >= bNorm) return "RED";
  if (gNorm >= rNorm && gNorm >= bNorm) return "GREEN";
  if (bNorm >= rNorm && bNorm >= gNorm) return "BLUE";
}


void pozitieInitiala() {
  moveServo(servo1, servo1.read(), 120);
  moveServo(servo2, servo2.read(), 150);
  moveServo(servo3, servo3.read(), 90);
  moveServo(servo4, servo4.read(), 100);
  delay(500);
}

void preluareObiect() {
  Serial.println("Cobor spre obiect");
  moveServo(servo1, servo1.read(), 70);
  delay(900);

  Serial.println("Cobor cotul");
  moveServo(servo2, servo2.read(), 175);
  delay(900);

  Serial.println("Amplasez incheietura");
  moveServo(servo3, servo3.read(), 0);
  delay(900);

  Serial.println("Prind obiect");
  moveServo(servo4, servo4.read(), 50);
  delay(900);

  Serial.println("Ridic cotul");
  moveServo(servo2, servo2.read(), 85);
  delay(900);
}

void sorteazaInCos(String culoare) {
  culoare.trim(); culoare.toUpperCase();

  bool gasit = false;
  for (int i = 0; i < nrCosuri; i++) {
    String c = culoareCos[i];
    c.trim(); c.toUpperCase();
    Serial.print("[DEBUG] Compar: "); Serial.print(culoare); Serial.print(" cu "); Serial.println(c);

    if (c == culoare) {
      gasit = true;
      if (i == 0) {
        Serial.println("Cos 1");
        moveServo(servo1, servo1.read(), 120);
        moveServo(servo2, servo2.read(), 90);
        moveServo(servo3, servo3.read(), 180);
        moveServo(servo4, servo4.read(), 100);
        delay(900);
      } else if (i == 1) {
        Serial.println("Cos 2");
        moveServo(servo1, servo1.read(), 140);
        moveServo(servo2, servo2.read(), 90);
        moveServo(servo3, servo3.read(), 140);
        moveServo(servo4, servo4.read(), 100);
        delay(900);
      } else if (i == 2) {
        Serial.println("Cos 3");
        moveServo(servo1, servo1.read(), 160);
        moveServo(servo2, servo2.read(), 100);
        moveServo(servo3, servo3.read(), 80);
        moveServo(servo4, servo4.read(), 100);
        delay(900);
      }
      Serial.print("COLOR:"); Serial.println(culoare);
      espSerial.print("COLOR:"); espSerial.println(culoare);
      break;
    }
  }
  if (!gasit) {
    Serial.println("Nicio culoare asociata! Nu fac miscare");
  }
}

void moveServo(Servo &s, int from, int to, int stepDelay) {
  int pas = (from < to) ? 1 : -1;
  for (int pos = from; pos != to; pos += pas) {
    s.write(pos);
    delay(stepDelay);
  }
  s.write(to);
}