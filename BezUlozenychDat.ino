/** ------------------
* includovane knihovny
* DOPSAT CO JE JAKY HLAVICKOVY SOUBOR
* knihovna - oled displej
* knihovna - klavesnice
* knihovna - RTC MYSLIM
------------------ */

#include <Wire.h>
#include <U8glib.h>
#include <Keypad.h>
#include <DS3231.h>
#define pinVstup 2 //cidlo vlhkosti
#define LED_ZELENA 3

/** ------------------
* PROMENNE 
------------------*/

//U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE); ///pridat popisek
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);   
U8GLIB_SSD1306_128X64 oled_1(U8G_I2C_OPT_NONE);

/**
* uvodni bitmapa 128x64 ulozena v PROGMEM
* VYSVETLIT PROGMEM
*/
const uint8_t uvodni[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf7, 0x3b, 0xbc, 0x10, 0x41, 0xb7, 0x78, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xf7, 0x3b, 0x5f, 0x77, 0xdd, 0xb3, 0x77, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xfa, 0xb7, 0x5f, 0x77, 0xdd, 0xb5, 0x6f, 0xff, 0xff, 0xff, 0xfe, 0x7e, 0x79, 0xff, 0xff, 0xff, 
	0xfa, 0xd6, 0xef, 0x70, 0xc3, 0xb5, 0x6f, 0xff, 0xff, 0xff, 0xfe, 0x7e, 0x79, 0xff, 0xff, 0xff, 
	0xfa, 0xce, 0x0f, 0x77, 0xdd, 0xb6, 0x6e, 0xff, 0xff, 0xff, 0x98, 0x60, 0x00, 0x7f, 0xff, 0xff, 
	0xfd, 0xce, 0xef, 0x77, 0xdd, 0xb6, 0x66, 0xff, 0xff, 0xff, 0x98, 0x60, 0x00, 0x7f, 0xff, 0xff, 
	0xfd, 0xed, 0xf7, 0x70, 0x5e, 0xb7, 0x70, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x67, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x67, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe6, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe6, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xf8, 0x2e, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xfe, 0xee, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfe, 0xee, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfe, 0xe0, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
	0xfe, 0xee, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
	0xfe, 0xee, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x40, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfe, 0xee, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x73, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x12, 0x40, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1e, 0x40, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x07, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x07, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x18, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x18, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x1f, 0xff, 
	0xf8, 0x71, 0xc1, 0x83, 0x08, 0x3f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x01, 0xff, 
	0xfb, 0xee, 0xdd, 0xbe, 0xfe, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x01, 0xff, 
	0xfb, 0xdf, 0x5d, 0xbe, 0x7e, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x07, 0xff, 
	0xf8, 0xdf, 0x43, 0x87, 0x1e, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x07, 0xff, 
	0xfb, 0xdf, 0x5d, 0xbf, 0xee, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x07, 0xff, 
	0xfb, 0xee, 0xdd, 0xbf, 0xee, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x07, 0xff, 
	0xfb, 0xf1, 0xde, 0x82, 0x1e, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x40, 0x1f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x1f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xf8, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xfe, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xfe, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0x9f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0x9f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/// klavesnice
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

/// RTC - obvod realneho casu
DS3231 rtc;
RTCDateTime datumCas;

/// senzor vlhkosti pudy
const int sucho = 563;
const int vlhko = 199;

// ------------------
// Globální proměnné
// ------------------
/// rezimy
int aktualniRezim = 0; //oduvodnit int
unsigned long posledniVypis = 0; //oduvodnit unsigned long, nevim co to je za promennou

///text
int xPos = 0;
int textWidth = 0;

///DALSI ZATIM NEVIM OD CEHO 
unsigned long casAktivace = 0;
bool stavSepnuto = false;
unsigned long ted = millis(); //mela jsem now a ne ted
String hladina ="";

///delka zalevani
int hh = 0, mm = 0, ss = 0;
int vyberCas = 0; //0=hh, 1=mm, 2=ss
int cifra = 0;

///rezim 6 - auto-zalevani (podrezimy)
int rezimZalevani = 0; //z celkoveho poctu 4
//kdyby enum tak Rezim6 stav6 = OZUBENEKOLO;
bool odpocetAktivni = false;
unsigned long casStart = 0;
int pocetDni = 0, pocetHodin = 0;

///rezim 7 - statistika (podrezimy)
int rezimStatistika = 0;
//kdyby enum tak Rezim7 stav7 = STAT_OBR;
const int maxMesicu = 3;
int zalitoSloupce[maxMesicu] = {3,7,15}; //CISLA DANA NA TVRDO  
//potrebuju? int monthNumber = 11;
const char* nazvyMesicu[12] = {"led","uno","bre","dub","kve","cvn", ///char* - retezce
                              "cvc","srp","zar","rij","lis","pro"};

/** ------------------
* FUNKCE 
------------------*/

void vykresli()
{
  //aktualniRezim = 0;
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.drawBitmapP(0, 0, 16, 64, uvodni); // 128/8 = 16 sloupců po 8 pixelech
  }
}

