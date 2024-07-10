#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN 9
#define SS_PIN  10

byte readCard[4];
byte a = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo; // Create a Servo object

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  lcd.setCursor(2, 0);
  lcd.print("Put your card");
  
  myServo.attach(3); // Attach the servo to pin 3
  myServo.write(0); // Ensure the servo starts at 0 degrees
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanned UID");
  a = 0;
  Serial.println(F("Scanned PICC's UID:"));
  for ( uint8_t i = 0; i < 4; i++) {  
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
    Serial.print(" ");
    lcd.setCursor(a, 1);
    lcd.print(readCard[i], HEX);
    lcd.print(" ");
    delay(500);
    a += 3;
  }
  Serial.println("");

  // Rotate the servo when the card is scanned successfully
  myServo.write(90); // Rotate to 90 degrees
  delay(1000); // Hold the position for 1 second
  myServo.write(0); // Return to 0 degrees

  mfrc522.PICC_HaltA();
}
