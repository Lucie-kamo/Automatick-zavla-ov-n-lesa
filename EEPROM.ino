
/** ------------------
* includovane knihovny
* DOPSAT CO JE JAKY HLAVICKOVY SOUBOR
* knihovna - u8g displej
* knihovna - klavesnice
* knihovna - RTC MYSLIM
------------------ */

#include <Wire.h>
#include <U8glib.h>
#include <Keypad.h>
#include <DS3231.h>
#include <EEPROM.h> //nevim jestli je potreba
#define pinVstup 2 //cidlo vlhkosti
#define LED_ZELENA 3

/** ------------------
* PROMENNE 
------------------*/

///pridat popisek
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  

struct Nastaveni{
  byte prahVlhkosti;
  byte hodZ;
  byte minZ;
  byte secZ;
  byte graf[3];
  unsigned long celkemS; //ale ja budu mit prece, ja pocitam minuty takze
};
Nastaveni nastaveni;
int eeAdresa = 0;
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
unsigned long ted = 0;

String hladina ="";
int prahZalevani = 15;
int hodinZ = 0; //nastaveno hodin zalevani
int minutZ = 1;
int sekundZ = 30;

int pamatujRezim = 0; //pri konci odpoctu se budu chtit vratit sem

//autoZalevani
unsigned long posledniAutoZalevani = 0; // v millis() posledni automaticke spusteni
//const unsigned long intervalAuto = 12UL * 60UL * 60UL * 1000UL; // 12 hodin v ms ///NA TOTO SE JESTE ZEPTAT
const unsigned long intervalAuto = 25000UL; // 5 sekund


///delka zalevani
int hh = 0, mm = 0, ss = 0;
int vyberCas = 0; //0=hh, 1=mm, 2=ss

///rezim 6 - auto-zalevani (podrezimy)
int rezimZalevani = 0; //z celkoveho poctu 4
bool odpocetAktivni = false;
unsigned long casStart = 0;
int pocetDni = 0, pocetHodin = 0;
unsigned long celkemOdpocet = 0;
int s = 0; //pro odpocet


///rezim 7 - statistika (podrezimy)
unsigned long celkemZalevanoSekundy = 0; // celkem se zalévalo

int rezimStatistika = 0;
const int pocetSloupcu = 3;
int zalitoSloupce[pocetSloupcu] = {0,0,0};
//int posledniHodina = -1; // pamatuje si, kdy naposledy proběhl posun
int posledniMesic = -1;
const char* nazvyMesicu[12] = {"led","uno","bre","dub","kve","cvn", ///char* - retezce
                              "cvc","srp","zar","rij","lis","pro"};

/** ------------------
* FUNKCE 
------------------*/

//prvni funkce pro graf at to nehledam
/*
void kontrolaZmenyHodiny(int aktualniHodina){

  if(posledniHodina == -1){
    posledniHodina = aktualniHodina;
    return;
  }

  if(aktualniHodina != posledniHodina){

    // posun dat doleva
    for(int i = 0; i < pocetSloupcu-1; i++){
      zalitoSloupce[i] = zalitoSloupce[i+1];
    }

    // nová aktuální hodina
    zalitoSloupce[pocetSloupcu-1] = 0;

    posledniHodina = aktualniHodina;
  }
}
*/
void kontrolaZmenyMesice(int aktualniMesic){

  if(posledniMesic == -1){
    posledniMesic = aktualniMesic;
    return;
  }

  if(aktualniMesic != posledniMesic){

    // posun dat doleva
    for(int i = 0; i < pocetSloupcu-1; i++){
      zalitoSloupce[i] = zalitoSloupce[i+1];
    }

    // nová aktuální hodina
    zalitoSloupce[pocetSloupcu-1] = 0;

    posledniMesic = aktualniMesic;
  }
}



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
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String velkeCislo = String(procentoVlhkosti) + "%";
    textWidth = u8g.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(velkeCislo);

    u8g.setFont(u8g_font_7x14);
    String text = "vlhkost";
    textWidth = u8g.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(text);
    // rovná čára (od souřadnic -> do souřadnic)
    u8g.drawLine(0,61,128,61);
  }
}

