#include <Wire.h>
#include <U8glib.h>
#include <Keypad.h>
#include <DS3231.h>
#include <EEPROM.h> 
#define pinVstup 2      //cidlo vlhkosti
#define LED_ZELENA 3

/** ------------------
*
* PROMENNE 
*
------------------*/

///inicializace displeje pres I2C
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  

struct Nastaveni{
  byte prahVlhkosti;
  byte hodZ;
  byte minZ;
  byte secZ;
  byte graf[3];
  unsigned long celkemS; 
};
Nastaveni nastaveni;
int eeAdresa = 0;

/**
* uvodni bitmapa 128x64 ulozena v PROGMEM
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
unsigned long casAktivace = 0;


/// rezimy
int aktualniRezim = 0; 
unsigned long posledniVypis = 0; 

///text
int xPos = 0;
int textSirka = 0;

///plovak
bool stavSepnuto = false;
String hladina ="";

unsigned long ted = 0;

//nastaveno od uzivatele
int prahZalevani = 15;
int hodinZ = 0; 
int minutZ = 1;
int sekundZ = 30;

int pamatujRezim = 0; //pri konci odpoctu se budu chtit vratit sem

//autoZalevani
unsigned long posledniAutoZalevani = 0; // v millis() posledni automaticke spusteni
//const unsigned long intervalAuto = 12UL * 60UL * 60UL * 1000UL; // 12 hodin v ms 
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
const char* nazvyMesicu[12] = {"led","uno","bre","dub","kve","cvn", 
                              "cvc","srp","zar","rij","lis","pro"};

/** ------------------
*
* FUNKCE 
*
------------------*/

///kontrola zmeny mesice a aktualizace mesicni statistiky zavlazovani
///funkce sleduje zda doslo ke zmene aktualniho mesice oproti naposledy ulozenemu mesici
///aktualniMesic - aktualni mesic 1-12 nacteny z RTC modulu
///pokud doslo ke zmene mesice, posunou se hodnoty zavlazovani v poli zalitoSloupce doleva
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

/// funkce pro vykreslneni uvodni bitmapy
/// parametry zadne, nevraci nic
void vykresli()
{
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.drawBitmapP(0, 0, 16, 64, uvodni); // 128/8 = 16 sloupců po 8 pixelech
  }
}

///funkce pro vykresleni aktualni namerene vlhkosti
///parametr - int procentoVlhkosti - procento, ktere budu zobrazovat na displeji
void vykresliVlhkost(int procentoVlhkosti){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String velkeCislo = String(procentoVlhkosti) + "%";
    textSirka = u8g.getStrWidth(velkeCislo.c_str()); // šířka textu v pixelech
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(velkeCislo);

    u8g.setFont(u8g_font_7x14);
    String text = "vlhkost";
    textSirka = u8g.getStrWidth(text.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(text);
    // rovná čára (od souřadnic - do souřadnic)
    u8g.drawLine(0,61,128,61);
  }
}

///funkce pro vykreslovani, zda je dost vody v nadrzi
///stav - aktualni logicka hodnota ze vstupu čidla hladiny (LOW - dostatek, HIGH - nedostatek)
///funkce nic nevrací (void)
void vykresliHladinu(bool stav){
  u8g.firstPage();
  if (stav == LOW) { 
    stavSepnuto = true;
    casAktivace = millis(); 
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

    textSirka = u8g.getStrWidth(hladina.c_str()); 
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 30);
    u8g.print(hladina);

    u8g.setFont(u8g_font_7x14);
    String text = "hladina v nadrzi";
    textSirka = u8g.getStrWidth(text.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(text);
    // rovná čára (od souřadnic -> do souřadnic)
    u8g.drawLine(0,61,128,61);
  }
}

///doplní pocatecni nulu pred jednociferne cislo
///cislo - cele cislo urcene k preformatovani
///return String - cislo prevedene na String, pripadne s doplnenou nulou
String doplnNulu(int cislo) {
  if (cislo < 10){
    return "0" + String(cislo);
  } 
  else {
    return String(cislo);
  }
}