void vykresliVlhkost(int procentoVlhkosti){
  //aktualniRezim = 1;
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);
    String velkeCislo = String(procentoVlhkosti) + "%";
    textWidth = oled_1.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(velkeCislo);

    oled_1.setFont(u8g_font_7x14);
    String text = "vlhkost";
    textWidth = oled_1.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 50);      // pod číslem
    oled_1.print(text);
    // rovná čára (od souřadnic -> do souřadnic)
    oled_1.drawLine(0,61,128,61);
  }
}

void vykresliHladinu(bool stav){
  //aktualniRezim = 2;
  oled_1.firstPage();
  if (stav == LOW) { 
    stavSepnuto = true;
    casAktivace = millis(); //now; - bylo predtim - ja prepisu na ted
  } 
  else { 
    stavSepnuto = false;
  }
  //posledniVypis = now;

  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);

    if(stavSepnuto == true){
      hladina = "ok!";
    }
    else{
      hladina = "nedostatek";
    }//lze optimalizovat

    textWidth = oled_1.getStrWidth(hladina.c_str()); 
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);
    oled_1.print(hladina);

    oled_1.setFont(u8g_font_7x14);
    String text = "hladina v nadrzi";
    textWidth = oled_1.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 50);      // pod číslem
    oled_1.print(text);
    // rovná čára (od souřadnic -> do souřadnic)
    oled_1.drawLine(0,61,128,61);
  }
}

String doplnNulu(int cislo) {
  if (cislo < 10){
    return "0" + String(cislo);
  } 
  else {
    return String(cislo);
  }
}

void vykresliDatumCas(RTCDateTime datumCas){
  //aktualniRezim = 3;
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

void vykresliKlavesnice(char klavesa){
  //aktualniRezim = 4;
  static String zapsano = "?"; ///static String zapsano = "?"; STATIC TAM FAKT MUSI BYT

  if(klavesa >= '0' && klavesa <= '9'){
    if(zapsano == "?"){
      zapsano = "";
    }
    int cislo = zapsano.toInt();
    if(zapsano.length() < 3 && cislo < 10){ //max 3 cislice
      zapsano += klavesa;
      int cislo = zapsano.toInt();
    }
    else{
      zapsano = String(klavesa);
    }
  }
  else if(klavesa == 'D'){ // smazat vse
    zapsano = "?";
  }


  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);
    textWidth = oled_1.getStrWidth(zapsano.c_str());
    xPos = (128 - textWidth)/2;
    oled_1.setPrintPos(xPos, 30);
    oled_1.print(zapsano + "%");

    oled_1.setFont(u8g_font_7x14);
    String text = "zalevat ?% <= " + String(zapsano);
    textWidth = oled_1.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;     
    oled_1.setPrintPos(xPos, 50);      
    oled_1.print(text);
    oled_1.drawLine(0,61,128,61);
  }
}

