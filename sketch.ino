#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int trigPin = 9;
const int echoPin = 8;
const int pirPin = 6;
const int ledPin = 13;
const int buzzerPin = 10;

const float tempLimit = 35.0;
const float humLimit = 80.0;
const int distanceLimit = 20;

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(pirPin, INPUT);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  dht.begin();

  lcd.setCursor(0,0);
  lcd.print("Smart Room");

  lcd.setCursor(0,1);
  lcd.print("Security");

  delay(2000);
  lcd.clear();
}

void loop() {

  // ---------- DHT ----------
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // ---------- Ultrasonic ----------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  float distance = duration * 0.0343 / 2;

  // ---------- PIR ----------
  int motion = digitalRead(pirPin);

  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  lcd.clear();

  // ===== Motion Alert =====
  if (motion == HIGH) {

    lcd.setCursor(0,0);
    lcd.print("Motion Found");

    lcd.setCursor(0,1);
    lcd.print("Security Alert");

    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);

  }

  // ===== Object Near =====
  else if (distance > 0 && distance < distanceLimit) {

    lcd.setCursor(0,0);
    lcd.print("Object Near");

    lcd.setCursor(0,1);
    lcd.print(distance);
    lcd.print(" cm");

    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 900);

  }

  // ===== High Temperature =====
  else if (temp > tempLimit) {

    lcd.setCursor(0,0);
    lcd.print("High Temp!");

    lcd.setCursor(0,1);
    lcd.print(temp);
    lcd.print((char)223);
    lcd.print("C");

    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1200);

  }

  // ===== High Humidity =====
  else if (hum > humLimit) {

    lcd.setCursor(0,0);
    lcd.print("Humidity High");

    lcd.setCursor(0,1);
    lcd.print(hum);
    lcd.print("%");

    digitalWrite(ledPin, HIGH);
  }

  // ===== Normal =====
  else {

    lcd.setCursor(0,0);
    lcd.print("T:");
    lcd.print(temp,1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0,1);
    lcd.print("H:");
    lcd.print(hum,0);
    lcd.print("% ");

    lcd.print("D:");
    lcd.print(distance,0);

  }

  delay(1000);
}