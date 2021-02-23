#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define I2C_ADDR 0x27 // กำหนดตำแหน่ง Address ของ I2C
#define BACKLIGHT_PIN 3
#define SDA_DIO 9
#define RESET_DIO 8
RFID RFID(SDA_DIO, RESET_DIO);

LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);
Servo DoorIn;
Servo DoorOut;

int infr_in = 0;
int infr_out = 0;
int buzzer = 6;
int ledR = 40;
int ledG = 41;

String data[4][4] = {
  {"4BE59399A4","1610900266","1","0"},
  {"7BA1C79588","1610900498","1","0"},
  {"5919358EFB","1580901344","1","0"},
  {"16D0B5F88B","1234567890","0","0"}
};
String input;
int count_parking = 2;
void setup() {
  lcd.begin(16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  lcd.setCursor(5,0);
  lcd.print("Welcome");
  lcd.home();
  lcd.setCursor(2,1);
  lcd.print("Free Space");
  lcd.setCursor(13,1);
  lcd.print(count_parking);
  
  SPI.begin();
  RFID.init();

  DoorIn.attach(2);
  DoorOut.attach(3);
  DoorIn.write(0);
  DoorOut.write(0);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);

  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  digitalWrite(ledR, HIGH);
  digitalWrite(ledG, LOW);
  Serial.begin(115200);
  
}

void loop() {

  infr_in = digitalRead(4);
  infr_out = digitalRead(5);
  if(infr_in == 0 || infr_out == 0)
  {
    if(RFID.isCard()){
    if(RFID.readCardSerial()){
      String strID = "";
      for (byte i = 0; i < 5; i++) {
        strID +=
          String(RFID.serNum[i],HEX);
      }
      strID.toUpperCase();
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(500);

      for(int i=0;i<5;i++)
      {
        if(strID.indexOf(data[i][0])>=0)
        {
          
          if(data[i][2] == "1"){
            lcd.clear();
            lcd.home();
            lcd.setCursor(5,0);
            lcd.print("Status");
            lcd.setCursor(4,1);
            lcd.print("Waiting..");
            delay(1500);
            lcd.clear();
            lcd.home();
            lcd.setCursor(2,0);
            lcd.print("Your ID Card");
            lcd.setCursor(3,1);
            lcd.print(strID);
            delay(2000);
            
            lcd.clear();
            lcd.home();
            lcd.setCursor(3,0);
            lcd.print("Student ID");
            lcd.setCursor(3,1);
            lcd.print(data[i][1]);
            delay(2000);
            lcd.clear();
            lcd.home();
            lcd.setCursor(5,0);
            lcd.print("Status");
            lcd.setCursor(3,1);
            lcd.print("Success ^U^");
            
            if(data[i][3] == "0")
            {
              if(count_parking <= 0)
              {
                lcd.clear();
                lcd.home();
                lcd.setCursor(5,0);
                lcd.print("Status");
                lcd.setCursor(5,1);
                lcd.print("Full!!");
                digitalWrite(ledR, HIGH);
                digitalWrite(buzzer, LOW);
                delay(50);
                digitalWrite(ledR, LOW);
                digitalWrite(buzzer, HIGH);
                delay(50);
                digitalWrite(ledR, HIGH);
                digitalWrite(buzzer, LOW);
                delay(50);
                digitalWrite(buzzer, HIGH);
                delay(2000);
                lcd.clear();
                lcd.home();
                lcd.setCursor(5,0);
                lcd.print("Welcome");
                lcd.setCursor(2,1);
                lcd.print("Free Space");
                lcd.setCursor(13,1);
                lcd.print(count_parking);
                return;
              }else{
                lcd.clear();
                lcd.home();
                lcd.setCursor(5,0);
                lcd.print("Status");
                lcd.setCursor(0,1);
                lcd.print("Door is Opening.");
                data[i][3] = "1";
                count_parking = count_parking -1;
                digitalWrite(ledR, LOW);
                digitalWrite(ledG, HIGH);
                while(infr_in == 0){
                  DoorIn.write(90);
                  infr_in = digitalRead(4);
                }
                
                
                delay(2000);
                digitalWrite(ledR, HIGH);
                digitalWrite(ledG, LOW);
                DoorIn.write(0);
                lcd.clear();
                lcd.home();
                lcd.setCursor(5,0);
                lcd.print("Welcome");
                lcd.setCursor(2,1);
                lcd.print("Free Space");
                lcd.setCursor(13,1);
                lcd.print(count_parking);
                return;
              }
            }else{
              
              lcd.clear();
              lcd.home();
              lcd.setCursor(5,0);
              lcd.print("Status");
              lcd.setCursor(0,1);
              lcd.print("Door is Opening.");
              data[i][3] = "0";
              count_parking = count_parking + 1;
              digitalWrite(ledR, LOW);
              digitalWrite(ledG, HIGH);
              while(infr_out == 0){
                  DoorOut.write(90);
                  infr_out = digitalRead(5);
               }
              delay(2000);
              digitalWrite(ledR, HIGH);
              digitalWrite(ledG, LOW);
              DoorOut.write(0);
              lcd.clear();
              lcd.home();
              lcd.setCursor(5,0);
              lcd.print("Welcome");
              lcd.setCursor(2,1);
              lcd.print("Free Space");
              lcd.setCursor(13,1);
              lcd.print(count_parking);
              return;
            }
          }else{
            lcd.clear();
            lcd.home();
            lcd.setCursor(5,0);
            lcd.print("Status");
            lcd.setCursor(4,1);
            lcd.print("Waiting..");
            delay(1500);
            lcd.clear();
            lcd.home();
            lcd.setCursor(5,0);
            lcd.print("Status");
            lcd.setCursor(2,1);
            lcd.print("Can't Pass!!");
            digitalWrite(ledR, HIGH);
            digitalWrite(buzzer, LOW);
            delay(50);
            digitalWrite(ledR, LOW);
            digitalWrite(buzzer, HIGH);
            delay(50);
            digitalWrite(ledR, HIGH);
            digitalWrite(buzzer, LOW);
            delay(50);
            digitalWrite(buzzer, HIGH);
            delay(1500);
            lcd.clear();
            lcd.home();
            lcd.setCursor(5,0);
            lcd.print("Welcome");
            lcd.setCursor(2,1);
            lcd.print("Free Space");
            lcd.setCursor(13,1);
            lcd.print(count_parking);
            return;
          }
        }
      }
          
    }
  }
  }
  
}
 
