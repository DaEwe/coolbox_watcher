#include <DallasTemperature.h>

#include <OneWire.h>

//www.diyusthad.com
#include <LiquidCrystal.h>
#define ONE_WIRE_BUS 9
# define PINOUT 19
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int num_sensors = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool cooling = false;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
void setup() {
  Serial.begin(9600);
  num_sensors = sensors.getDeviceCount();
  Serial.print("num of sensors");
  Serial.println(num_sensors);
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(PINOUT,OUTPUT);
  stopCooling();
}

float getTemperature(){
  sensors.requestTemperatures();
  float temp1 = sensors.getTempCByIndex(0);
  float temp2 = sensors.getTempCByIndex(1);
  float temp3 = sensors.getTempCByIndex(2);
  float avg = (temp1+temp2+temp3)/3;
  // TODO plausability check and shutdown!
  return avg;
 }

void displayTemperature(float temperature){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tmp: ");
  lcd.print(temperature);
  
  lcd.setCursor(0,1);
  if (cooling){
    lcd.print("* * * * * * * *");
  } else {
    lcd.print("~ ~ ~ ~ ~ ~ ~ ~");
  }
}

void startCooling(){
  digitalWrite(PINOUT, LOW);
  
  
}

void stopCooling(){
  digitalWrite(PINOUT, HIGH);
  
}

void loop() {
  float temperature = getTemperature();
  displayTemperature(temperature);
  if (cooling){
    // COOLING STATE: stop if temperature gets too low
    if (temperature < 1){
      cooling = false;
      stopCooling();
    }
  } else {
  // IDLE STATE: start cooling if it gets too warm
    if (temperature > 4){
      cooling = true;
      startCooling();  
    }
  }   
  delay(1000);
}
