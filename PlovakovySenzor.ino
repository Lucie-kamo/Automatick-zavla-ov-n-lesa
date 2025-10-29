#define pinVstup 2
unsigned long casAktivace = 0;
unsigned long posledniVypis = 0;
bool stavSepnuto = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinVstup, INPUT_PULLUP); // interní pull-up, senzor k GND
}

void loop() {
  unsigned long now = millis();
  
  // kontrola, zda je čas na nový výpis (každou sekundu)
  if (now - posledniVypis >= 1000) {
    bool stav = digitalRead(pinVstup); // čteme stav senzoru
    if (stav == LOW) { // sepnutý senzor
      Serial.println("Detekovani sepnuti plovakoveho senzoru");
      stavSepnuto = true;
      casAktivace = now; // aktualizuje čas posledního sepnutí
    } else { // senzor není sepnutý
      Serial.print("Cas od posledni aktivace plovakoveho senzoru: ");
      Serial.print(now - casAktivace);
      Serial.println(" ms.");
      stavSepnuto = false;
    }
    posledniVypis = now; // aktualizujeme čas posledního výpisu
  }
}