void vykresliHladinu(bool stav){
  //aktualniRezim = 2;
  u8g.firstPage();
  if (stav == LOW) { 
    stavSepnuto = true;
    casAktivace = millis(); //now; - bylo predtim - ja prepisu na ted
  } 
  else { 
    stavSepnuto = false;
  }

  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);

    if(stavSepnuto == true){
      hladina = "ok!";
    }
    else{
      hladina = "nedostatek";
    }//lze optimalizovat

    textWidth = u8g.getStrWidth(hladina.c_str()); 
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);
    u8g.print(hladina);

    u8g.setFont(u8g_font_7x14);
    String text = "hladina v nadrzi";
    textWidth = u8g.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(text);
    // rovná čára (od souřadnic -> do souřadnic)
    u8g.drawLine(0,61,128,61);
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
  u8g.firstPage();

  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String cas = String(doplnNulu(datumCas.hour)) + ":" + String(doplnNulu(datumCas.minute)) + ":" + String(doplnNulu(datumCas.second));
    textWidth = u8g.getStrWidth(cas.c_str());
    int xPos = (128 - textWidth)/2;
    u8g.setPrintPos(xPos, 30);
    u8g.print(cas);

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

    u8g.setFont(u8g_font_7x14);
    String datum = String(datumCas.day) + ". " + mesic + " " + String(datumCas.year);
    textWidth = u8g.getStrWidth(datum.c_str());
    xPos = (128 - textWidth) / 2;     
    u8g.setPrintPos(xPos, 50);      
    u8g.print(datum);
    u8g.drawLine(0,61,128,61);
  }
}

void vykresliKlavesnice(char klavesa){ //nazev je nestastny ale jde o to od kolika procent chci zalevat
  //aktualniRezim = 4;
  //static String zapsano = "?"; ///static String zapsano = "?"; STATIC TAM FAKT MUSI BYT
  static String zapsano = String(nastaveni.prahVlhkosti);
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

  //PRIDANO PRO BUDOUCI UKLADANI DO PAMETI
  else if(klavesa == 'C'){
    if(zapsano != "?"){
      prahZalevani = zapsano.toInt();  // ULOŽENÍ
      nastaveni.prahVlhkosti = (byte)prahZalevani;
      EEPROM.put(eeAdresa, nastaveni);
      vykresliUlozeno();
      delay(2000);
    }
  }


  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    textWidth = u8g.getStrWidth(zapsano.c_str());
    xPos = (128 - textWidth)/2;
    u8g.setPrintPos(xPos, 30);
    u8g.print(zapsano + "%");

    u8g.setFont(u8g_font_7x14);
    String text = "zalevat ?% <= " + String(zapsano);
    textWidth = u8g.getStrWidth(text.c_str());
    xPos = (128 - textWidth) / 2;     
    u8g.setPrintPos(xPos, 50);      
    u8g.print(text);
    u8g.drawLine(0,61,128,61);
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
  else if(klavesa == 'B'){ // přepnutí mezi hh, mm, ss //PUVODNE JSEM MELA C
    vyberCas++;
    if(vyberCas > 2) vyberCas = 0;
  }
  else if(klavesa == 'C'){
    hodinZ = hh;
    minutZ = mm;
    sekundZ = ss;

    nastaveni.hodZ = (byte)hodinZ;
    nastaveni.minZ = (byte)minutZ;
    nastaveni.secZ = (byte)sekundZ;
    EEPROM.put(eeAdresa, nastaveni);

    vykresliUlozeno();
    delay(2000);
  }
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String cas = doplnNulu(hh)+":"+doplnNulu(mm)+":"+doplnNulu(ss);
    int w = u8g.getStrWidth(cas.c_str());
    int x = (128-w)/2;
    u8g.setPrintPos(x,30);
    u8g.print(cas);

    u8g.setFont(u8g_font_7x14);
    String textVypsat="delka zalevani";
    w = u8g.getStrWidth(textVypsat.c_str());
    x = (128-w)/2;
    u8g.setPrintPos(x,50);
    u8g.print(textVypsat);
    u8g.drawLine(0,61,128,61);

    //podtrzeni vybrane pozice
    u8g.drawLine(x+11+vyberCas*27,34,x+11+vyberCas*27+20,34);
  }
}

