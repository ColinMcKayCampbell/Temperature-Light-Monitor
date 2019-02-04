#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

const int tempPin = A0;
const int lightPin = D1;
bool flash = true;
int count = 0;
float avgT;
long avgL = 0;
bool firstTime = true;
float high = 0;
float low = 0;


bool filter(float temp){
  //Serial.print(String(temp) + " " + String(avgT) + "\n");
  
  if(abs(temp) < abs(avgT)){
    if((avgT - temp) > 2){
      return false;
    }
  }
  else if(abs(avgT) <= abs(temp)){
    if((temp - avgT) > 2){
      return false;
    }
  }
  else{
    return true;
  }
}

float getTemp(){
  float temp = analogRead(tempPin);
  temp = ((temp/1024.0)*5.0); // Convert ADC reading to volts
  temp = (temp - 0.5)*100;
  
  if(avgT == NULL){
     count++;
     avgT = temp;
     updateHighLow(temp);
     return temp;
  }
  bool check = filter(temp);
  //Serial.print(check + "\n");
  //if(check){
    count++;
    avgT = avgT + temp;
    updateHighLow(temp);
    return temp;
  //}
}
 


void updateHighLow(float temp){
   if(temp > high){high = temp;}
  else if(temp < low){low = temp;}
}

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  lcd.begin(16,2);
  float tempVal = getTemp();
  low = tempVal;
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}
int getLight(){
  int lightRead = analogRead(lightPin);
  return lightRead;
}
void flashLED(){
  if(flash == true){
  digitalWrite(13, HIGH);
  flash = false;
  }else{
  digitalWrite(13,LOW);
  flash = true;
  }
}

void serialSend(){
  Serial.print(avgT/count);
  Serial.print(",");
  Serial.print(avgL/count);
  avgT = 0;
  avgL = 0;
  count = 0;
}
void loop() {
  float temp = getTemp();
  int lux = getLight();
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String(temp) + "C   H " + String(high));
  lcd.setCursor(0,1);
  if(String(lux).length() == 3){
  lcd.print(String(lux) + "      L " + String(low));
  }
  if(String(lux).length() == 2){
  lcd.print(String(lux) + "       L " + String(low));
  }
  if(String(lux).length() == 1){
  lcd.print(String(lux) + "        L " + String(low));
  }
  
  flashLED();
  
  avgL = avgL + lux;
  
  if(count >59){
    serialSend();
  }
  
  delay(500);
 
}
