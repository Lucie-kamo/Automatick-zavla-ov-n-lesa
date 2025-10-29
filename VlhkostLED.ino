#define LED_ZELENA 30
#define LED_ORANZOVA 32
#define LED_CERVENA 34


const int dry = 563;
const int wet = 199;


void setup() {
  Serial.begin(9600);
  pinMode(LED_ZELENA, OUTPUT);
  pinMode(LED_ORANZOVA, OUTPUT);
  pinMode(LED_CERVENA, OUTPUT);
}

void loop() {
  int sensorVal = analogRead(A0);
  //Serial.println(sensorVal);
  
  int percentageHumidity = map(sensorVal, wet, dry, 100, 0);
  digitalWrite(LED_ORANZOVA, LOW);
  digitalWrite(LED_ZELENA, LOW);
  digitalWrite(LED_CERVENA, LOW);

  if(percentageHumidity >= 30){
    digitalWrite(LED_ZELENA, HIGH);
  }
  else if(/*percentageHumidity < 30 &&*/percentageHumidity >= 5){
    digitalWrite(LED_ORANZOVA, HIGH);
  }
  else if(percentageHumidity < 5){
    digitalWrite(LED_CERVENA, HIGH);
  }

  Serial.print(percentageHumidity);
  Serial.println("%");

  delay(100);
}
