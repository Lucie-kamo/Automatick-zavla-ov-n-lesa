#include <U8glib.h>
#include <Keypad.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

// ------------------
// Klavesnice
// ------------------
const byte radky = 4;
const byte sloupce = 4;
char klavesy[radky][sloupce] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinyRadku[radky] = {48, 46, 44, 42};
byte pinySloupcu[sloupce] = {40, 38, 36, 34};
Keypad klavesnice = Keypad(makeKeymap(klavesy), pinyRadku, pinySloupcu, radky, sloupce);

// ------------------
// Rezimy
// ------------------
enum Rezim { OZUBENEKOLO, NASTAVOVANI, ODPOCET, NASTAV_AUTO };
Rezim stav = OZUBENEKOLO;

// ------------------
// Odpočet
// ------------------
bool odpoctAktivni = false;
int odpoctSekundy = 30;
int bliknuti = 0;
unsigned long casStart = 0;

// ------------------
// Nastavovani auto zalevani
// ------------------
int pocetDni = 0;
int pocetHodin = 0;

// ------------------
// Vyber pro A/B
// ------------------
int vyber = 0;

// ------------------
// Pomocne funkce
// ------------------
void vykresliOzubeneKolo() {
  oled.firstPage();
  while(oled.nextPage()) {
    oled.setFont(u8g_font_helvB10);
    String top = "zalevani";
    int wTop = oled.getStrWidth(top.c_str());
    oled.setPrintPos((128 - wTop)/2, 20);
    oled.print(top);

    // nakresli "ozubene kolo" jako kolecko s carkami
    int stredX = 64;
    int stredY = 45;
    int radius = 15;
    oled.drawCircle(stredX, stredY, radius);
    for(int i=0;i<12;i++){
      float uhel = i*30.0*3.14159/180.0;
      int x1 = stredX + radius * cos(uhel);
      int y1 = stredY + radius * sin(uhel);
      int x2 = stredX + (radius-4) * cos(uhel);
      int y2 = stredY + (radius-4) * sin(uhel);
      oled.drawLine(x1, y1, x2, y2);
    }
  }
}

void vykresliZalevatTed() {
  oled.firstPage();
  while(oled.nextPage()) {
    oled.setFont(u8g_font_helvB10);
    String top = "zalevat ted";
    int wTop = oled.getStrWidth(top.c_str());
    oled.setPrintPos((128 - wTop)/2, 20);
    oled.print(top);

    oled.drawLine(0,50,128,50);
  }
}

void vykresliOdpoct() {
  oled.firstPage();
  while(oled.nextPage()) {
    oled.setFont(u8g_font_helvB14);
    int s = odpoctSekundy - (millis()-casStart)/1000;
    if(s<0) s=0;
    char buf[9];
    sprintf(buf, "00:00:%02d", s);
    int wTop = oled.getStrWidth(buf);
    oled.setPrintPos((128-wTop)/2, 20);
    oled.print(buf);

    oled.setFont(u8g_font_7x14);
    String bottom = "jeste zbyva";
    int wBot = oled.getStrWidth(bottom.c_str());
    oled.setPrintPos((128-wBot)/2, 40);
    oled.print(bottom);

    if(s==0 && odpoctAktivni) {
      odpoctAktivni = false;
      bliknuti = 0;
    }
  }
}

void vykresliAuto() {
  oled.firstPage();
  while(oled.nextPage()) {
    oled.setFont(u8g_font_helvB10);
    char buf[20];
    sprintf(buf, "%02ddnu %02dhod", pocetDni, pocetHodin);
    int wTop = oled.getStrWidth(buf);
    oled.setPrintPos((128-wTop)/2, 20);
    oled.print(buf);

    oled.setFont(u8g_font_7x14);
    String bottom = "auto zalevani";
    int wBot = oled.getStrWidth(bottom.c_str());
    oled.setPrintPos((128-wBot)/2, 40);
    oled.print(bottom);

    oled.drawLine(0,55,128,55);
  }
}

// ------------------
// Setup
// ------------------
void setup() {
}

// ------------------
// Loop
// ------------------
void loop() {
  char k = klavesnice.getKey();

  if(k){
    switch(k){
      case 'D':
        if(stav==OZUBENEKOLO) stav=NASTAVOVANI;
        else if(stav==NASTAVOVANI && !odpoctAktivni){
          stav=ODPOCET;
          casStart=millis();
          odpoctAktivni=true;
        }
        break;
      case 'C':
        if(stav==ODPOCET && odpoctAktivni){
          odpoctAktivni=false;
          stav=NASTAVOVANI;
        } else if(stav==NASTAV_AUTO){
          stav=OZUBENEKOLO;
        }
        break;
      case 'B':
        if(stav==NASTAVOVANI){
          stav=NASTAV_AUTO;
        } else if(stav==NASTAV_AUTO){
          pocetDni++; if(pocetDni>50) pocetDni=0;
        }
        break;
      case 'A':
        if(stav==NASTAV_AUTO){
          pocetHodin++; if(pocetHodin>23) pocetHodin=0;
        }
        break;
    }
  }

  // vykreslovani podle stavu
  if(stav==OZUBENEKOLO) vykresliOzubeneKolo();
  if(stav==NASTAVOVANI) vykresliZalevatTed();
  if(stav==ODPOCET && odpoctAktivni) vykresliOdpoct();
  if(stav==NASTAV_AUTO) vykresliAuto();
}