//nastaveni doby zalevani
void vykresliCasZalevani(char klavesa){
  if(klavesa >= '0' && klavesa <= '9'){
    int cislo = klavesa - '0';
    if(vyberCas == 0){
      hh = hh*10 + cislo;
      if(hh > 23){
        hh = cislo;
      }
    }
    else if(vyberCas == 1){
      mm = mm*10 + cislo;
      if(mm > 59){
        mm = cislo;
      }
    }
    else if(vyberCas == 2){
      ss = ss*10 + cislo;
      if(ss > 59){
        ss = cislo;
      }
    }
  }
  else if(klavesa == 'C'){ // přepnutí mezi hh, mm, ss
    vyberCas++;
    if(vyberCas > 2) vyberCas = 0;
  }
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);
    String cas = doplnNulu(hh)+":"+doplnNulu(mm)+":"+doplnNulu(ss);
    int w = oled_1.getStrWidth(cas.c_str());
    int x = (128-w)/2;
    oled_1.setPrintPos(x,30);
    oled_1.print(cas);

    oled_1.setFont(u8g_font_7x14);
    String textVypsat="delka zalevani";
    w = oled_1.getStrWidth(textVypsat.c_str());
    x = (128-w)/2;
    oled_1.setPrintPos(x,50);
    oled_1.print(textVypsat);
    oled_1.drawLine(0,61,128,61);

    //podtrzeni vybrane pozice
    oled_1.drawLine(x+11+vyberCas*27,34,x+11+vyberCas*27+20,34);
  }
}

//rezim 6
void vykresliOzubene(){
  //aktualniRezim = 6;
  //rezimZalevani = 0;
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);
    String nadpisZalevani = "zalevani";
    textWidth = oled_1.getStrWidth(nadpisZalevani.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(nadpisZalevani);

    int stredX = 64, stredY = 45;
    int radius = 10;      // menší kruh
    int delkaZubu = 5;    // délka "zubů" ven

    oled_1.drawCircle(stredX, stredY, radius);

    for(int i = 0; i < 12; i++){
      float uhel = i * 30 * 3.14159 / 180; // 12 indexů
      int x1 = stredX + radius * cos(uhel);          // začátek zubu na obvodu
      int y1 = stredY + radius * sin(uhel);
      int x2 = stredX + (radius + delkaZubu) * cos(uhel);  // konec zubu ven
      int y2 = stredY + (radius + delkaZubu) * sin(uhel);
      oled_1.drawLine(x1, y1, x2, y2);
    }


    
/*
    oled_1.setFont(u8g_font_7x14);
    String text = "vlhkost";
    textWidth = oled_1.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 50);      // pod číslem
    oled_1.print(text);
    // rovná čára (od souřadnic -> do souřadnic)
    oled_1.drawLine(0,61,128,61);*/
  }
}

void vykresliZalevatTed(){
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB10);
    String textTed ="zalevat ted";
    textWidth = oled_1.getStrWidth(textTed.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(textTed);
  }
}

void vykresliOdpocet(){
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);

    //spocitat sekundy
    int s = 30 - (millis() - casStart) / 1000;
    if(s < 0){
      s = 0;
    }
    String cas = "00:00:"+doplnNulu(s);

    textWidth = oled_1.getStrWidth(cas.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(cas);


    oled_1.setFont(u8g_font_7x14);
    String textZbytek = "jeste zbyva";
    textWidth = oled_1.getStrWidth(textZbytek.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 50);      // pod číslem
    oled_1.print(textZbytek);
    // rovná čára (od souřadnic -> do souřadnic)
    oled_1.drawLine(0,61,128,61);

    /*if(s == 0){
      rezimZalevani = 0;
    }*/
  }
}