///vykresli aktualni datum a cas na displeji 
///datumCas - struktura obsahujici aktualni datum a cas, ziskana z RTC modulu
///funkce nic nevraci (void)
///vyuziva pomocnou funkci doplnNulu()
void vykresliDatumCas(RTCDateTime datumCas){
  u8g.firstPage();

  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String cas = String(doplnNulu(datumCas.hour)) + ":" + String(doplnNulu(datumCas.minute)) + ":" + String(doplnNulu(datumCas.second));
    textSirka = u8g.getStrWidth(cas.c_str());
    int xPos = (128 - textSirka)/2;
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
    textSirka = u8g.getStrWidth(datum.c_str());
    xPos = (128 - textSirka) / 2;     
    u8g.setPrintPos(xPos, 50);      
    u8g.print(datum);
    u8g.drawLine(0,61,128,61);
  }
}


///zpracuje vstup z klavesnice a nastavi prah vlhkosti pro zavlazovani
///ovladaci klavesy:
/// - '0'-'9': zadavani cisel
/// - 'D': vymazani aktualni hodnoty
/// - 'C': potvrzeni a ulozeni hodnoty do EEPROM (po potvrzeni se zobrazi hlaska o uspesnem ulozeni)
///
///klavesa - znak stisknute klavesy
///funkce nic nevraci (void)
void vykresliKlavesnice(char klavesa){ 
  static String zapsano = String(nastaveni.prahVlhkosti); ///static - pamatuje si hodnotu mezi volanimi funkce
  if(klavesa >= '0' && klavesa <= '9'){
    if(zapsano == "?"){
      zapsano = "";
    }
    int cislo = zapsano.toInt();
    if(zapsano.length() < 3 && cislo < 10){ 
      zapsano += klavesa;
      int cislo = zapsano.toInt();
    }
    else{
      zapsano = String(klavesa);
    }
  }
  else if(klavesa == 'D'){ 
    zapsano = "?";
  }

  else if(klavesa == 'C'){
    if(zapsano != "?"){
      prahZalevani = zapsano.toInt();  
      nastaveni.prahVlhkosti = (byte)prahZalevani;
      EEPROM.put(eeAdresa, nastaveni);
      vykresliUlozeno();
      delay(2000);
    }
  }


  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    textSirka = u8g.getStrWidth(zapsano.c_str());
    xPos = (128 - textSirka)/2;
    u8g.setPrintPos(xPos, 30);
    u8g.print(zapsano + "%");

    u8g.setFont(u8g_font_7x14);
    String text = "zalevat ?% <= " + String(zapsano);
    textSirka = u8g.getStrWidth(text.c_str());
    xPos = (128 - textSirka) / 2;     
    u8g.setPrintPos(xPos, 50);      
    u8g.print(text);
    u8g.drawLine(0,61,128,61);
  }
}

///nastaveni delky zavlazovani
///ovladani:
/// - '0'-'9': zadavani cislic pro cas
/// - 'B': prepinani mezi minutami, sekundami, hodinami
/// - 'C': potvrzeni a ulozeni do EEPROM
///
///na displeji je práve nastavovana slozka podtrzena
///klavesa - znak predstavujici stisknutou klavesu
///funkce nic nevraci (void)
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
  else if(klavesa == 'B'){ // přepnutí mezi hh, mm, ss 
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

///vykresli obrazovku s nadpisem "zalevani" a ikonou ozubeneho kola
///funkce nic nevraci (void)
void vykresliOzubene(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String nadpisZalevani = "zalevani";
    textSirka = u8g.getStrWidth(nadpisZalevani.c_str());
    xPos = (128 - textSirka) / 2;  
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

///zobrazi obrazovku s textem "zalevat ted"
///funkce nic nevraci (void)
void vykresliZalevatTed(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB10);
    String textTed ="zalevat ted";
    textSirka = u8g.getStrWidth(textTed.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(textTed);
  }
}

///vykresli odpocet, ktery se spusti po zapnuti zavlazovani
///vysledny cas je formatovany take za pomoci doplnNulu
///funkce nevraci nic (void)
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

    textSirka = u8g.getStrWidth(cas.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(cas);


    u8g.setFont(u8g_font_7x14);
    String textZbytek = "jeste zbyva";
    textSirka = u8g.getStrWidth(textZbytek.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 50);      // pod číslem
    u8g.print(textZbytek);
    // rovná čára (od souřadnic - do souřadnic)
    u8g.drawLine(0,61,128,61);

  }
}


