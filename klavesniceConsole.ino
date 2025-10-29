#include <Keypad.h>


const byte radky= 4;
const byte sloupce = 4;

char klavesy[radky][sloupce] = {
  {'1', '2', '3', 'A'},           //A - inkrementovat
  {'4', '5', '6', 'B'},           //B - dekrementovat
  {'7', '8', '9', 'C'},           //C - ok/ulozit
  {'*', '0', '#', 'D'}            //* - previous, # - next, D - zpět/neukládat/cancel
};

byte pinyRadku[radky] = {48, 46, 44, 42};
byte pinySloupcu[sloupce] = {40, 38, 36, 34};

Keypad klavesnice = Keypad(makeKeymap(klavesy), pinyRadku, pinySloupcu, radky, sloupce);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char klavesa = klavesnice.getKey();
  if(klavesa){
    Serial.print("stisknuta klavesa: ");
    Serial.println(klavesa);
  }
}