void vykresliPravidelne(){
  //rezimStatistika = 2;
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB10);
    String textPravidelne ="kolikrat";
    textWidth = oled_1.getStrWidth(textPravidelne.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(textPravidelne);
  }
}

//rezim 7 - statistika
void vykresliStatistika(){
  //aktualniRezim = 7;
  //rezimStatistika = 0;
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB10);
    String textStatistika ="Statistika";
    textWidth = oled_1.getStrWidth(textStatistika.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(textStatistika);

    //graf
    int left = 50;    // levý okraj čtverce
    int top = 35;     // horní okraj čtverce
    int size = 20;    // velikost čtverce

    // nakreslení čtverce
    oled_1.drawFrame(left, top, size, size);

    // body pro stoupající křivku s malými propady
    int body[] = {3, 5, 7, 6, 8, 10, 9, 12, 14, 13, 15, 18, 17, 19, 20};
    int pocetBodu = sizeof(body)/sizeof(body[0]);

    for(int i = 0; i < pocetBodu - 1; i++){
      int x1 = left + i * size / pocetBodu;
      int y1 = top + size - body[i];  // invert Y, protože OLED (0,0 nahoře)
      int x2 = left + (i + 1) * size / pocetBodu;
      int y2 = top + size - body[i+1];
      oled_1.drawLine(x1, y1, x2, y2);
    }

    //graf
  }
}

void vykresliGraf(){
  //rezimStatistika = 1;
  oled_1.firstPage();
  while(oled_1.nextPage()){
    const int horniOkraj = 10, spodniOkraj = 45, vyska = spodniOkraj - horniOkraj;
    const int sirkaSloupce = 30, mezeraSloupce = 5;
    oled_1.setFont(u8g_font_6x10);

    for(int hodnotaY = 0; hodnotaY <= 15; hodnotaY += 5){
      int y = spodniOkraj - (hodnotaY * vyska / 15);
      oled_1.drawLine(10, y, 110, y);
      oled_1.setPrintPos(0, y + 3);
      oled_1.print(hodnotaY);
    }

    for(int i = 0; i < maxMesicu; i++){
      int hodnota = zalitoSloupce[i];
      int x = 20 + i * (sirkaSloupce + mezeraSloupce);
      int vyskaSloupce = hodnota * vyska / 15;
      oled_1.drawFrame(x, spodniOkraj - vyskaSloupce, sirkaSloupce, vyskaSloupce);
      oled_1.drawBox(x + 1, spodniOkraj - vyskaSloupce + 1, sirkaSloupce - 2, vyskaSloupce - 2);
      int cisloMesice = (datumCas.month - (maxMesicu - i) + 12) % 12;
      oled_1.setPrintPos(x + 5, spodniOkraj + 10);
      oled_1.print(nazvyMesicu[cisloMesice]);
    }
  }
}

void vykresliStatMinut(){
  //rezimStatistika = 2;
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);
    String velkeCislo = "126 minut"; //NATVRDO
    textWidth = oled_1.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(velkeCislo);

    oled_1.setFont(u8g_font_7x14);
    String textMinut = "zalevalo se";
    textWidth = oled_1.getStrWidth(textMinut.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 50);      // pod číslem
    oled_1.print(textMinut);
    // rovná čára (od souřadnic -> do souřadnic)
    oled_1.drawLine(0,61,128,61);
  }
}

void vykresliStatLitry(){
  //rezimStatistika = 3;
  oled_1.firstPage();
  while(oled_1.nextPage()){
    oled_1.setFont(u8g_font_helvB14);
    String velkeCislo = "12L"; //NATVRDO
    textWidth = oled_1.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 30);      // vertikální pozice
    oled_1.print(velkeCislo);

    oled_1.setFont(u8g_font_7x14);
    String textLitr = "spotreba vody";
    textWidth = oled_1.getStrWidth(textLitr.c_str());
    xPos = (128 - textWidth) / 2;  
    oled_1.setPrintPos(xPos, 50);      // pod číslem
    oled_1.print(textLitr);
    // rovná čára (od souřadnic -> do souřadnic)
    oled_1.drawLine(0,61,128,61);
  }
}



