#define BLYNK_TEMPLATE_ID "xxxxxxxxxxxxxxx"
#define BLYNK_DEVICE_NAME "xxxxxxxxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxxxxxxxxxxx"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "nagarajan";
char pass[] = "1029384756";
BlynkTimer timer;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float IDtime,RNtime; 
const int voltage_sensor=34;
double Voltage = 0;
float R1=30000.0;
float R2=7500.0; 
float watt = 0;
float bill_amount, t , C_bill = 0,PF=0;   
unsigned int energyTariff = 8.0;
BLYNK_WRITE(V5)
{
  int s0 = param.asInt(); // parameter as int 
  if(s0 == HIGH)
  {    
    digitalWrite(23,HIGH);
    Serial.println("ON");
    delay(10);
    }  
    if(s0 == LOW)
  {
    digitalWrite(23,LOW);
    Serial.println("OFF");
    delay(10);
    }  
}
void setup() 
{
 Serial.begin(9600);
 Blynk.begin(auth, ssid, pass);
 pinMode(16,OUTPUT);
 pinMode(23,OUTPUT);
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print(" SMART DC MOTOR   ");
 lcd.setCursor(0,1);
 lcd.print("       IOT      ");
 delay(3000);
 lcd.clear();
   
}

void loop() 
{
     Blynk.run();
     sensors.requestTemperatures();
     float temp1 = sensors.getTempCByIndex(0);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Temperature:");
        lcd.print(temp1);
        lcd.setCursor(0,1);
        lcd.print("Motor_V:");
        lcd.setCursor(9,1);
        lcd.print(VoltageSensor(voltage_sensor));
        lcd.print("V");
        delay(2000);
        lcd.clear();
        float I = 0.25;
        float P = VoltageSensor(voltage_sensor) * I;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PWR IN W :");
  lcd.setCursor(11,0);
  lcd.print(P);
  watt = P+watt;
  Serial.println(watt);
  lcd.setCursor(0,1);
  lcd.print("T PWR IN W :");
  lcd.setCursor(13,1);
  lcd.print(watt);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IDL TIME:");
  lcd.setCursor(10,0);
  lcd.print(IDtime);
  lcd.setCursor(0,1);
  lcd.print("RUN TIME:");
  lcd.setCursor(10,1);
  lcd.print(RNtime);
  delay(1000);
    t = watt * 24 * 30 ;   
    C_bill = t /1000;
    bill_amount = C_bill * 8.0;
    Serial.println(bill_amount);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TTAL BILL:");
    lcd.setCursor(10,0);
    lcd.print(bill_amount);
     delay(1000);
    if(VoltageSensor(voltage_sensor) <1){
    IDtime+=0.25;
    }
    if(VoltageSensor(voltage_sensor) >4){
    RNtime+=0.25;
    }
        if(temp1>41){
        lcd.clear();
        digitalWrite(16,HIGH);
        lcd.setCursor(0,0);
        lcd.print("ALERT...!!!!");
        lcd.setCursor(0,1);
        lcd.print("MOTOR OVERHEAT..!!!");
        delay(2000);
        digitalWrite(16,LOW);
        lcd.clear();   
        }
        if(temp1<41){
        digitalWrite(16,LOW);
        }
    Blynk.virtualWrite(V0, temp1);
    Blynk.virtualWrite(V1, IDtime);
    Blynk.virtualWrite(V2, RNtime);
    Blynk.virtualWrite(V3, watt);
    Blynk.virtualWrite(V4, bill_amount);
}

float VoltageSensor(const int Channel){
  int value=analogRead(Channel);
  float a=value*3.7;
  float Vout=a/4095.0;
  float Vin=Vout/(R2/(R1+R2));
  return Vin;
}
