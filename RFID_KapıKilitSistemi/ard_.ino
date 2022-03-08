#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h> 
#include<Servo.h>
#include <dht11.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 5
#define RST_PIN 10
#define LED_G 3 
#define LED_R 4 
#define Buton 9
#define pot A0 

Servo motor;


MFRC522 mfrc522(SS_PIN, RST_PIN);

int buzzer=2;
int DhtPin=1;
int deger;
int frekans;

dht11 dht_sensor;

void setup() 
{
  pinMode(pot,INPUT); 
  int chk = dht_sensor.read(DhtPin);
  pinMode(buzzer,OUTPUT);
  motor.attach(6);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(Buton,INPUT);

  

  Serial.println("Kartınızı yerleştirin");
  Serial.println();

}

void loop() 
{
  deger = analogRead(pot);
  frekans = map(deger,0,1023,30,900);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nem(%):");
  lcd.print((float)dht_sensor.humidity, 1); 
  lcd.setCursor(0,1);
  lcd.print("Derece(C):");
  lcd.print((float)dht_sensor.temperature, 1);
  motor.write(0);

  
  if(digitalRead(Buton) == 1){
    lcd.clear();
    Serial.println("Erişim İzni");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    motor.write(90);  // motor
    tone(buzzer, frekans,250);
    lcd.setCursor(0,1);
    lcd.print("Kilit acik");
   
    delay(3000);
    digitalWrite(LED_G, LOW);
    
    motor.write(0); 
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Kilitli");
  }
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("ID :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mesaj : ");
  content.toUpperCase();


  
 if (content.substring(1) == "39 65 B0 A3") 
  {
    lcd.clear();
    Serial.println("Erisim Izni");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(buzzer, frekans,250);
    motor.write(90);
    
    
    
    lcd.setCursor(0,1);
    lcd.print("Kilit acik");

    delay(3000);

    
    digitalWrite(LED_G, LOW);
    
    motor.write(0);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Kilitli");
  }

else
{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Geçersiz ID");
    Serial.println(" Erişim Yok");
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_R, LOW);
    delay(100);
    digitalWrite(LED_R, HIGH);
    delay(500);
    digitalWrite(LED_R, LOW);
    delay(100);
    digitalWrite(LED_R, HIGH);
    delay(500);
    digitalWrite(LED_R, LOW);
    lcd.setCursor(0,1);
    lcd.print("Kapı kilitli");
 }
 delay (100);
 }