//rezim 6
void vykresliOzubene(){
  //aktualniRezim = 6;
  //rezimZalevani = 0;
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String nadpisZalevani = "zalevani";
    textWidth = u8g.getStrWidth(nadpisZalevani.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(nadpisZalevani);

    int stredX = 64, stredY = 45;
    int radius = 10;      // menší kruh
    int delkaZubu = 5;    // délka "zubů" ven

    u8g.drawCircle(stredX, stredY, radius);

    for(int i = 0; i < 12; i++){
      float uhel = i * 30 * 3.14159 / 180; // 12 indexů
      int x1 = stredX + radius * cos(uhel);          // začátek zubu na obvodu
      int y1 = stredY + radius * sin(uhel);
      int x2 = stredX + (radius + delkaZubu) * cos(uhel);  // konec zubu ven
      int y2 = stredY + (radius + delkaZubu) * sin(uhel);
      u8g.drawLine(x1, y1, x2, y2);
    }

  }
}

void vykresliZalevatTed(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB10);
    String textTed ="zalevat ted";
    textWidth = u8g.getStrWidth(textTed.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(textTed);
  }
}

void vykresliOdpocet(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    
    if(s < 0){
      s = 0;
    }

    int hh_ = s / 3600;
    int mm_ = (s % 3600) / 60;
    int ss_ = s % 60;
    String cas = doplnNulu(hh_) + ":" + doplnNulu(mm_) + ":" + doplnNulu(ss_);

    textWidth = u8g.getStrWidth(cas.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(cas);


    u8g.setFont(u8g_font_7x14);
    String textZbytek = "jeste zbyva";
    textWidth = u8g.getStrWidth(textZbytek.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(textZbytek);
    // rovná čára (od souřadnic -> do souřadnic)
    u8g.drawLine(0,61,128,61);

  }
}

void vykresliPravidelne(){
  //rezimStatistika = 2;
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB10);
    String textPravidelne ="kolikrat";
    textWidth = u8g.getStrWidth(textPravidelne.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(textPravidelne);
  }
}

//rezim 7 - statistika
void vykresliStatistika(){
  //aktualniRezim = 7;
  //rezimStatistika = 0;
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB10);
    String textStatistika ="Statistika";
    textWidth = u8g.getStrWidth(textStatistika.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(textStatistika);

    //graf
    int left = 50;    // levý okraj čtverce
    int top = 35;     // horní okraj čtverce
    int size = 20;    // velikost čtverce

    // nakreslení čtverce
    u8g.drawFrame(left, top, size, size);

    // body pro stoupající křivku s malými propady
    int body[] = {3, 5, 7, 6, 8, 10, 9, 12, 14, 13, 15, 18, 17, 19, 20};
    int pocetBodu = sizeof(body)/sizeof(body[0]);

    for(int i = 0; i < pocetBodu - 1; i++){
      int x1 = left + i * size / pocetBodu;
      int y1 = top + size - body[i];  // invert Y, protože u8g (0,0 nahoře)
      int x2 = left + (i + 1) * size / pocetBodu;
      int y2 = top + size - body[i+1];
      u8g.drawLine(x1, y1, x2, y2);
    }

    //graf
  }
}