///Zobrazi napis "statistika" a pod tim vykresli graf 
///(uvodni vykresleni v rezimu statistika)
void vykresliStatistika(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB10);
    String textStatistika ="Statistika";
    textSirka = u8g.getStrWidth(textStatistika.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 30);      // vertikální pozice
    u8g.print(textStatistika);

    //graf
    int left = 50;    
    int top = 35;     
    int size = 20;    

    // nakreslení čtverce
    u8g.drawFrame(left, top, size, size);

    // body pro stoupající křivku s malými propady
    int body[] = {3, 5, 7, 6, 8, 10, 9, 12, 14, 13, 15, 18, 17, 19, 20};
    int pocetBodu = sizeof(body)/sizeof(body[0]);

    for(int i = 0; i < pocetBodu - 1; i++){
      int x1 = left + i * size / pocetBodu;
      int y1 = top + size - body[i];  
      int x2 = left + (i + 1) * size / pocetBodu;
      int y2 = top + size - body[i+1];
      u8g.drawLine(x1, y1, x2, y2);
    }
  }
}

///funkce vykresli sloupcovy graf zavlazovani na displej
///zobrazuje pocty zavlazovani v poslednich trech mesicich
///konstanty:
/// - 'spodniOkraj' - Y-pozice spodni casti grafu
/// - 'vyskaMax' - maximalni vyska sloupce (v pixelech)
/// - 'maxZalevani' - maximalni hodnota poctu zavlazovani 
///funkce nic nevraci (void)
void vykresliGraf() {
  u8g.firstPage();
  while(u8g.nextPage()){
    const int spodniOkraj = 50; 
    const int vyskaMax = 35;    
    const int maxZavlazeni = 15; 

    u8g.setFont(u8g_font_6x10);

    // vodorovné linky 
    for(int i = 0; i <= 3; i++){
      int y = spodniOkraj - (i * (vyskaMax / 3));
      u8g.drawLine(20, y, 120, y);
      u8g.setPrintPos(0, y + 3);
      u8g.print(i * 5); // měřítko 0, 5, 10, 15
    }

    // kreslení sloupců
    for(int i = 0; i < pocetSloupcu; i++){
      int hodnota = zalitoSloupce[i];
      int x = 30 + i * 35; 
      
      // výpočet výšky 
      int h = (hodnota * vyskaMax) / maxZavlazeni;
      if(h > vyskaMax) h = vyskaMax; // limit

      if(h > 0) {
        u8g.drawBox(x, spodniOkraj - h, 20, h);
      }
      
      // popisky pod sloupci
      int cisloMesice = (datumCas.month - (pocetSloupcu - 1 - i) + 11) % 12; 
      u8g.setPrintPos(x, spodniOkraj + 12);
      u8g.print(nazvyMesicu[cisloMesice]);
    }
  }
}

///vykresli celkovou dobu zavlazovani v minutach na displeji
///pouziva globalni promennou 'celkemZalevanoSekund' - celkovy cas zavlazovani v sekundach
///funkce nic nevraci (void)
void vykresliStatMinut(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    unsigned long celkemMinut = celkemZalevanoSekundy / 60;
    String velkeCislo = String(celkemMinut) + " minut(y)";

    
    textSirka = u8g.getStrWidth(velkeCislo.c_str()); 
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 30);      
    u8g.print(velkeCislo);

    u8g.setFont(u8g_font_7x14);
    String textMinut = "zalevalo se";
    textSirka = u8g.getStrWidth(textMinut.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 50);      
    u8g.print(textMinut);
    u8g.drawLine(0,61,128,61);
  }
}

///vykresli zpravu potvrzujici uspesne ulozeni
///jde o zpetnou vazbu uzivateli
///funkce nic nevraci (void)
void vykresliUlozeno(){
  u8g.firstPage();
  while(u8g.nextPage()){
    u8g.setFont(u8g_font_helvB14);
    String ulozeno = "ulozeno";
    textSirka = u8g.getStrWidth(ulozeno.c_str()); 
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 30);      
    u8g.print(ulozeno);

    u8g.setFont(u8g_font_7x14);
    String jupi = "jupii";
    textSirka = u8g.getStrWidth(jupi.c_str());
    xPos = (128 - textSirka) / 2;  
    u8g.setPrintPos(xPos, 50);      
    u8g.print(jupi);
    u8g.drawLine(0,61,128,61);
  }
}

