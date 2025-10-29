#include <Wire.h>
#include <DS3231.h>

DS3231 rtc;
RTCDateTime datumCas;

void setup() {
  Serial.begin(9600);
  rtc.begin();
  rtc.setDateTime(__DATE__, __TIME__);

}

void loop() {
  datumCas = rtc.getDateTime();
  Serial.print(datumCas.year); Serial.print("-");
  Serial.print(datumCas.month); Serial.print("-");
  Serial.print(datumCas.day); Serial.print(" ");
  Serial.print(datumCas.hour); Serial.print(":");
  Serial.print(datumCas.minute); Serial.print(":");
  Serial.print(datumCas.second); Serial.println("");
  delay(1000);
}
