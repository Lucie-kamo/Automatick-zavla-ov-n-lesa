#include <Wire.h>
#include <DS3231.h>
#include "U8glib.h"

DS3231 rtc;
RTCDateTime datumCas;
int textWidth = 0;

U8GLIB_SSD1306_128X64 oled_1(U8G_I2C_OPT_NONE); 

String doplnNulu(int cislo) {
  if (cislo < 10) return "0" + String(cislo);
  else return String(cislo);
}

void vykresliDatumCas(RTCDateTime datumCas){
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);
    String cas = String(doplnNulu(datumCas.hour)) + ":" + String(doplnNulu(datumCas.minute)) + ":" + String(doplnNulu(datumCas.second));
    textWidth = oled_1.getStrWidth(cas.c_str());
    int xPos = (128 - textWidth)/2;
    oled_1.setPrintPos(xPos, 30);
    oled_1.print(cas);

    String mesic = "";
    switch(datumCas.month){
      case 1: mesic = "leden"; break;
      case 2: mesic = "unor"; break;
      case 3: mesic = "brezen"; break;
      case 4: mesic = "duben"; break;
      case 5: mesic = "kveten"; break;
      case 6: mesic = "cerven"; break;
      case 7: mesic = "cervenec"; break;
      case 8: mesic = "srpen"; break;
      case 9: mesic = "zari"; break;
      case 10: mesic = "rijen"; break;
      case 11: mesic = "listopad"; break;
      case 12: mesic = "prosinec"; break;
    }

    oled_1.setFont(u8g_font_7x14);
    String datum = String(datumCas.day) + ". " + mesic + " " + String(datumCas.year);
    textWidth = oled_1.getStrWidth(datum.c_str());
    xPos = (128 - textWidth) / 2;     
    oled_1.setPrintPos(xPos, 50);      
    oled_1.print(datum);
    oled_1.drawLine(0,61,128,61);
  }
}

void setup() {
  Serial.begin(9600);
  rtc.begin();
  rtc.setDateTime(__DATE__, __TIME__);
}

void loop() {
  datumCas = rtc.getDateTime();
  vykresliDatumCas(datumCas);
  delay(1000);
}
