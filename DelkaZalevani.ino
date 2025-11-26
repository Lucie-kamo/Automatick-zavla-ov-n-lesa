#include "U8glib.h"
#include <Keypad.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

// klávesnice
const byte radky = 4;
const byte sloupce = 4;

char klavesy[radky][sloupce] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinyRadku[radky]   = {48, 46, 44, 42};
byte pinySloupcu[sloupce] = {40, 38, 36, 34};

Keypad klavesnice = Keypad(makeKeymap(klavesy), pinyRadku, pinySloupcu, radky, sloupce);

// ------------------
//   ULOŽENÝ ČAS
// ------------------
int hh = 0;
int mm = 0;
int ss = 0;

// 0 = hodiny, 1 = minuty, 2 = sekundy
int vyber = 0;

// počítadlo cifry (0 nebo 1)
int cifra = 0;

String doplnNulu(int cislo) {
  if (cislo < 10) return "0" + String(cislo);
  return String(cislo);
}

void vykresliCas() {
  oled.firstPage();
  while (oled.nextPage()) {

    oled.setFont(u8g_font_helvB14);

    // -- velký čas --
    String cas = doplnNulu(hh) + ":" + doplnNulu(mm) + ":" + doplnNulu(ss);

    int width = oled.getStrWidth(cas.c_str());
    int x = (128 - width) / 2;

    oled.setPrintPos(x, 30);
    oled.print(cas);

    // text
    oled.setFont(u8g_font_7x14);
    String text = "delka zalevani";
    width = oled.getStrWidth(text.c_str());
    x = (128 - width) / 2;

    oled.setPrintPos(x, 50);
    oled.print(text);

    oled.drawLine(0,61,128,61);

    // podtržení vybrané pozice
    // posunutí aby to sedlo
    oled.drawLine(x + 11 + vyber*27, 34, x + 11 + vyber*27 + 20, 34);
  }
}

void setup() {}

void loop() {

  char k = klavesnice.getKey();

  if (k) {

    // posun dopředu
    if (k == '#') {
      vyber++;
      if (vyber > 2) vyber = 0;
      cifra = 0;
    }

    // posun dozadu
    if (k == '*') {
      vyber--;
      if (vyber < 0) vyber = 2;
      cifra = 0;
    }

    // reset
    if (k == 'D') {
      hh = mm = ss = 0;
      cifra = 0;
    }

    // inkrementace
    if (k == 'A') {
      if (vyber == 0) { hh++; if (hh > 23) hh = 0; }
      if (vyber == 1) { mm++; if (mm > 59) mm = 0; }
      if (vyber == 2) { ss++; if (ss > 59) ss = 0; }
    }

    // dekrementace
    if (k == 'B') {
      if (vyber == 0) { hh--; if (hh < 0) hh = 23; }
      if (vyber == 1) { mm--; if (mm < 0) mm = 59; }
      if (vyber == 2) { ss--; if (ss < 0) ss = 59; }
    }

    // -------------- ČÍSELNÉ ZADÁVÁNÍ --------------
    if (k >= '0' && k <= '9') {

      int cislo = k - '0';

      if (vyber == 0) {         // HODINY
        if (cifra == 0) hh = cislo * 10;
        else hh = hh + cislo;

        if (hh > 23) hh = 23;
      }

      if (vyber == 1) {         // MINUTY
        if (cifra == 0) mm = cislo * 10;
        else mm = mm + cislo;

        if (mm > 59) mm = 59;
      }

      if (vyber == 2) {         // SEKUNDY
        if (cifra == 0) ss = cislo * 10;
        else ss = ss + cislo;

        if (ss > 59) ss = 59;
      }

      cifra++;

      // po dvou cifrách pokračovat dál
      if (cifra >= 2) {
        cifra = 0;
        vyber++;
        if (vyber > 2) vyber = 0;
      }
    }
  }

  vykresliCas();
}
