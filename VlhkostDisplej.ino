#define LED_ZELENA 30
#define LED_ORANZOVA 32
#define LED_CERVENA 34
#include "U8glib.h"


const int dry = 563;
const int wet = 199;

U8GLIB_SSD1306_128X64 oled_1(U8G_I2C_OPT_NONE);   

void vykresli(int percentageHumidity)
{
  
  oled_1.firstPage();
  while(oled_1.nextPage()){
    // nastavení fontu, odkaz v článku
    oled_1.setFont(u8g_font_helvB14);
    String velkeCislo = String(percentageHumidity) + "%";
    int textWidth = oled_1.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    int xPos = (128 - textWidth) / 2; // vycentrování horizontálně
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(velkeCislo);
    
    // změna fontu
    oled_1.setFont(u8g_font_7x14);
    String text = "vlhkost";
    textWidth = oled_1.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;     // vycentrování
    oled_1.setPrintPos(xPos, 50);      // pod číslem
    oled_1.print(text);
    // rovná čára (od souřadnic -> do souřadnic)
    oled_1.drawLine(0,61,128,61);
  }
    
}

void setup(void) {
  Serial.begin(9600);
  pinMode(LED_ZELENA, OUTPUT);
  pinMode(LED_ORANZOVA, OUTPUT);
  pinMode(LED_CERVENA, OUTPUT);
}

void loop(void) {
  int sensorVal = analogRead(A0);  
  int percentageHumidity = map(sensorVal, wet, dry, 100, 0);
  vykresli(percentageHumidity);
  // "obnovovací frekvence displeje"
  delay(500);

}