void setup() {
  ///ODUVODNIT VSE
  Serial.begin(9600);
  pinMode(pinVstup, INPUT_PULLUP);
  rtc.begin();
  if(rtc.isReady()){
    rtc.setDateTime(__DATE__, __TIME__);
  }
  pinMode(LED_ZELENA, OUTPUT);
}

void loop() {
  datumCas = rtc.getDateTime();
  ted = millis(); //mela jsem psano now
  char klavesa = klavesnice.getKey();
  ///tady ty sensor val a tak jeste prepsat do cestiny
  int sensorVal = analogRead(A0);  
  int procentoVlhkosti = map(sensorVal, vlhko, sucho, 100, 0);
  //nevim kam dat
  bool stav = digitalRead(pinVstup); 
  digitalWrite(LED_ZELENA, LOW);
  

  switch(klavesa){
    case '*': aktualniRezim = aktualniRezim - 1;
              if(aktualniRezim < 0){
                aktualniRezim = 7;
              }
              break;
    case '#': aktualniRezim = aktualniRezim + 1;
              if(aktualniRezim > 7){
                aktualniRezim = 0;
              }
              break;
    case 'A': if(aktualniRezim == 6){
                rezimZalevani--;
                if(rezimZalevani < 0){
                  rezimZalevani = 2;
                }
              }
              else if(aktualniRezim == 7){
                rezimStatistika--;
                if(rezimStatistika < 0){
                  rezimStatistika = 3;
                }
              }
              break;
    case 'B': if(aktualniRezim == 6){
                rezimZalevani++;
                if(rezimZalevani > 2){
                  rezimZalevani = 0;
                }
              }
              else if(aktualniRezim == 7){
                rezimStatistika++;
                if(rezimStatistika > 3){
                  rezimStatistika = 0;
                }
              }
              break;
  }

  if(aktualniRezim == 6 && rezimZalevani == 1 && klavesa == 'C' && !odpocetAktivni){
    //vykresliOdpocet();
    odpocetAktivni = true;
    casStart = millis();
  }

  if(klavesa == 'D' && odpocetAktivni){
    odpocetAktivni = false;
  }

  //tim si nejsem jista
  if(odpocetAktivni){
    int s = 30 - (millis() - casStart) / 1000;
    digitalWrite(LED_ZELENA, HIGH);
    if(s <= 0){
      odpocetAktivni = false;   // konec
      digitalWrite(LED_ZELENA, LOW);
    }
  }


  if(aktualniRezim == 4){
    vykresliKlavesnice(klavesa);
  }
  else if (aktualniRezim == 5){
    vykresliCasZalevani(klavesa);
  }

  if(ted - posledniVypis >= 1000){
    posledniVypis = ted;
    switch(aktualniRezim){
      case 0: vykresli(); break;
      case 1: vykresliVlhkost(procentoVlhkosti); break;
      case 2: vykresliHladinu(stav); break;
      case 3: vykresliDatumCas(datumCas); break;
      case 6: 
              if(odpocetAktivni){
                vykresliOdpocet();
              }
              else{
                switch(rezimZalevani){
                  case 0: vykresliOzubene();break;
                  case 1: vykresliZalevatTed(); /*musi cekat na stisk klavesy*/ break;
                  case 2: vykresliPravidelne(); break;
                }
              }
              break; 
      case 7: 
              switch(rezimStatistika){
                case 0: vykresliStatistika(); break;
                case 1: vykresliGraf(); break;
                case 2: vykresliStatMinut(); break;
                case 3: vykresliStatLitry(); break;
              }
              break;
      
    }
  }
}



//chybí rezimZalevani 2
// vsechny hodnoty natvrdo
// nesviti LED
//predavat vsechna data mezi sebou
//vsude textWidth cestina