void vykresliGraf() {
  u8g.firstPage();
  while(u8g.nextPage()){
    const int spodniOkraj = 50; // Posunuto výš, aby zbyl prostor na popisky
    const int vyskaMax = 35;    // Maximální výška sloupce
    const int maxZavlazeni = 15; // Kolik zavlažení odpovídá plnému sloupci

    u8g.setFont(u8g_font_6x10);

    // Vodorovné linky (měřítko)
    for(int i = 0; i <= 3; i++){
      int y = spodniOkraj - (i * (vyskaMax / 3));
      u8g.drawLine(20, y, 120, y);
      u8g.setPrintPos(0, y + 3);
      u8g.print(i * 5); // Jednoduché měřítko 0, 3, 6, 9
    }

    // Kreslení sloupců
    for(int i = 0; i < pocetSloupcu; i++){
      int hodnota = zalitoSloupce[i];
      int x = 30 + i * 35; 
      
      // Výpočet výšky (trojčlenka)
      int h = (hodnota * vyskaMax) / maxZavlazeni;
      if(h > vyskaMax) h = vyskaMax; // Limit

      if(h > 0) {
        u8g.drawBox(x, spodniOkraj - h, 20, h);
      }
      
      // Popisky pod sloupci
      int cisloMesice = (datumCas.month - (pocetSloupcu - 1 - i) + 11) % 12; 
      u8g.setPrintPos(x, spodniOkraj + 12);
      u8g.print(nazvyMesicu[cisloMesice]);
    }
  }
}

void vykresliStatMinut(){
  //rezimStatistika = 2;
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    //mozna by stacil jiny datovy typ
    unsigned long celkemMinut = celkemZalevanoSekundy / 60;
    String velkeCislo = String(celkemMinut) + " minut(y)";

    
    textWidth = u8g.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(velkeCislo);

    u8g.setFont(u8g_font_7x14);
    String textMinut = "zalevalo se";
    textWidth = u8g.getStrWidth(textMinut.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(textMinut);
    // rovná čára (od souřadnic -> do souřadnic)
    u8g.drawLine(0,61,128,61);
  }
}

void vykresliStatLitry(){
  //rezimStatistika = 3;
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String velkeCislo = "12L"; //NATVRDO
    textWidth = u8g.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(velkeCislo);

    u8g.setFont(u8g_font_7x14);
    String textLitr = "spotreba vody";
    textWidth = u8g.getStrWidth(textLitr.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(textLitr);
    // rovná čára (od souřadnic -> do souřadnic)
    u8g.drawLine(0,61,128,61);
  }
}

void vykresliUlozeno(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String ulozeno = "ulozeno";
    textWidth = u8g.getStrWidth(ulozeno.c_str()); // šířka textu v pixelech
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(ulozeno);

    u8g.setFont(u8g_font_7x14);
    String jupi = "jupii";
    textWidth = u8g.getStrWidth(jupi.c_str());
    xPos = (128 - textWidth) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(jupi);
    // rovná čára (od souřadnic -> do souřadnic)
    u8g.drawLine(0,61,128,61);
  }
}