///automaticky spousti zavlazovani na zaklade vlhkosti a casoveho intervalu
///pri spusteni se rozsviti LED (pouze ilustracni, pri skutecnem vyuziti by se spustilo cerpadlo)
///po ukonceni vypne LED, uklada do EEPROM
///vlhkost - aktualni vlhkost je porovnavana s prahem 
///funkce nic nevraci (void)
void automatickeZalevani(int vlhkost) {
  unsigned long tedMillis = millis();

  // spusti automaticky jen pokud:
  // - vlhkost pod prahem
  // - od posledniho spusteni uplynulo 12h
  // - zavlazovani momentalne nebezi
  if(vlhkost <= prahZalevani && !odpocetAktivni && (tedMillis - posledniAutoZalevani >= intervalAuto)) {
    odpocetAktivni = true;
    casStart = tedMillis;           // start odpočtu
    digitalWrite(LED_ZELENA, HIGH); // LED sviti při zavlazovani
    posledniAutoZalevani = tedMillis; // ulozit cas spusteni
    
    zalitoSloupce[pocetSloupcu-1]++; 
  }

  // pokud zavlažování běží, pocita zbylý čas
  if(odpocetAktivni) {
    celkemOdpocet = hodinZ*3600 + minutZ*60 + sekundZ; 
    s = celkemOdpocet - (tedMillis - casStart)/1000;

    static unsigned long posledniUpdate = 0; 
    if (tedMillis - posledniUpdate >= 1000) {
        celkemZalevanoSekundy++;
        posledniUpdate = tedMillis;
    }

    // vykresleni odpoctu na displej
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



///inicializace a nastaveni vychozich hodnot
///inicializace seriove komunikace (serial.begin(9600))
///konfigurace vstupu a vystupu (pinVstup, LED_ZELENA)
///inicializace RTC modulu 
///inicializace poli a promennych
///nastaveni a nacteni z eeprom
///prekopirovani nastaveni do pomocnych promennych 
void setup() {
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

  // Kontrola, zda je EEPROM prázdná 
  if (nastaveni.prahVlhkosti > 100) { 
    // nastavit tyto hodnoty, pokud je paměť prázdná
    nastaveni.prahVlhkosti = 15;
    nastaveni.hodZ = 0; nastaveni.minZ = 1; nastaveni.secZ = 30;
    nastaveni.celkemS = 0;
    for(int i=0; i<3; i++) nastaveni.graf[i] = 0;
    EEPROM.put(eeAdresa, nastaveni);
  }
  
  prahZalevani = nastaveni.prahVlhkosti;
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


///hlavni smycka programu
///po spusteni se nepretrzite opakuje a provadi:
/// - aktualizace casu
/// - cteni vstupu (detekuje stisk klavesy, cteni hodnoty senzoru vlhkosti)
/// - volani automatickeZavlazovani
/// - ovladani:
///   - '*' a '#' - prepinani mezi rezimy
///   - 'A' a 'B' - zmena podrezimu
///   - 'C' - spusti rucni zalevani v rezimu zalevat ted
///   - 'D' - okamzite ukoncuje zavlazovani pokud je aktivni
///
/// - aktualizace stavu pameti
/// - zobrazeni na displeji
void loop() {
  datumCas = rtc.getDateTime();
  kontrolaZmenyMesice(datumCas.month);
  ted = millis(); 
  char klavesa = klavesnice.getKey();
  int senzorHodnota = analogRead(A0);  
  int procentoVlhkosti = map(senzorHodnota, vlhko, sucho, 100, 0);
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
                  rezimZalevani = 1;
                }
              }
              else if(aktualniRezim == 7){
                rezimStatistika--;
                if(rezimStatistika < 0){
                  rezimStatistika = 2;
                }
              }
              break;
    case 'B': if(aktualniRezim == 6){
                rezimZalevani++;
                if(rezimZalevani > 1){
                  rezimZalevani = 0;
                }
              }
              else if(aktualniRezim == 7){
                rezimStatistika++;
                if(rezimStatistika > 2){
                  rezimStatistika = 0;
                }
              }
              break;
  }

  if(aktualniRezim == 6 && rezimZalevani == 1 && klavesa == 'C' && !odpocetAktivni){
   
    odpocetAktivni = true;
    casStart = millis();
    digitalWrite(LED_ZELENA, HIGH); 
    zalitoSloupce[pocetSloupcu-1]++; 

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
        case 6:{
          if(odpocetAktivni){
                  vykresliOdpocet();
                  celkemZalevanoSekundy++;
                }
                else{
                  switch(rezimZalevani){
                    case 0: vykresliOzubene();break;
                    case 1: vykresliZalevatTed(); /*musi cekat na stisk klavesy*/ break;
                  }
          }
        } break; 
                
        case 7: {
          switch(rezimStatistika){
                  case 0: vykresliStatistika(); break;
                  case 1: vykresliGraf(); break;
                  case 2: vykresliStatMinut(); break;
          }
        } break;
      }
    }
  }
}
