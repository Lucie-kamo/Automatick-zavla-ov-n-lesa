#include <Wire.h>
#include <U8glib.h>
#include <Keypad.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

// ------------------
// Klávesnice
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
// Režimy
// ------------------
enum Rezim { STATISTIKA_OBRAZOVKA, STAT_GRAF, STAT_MINUT, STAT_LITRY };
Rezim stav = STATISTIKA_OBRAZOVKA;

// ------------------
// Graf
// ------------------
const int MAX_MONTHS = 3;
int waterings[MAX_MONTHS] = {3, 7, 15};
int monthNumber = 11;   // prosinec
const char* monthNames[12] = {"led","uno","bre","dub","kve","cvn",
                              "cvc","srp","zar","rij","lis","pro"};

// ------------------
// Funkce vykreslování
// ------------------
void vykresliStatistikaObrazovku() {
  oled.firstPage();
  do {
    oled.setFont(u8g_font_helvB10);
    String text = "Statistika";
    int w = oled.getStrWidth(text.c_str());
    oled.setPrintPos((128 - w)/2, 32);
    oled.print(text);
  } while(oled.nextPage());
}

void vykresliStatGraf() {
  oled.firstPage();
  do {
    const int chartTop = 10;
    const int chartBottom = 45;
    const int chartHeight = chartBottom - chartTop;
    const int barWidth = 30;
    const int barSpacing = 5;

    oled.setFont(u8g_font_6x10);

    // osa Y
    for(int yVal=0; yVal<=15; yVal+=5){
      int y = chartBottom - (yVal * chartHeight / 15);
      oled.drawLine(10, y, 110, y);
      oled.setPrintPos(0, y+3);
      oled.print(yVal);
    }

    // sloupce
    for(int i=0; i<MAX_MONTHS; i++){
      int val = waterings[i];
      int x = 20 + i*(barWidth+barSpacing);
      int barH = val * chartHeight / 15;
      oled.drawFrame(x, chartBottom - barH, barWidth, barH);
      oled.drawBox(x+1, chartBottom - barH +1, barWidth-2, barH-2);

      // popisek měsíce
      int m = (monthNumber - (MAX_MONTHS - 1 - i) + 12) % 12;
      oled.setPrintPos(x + 5, chartBottom + 10);
      oled.print(monthNames[m]);
    }
  } while(oled.nextPage());
}

void vykresliStatMinut() {
  oled.firstPage();
  do {
    oled.setFont(u8g_font_helvB14);
    String hodnota = "126 minut";
    int w = oled.getStrWidth(hodnota.c_str());
    oled.setPrintPos((128 - w)/2, 25);
    oled.print(hodnota);

    oled.setFont(u8g_font_7x14);
    String podtitul = "celkem";
    int w2 = oled.getStrWidth(podtitul.c_str());
    oled.setPrintPos((128 - w2)/2, 45);
    oled.print(podtitul);
  } while(oled.nextPage());
}

void vykresliStatLitry() {
  oled.firstPage();
  do {
    oled.setFont(u8g_font_helvB14);
    String hodnota = "4 L";
    int w = oled.getStrWidth(hodnota.c_str());
    oled.setPrintPos((128 - w)/2, 25);
    oled.print(hodnota);

    oled.setFont(u8g_font_7x14);
    String podtitul = "celkem";
    int w2 = oled.getStrWidth(podtitul.c_str());
    oled.setPrintPos((128 - w2)/2, 45);
    oled.print(podtitul);
  } while(oled.nextPage());
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
      case 'C':
        stav = STATISTIKA_OBRAZOVKA;  // vždy návrat na start
        break;
      case 'A':
        // posun o režim nahoru cyklicky
        if(stav == STAT_GRAF) stav = STAT_LITRY;
        else if(stav == STAT_MINUT) stav = STAT_GRAF;
        else if(stav == STAT_LITRY) stav = STAT_MINUT;
        break;
      case 'B':
        // postupné přepínání
        if(stav == STATISTIKA_OBRAZOVKA) stav = STAT_GRAF;
        else if(stav == STAT_GRAF) stav = STAT_MINUT;
        else if(stav == STAT_MINUT) stav = STAT_LITRY;
        else if(stav == STAT_LITRY) stav = STAT_GRAF;
        break;
    }
  }

  // vykreslování podle stavu
  switch(stav){
    case STATISTIKA_OBRAZOVKA:
      vykresliStatistikaObrazovku();
      break;
    case STAT_GRAF:
      vykresliStatGraf();
      break;
    case STAT_MINUT:
      vykresliStatMinut();
      break;
    case STAT_LITRY:
      vykresliStatLitry();
      break;
  }
}