void automatickeZalevani(int vlhkost) {
  unsigned long tedMillis = millis();

  // Spust automaticky jen pokud:
  // - vlhkost pod prahem
  // - od posledniho spusteni uplynulo 12h
  // - zavlazovani momentalne nebezi
  if(vlhkost <= prahZalevani && !odpocetAktivni && (tedMillis - posledniAutoZalevani >= intervalAuto)) {
    odpocetAktivni = true;
    casStart = tedMillis;           // start odpočtu
    digitalWrite(LED_ZELENA, HIGH); // LED svítí při zavlažování
    posledniAutoZalevani = tedMillis; // ulozit cas spusteni
    
    zalitoSloupce[pocetSloupcu-1]++; //PRIDANO KVULI GRAFU
  }

  // pokud zavlažování běží, počítáme zbylý čas
  if(odpocetAktivni) {
    celkemOdpocet = hodinZ*3600 + minutZ*60 + sekundZ; 
    s = celkemOdpocet - (tedMillis - casStart)/1000;

    static unsigned long posledniUpdate = 0; //ani nevim proc static
    if (tedMillis - posledniUpdate >= 1000) {
        celkemZalevanoSekundy++;
        posledniUpdate = tedMillis;
    }

    // **vykreslení odpočtu na displej**
    vykresliOdpocet();

    // konec zavlažování
    if(s <= 0) { 
      odpocetAktivni = false;
      digitalWrite(LED_ZELENA, LOW); 
      s = 0;
      nastaveni.graf[2] = (byte)zalitoSloupce[2];
      nastaveni.celkemS = celkemZalevanoSekundy;
      EEPROM.put(eeAdresa, nastaveni);
    }
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
  digitalWrite(LED_ZELENA, LOW);

  
  for(int i = 0; i < pocetSloupcu; i++) {
    zalitoSloupce[i] = 0;
  }
  

  EEPROM.get(eeAdresa, nastaveni);

  // Kontrola, zda je EEPROM prázdná (čistá EEPROM vrací 255)
  if (nastaveni.prahVlhkosti > 100) { 
    // Nastavit tovární hodnoty, pokud je paměť prázdná
    nastaveni.prahVlhkosti = 15;
    nastaveni.hodZ = 0; nastaveni.minZ = 1; nastaveni.secZ = 30;
    nastaveni.celkemS = 0;
    for(int i=0; i<3; i++) nastaveni.graf[i] = 0;
    EEPROM.put(eeAdresa, nastaveni);
  }

  // Překopírování z EEPROM do prac proměnných
  prahZalevani = nastaveni.prahVlhkosti;
  /*hodinZ = nastaveni.hodZ;
  minutZ = nastaveni.minZ;
  sekundZ = nastaveni.secZ;*/
  hh = nastaveni.hodZ; 
  mm = nastaveni.minZ; 
  ss = nastaveni.secZ;
  
  // Pro zobrazení v odpočtu
  hodinZ = hh;
  minutZ = mm;
  sekundZ = ss;
  celkemZalevanoSekundy = nastaveni.celkemS;
  for(int i=0; i<3; i++) zalitoSloupce[i] = nastaveni.graf[i];
}

void loop() {
  datumCas = rtc.getDateTime();
  //kontrolaZmenyHodiny(datumCas.minute); //pridano kvuli grafu!!
  kontrolaZmenyMesice(datumCas.month);
  ted = millis(); //mela jsem psano now
  char klavesa = klavesnice.getKey();
  ///tady ty sensor val a tak jeste prepsat do cestiny
  int sensorVal = analogRead(A0);  
  int procentoVlhkosti = map(sensorVal, vlhko, sucho, 100, 0);
  //nevim kam dat
  bool stav = digitalRead(pinVstup); 
  
  automatickeZalevani(procentoVlhkosti);




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
   
    odpocetAktivni = true;
    casStart = millis();
    digitalWrite(LED_ZELENA, HIGH); //nevim jestli musi bejt
    zalitoSloupce[pocetSloupcu-1]++; //PRIDANO KVULI GRAFU

    //pamatovani rezimu
    pamatujRezim = aktualniRezim;
    aktualniRezim = -1;
  }

  if(klavesa == 'D' && odpocetAktivni){
    odpocetAktivni = false;
    digitalWrite(LED_ZELENA, LOW);
  }

  

  if(aktualniRezim == 4){
    vykresliKlavesnice(klavesa);
  }
  else if (aktualniRezim == 5){
    vykresliCasZalevani(klavesa);
  }

  if(ted - posledniVypis >= 1000){
    posledniVypis = ted;

    

    if(odpocetAktivni){
    vykresliOdpocet();
}

    else{

    
    switch(aktualniRezim){
      case 0: vykresli(); break;
      case 1: vykresliVlhkost(procentoVlhkosti); break;
      case 2: vykresliHladinu(stav); break;
      case 3: vykresliDatumCas(datumCas); break;
      case 6: 
              if(odpocetAktivni){
                vykresliOdpocet();
                celkemZalevanoSekundy++;
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
}



//chybí rezimZalevani 2
// vsechny hodnoty natvrdo
// nesviti LED
//predavat vsechna data mezi sebou
//vsude textWidth cestina