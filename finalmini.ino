#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Modules
MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// GPS and GSM
SoftwareSerial gpsSerial(4, 3);   // GPS (RX, TX)
SoftwareSerial gsm(10, 11);       // GSM (RX, TX)

TinyGPSPlus gps;

// Pins
int buzzer = 8;
int button = 9;

// Flags
bool accident = false;
bool alertSent = false;

// Timing
unsigned long accidentTime = 0;
const unsigned long cancelWindow = 30000;

// GPS data
String latitude = "0.0";
String longitude = "0.0";

void setup() {
  Wire.begin();
  Serial.begin(9600);

  gpsSerial.begin(9600);
  gsm.begin(9600);

  mpu.initialize();

  // Check MPU
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 FAILED");
  } else {
    Serial.println("MPU6050 OK");
  }

  lcd.init();
  lcd.backlight();

  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  lcd.setCursor(0,0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {

  // 📡 GPS Reading
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      latitude = String(gps.location.lat(), 6);
      longitude = String(gps.location.lng(), 6);
      Serial.print("Lat: ");
      Serial.println(latitude);

      Serial.print("Lng: ");
      Serial.println(longitude);
      Serial.println("----------------");

    }
  }

  // 📊 MPU Data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float A = sqrt((float)ax*ax + (float)ay*ay + (float)az*az) / 16384.0;
  float shock = abs(A - 1.0);

  Serial.print("Shock: ");
  Serial.println(shock);

  // 🚨 Accident Detection
  if (shock > 1.5 && !accident) {
    accident = true;
    alertSent = false;
    accidentTime = millis();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Accident");
    lcd.setCursor(0,1);
    lcd.print("Detected!!");

    tone(buzzer, 1000);
  }

  if (accident) {

    // ❌ Cancel option
    if (digitalRead(button) == LOW &&
        millis() - accidentTime <= cancelWindow) {

      noTone(buzzer);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("False Alarm");
      lcd.setCursor(0,1);
      lcd.print("Cancelled");
      delay(3000);
      lcd.clear();

      accident = false;
      alertSent = false;
    }

    // 📩 Send SMS
    if (!alertSent && millis() - accidentTime > cancelWindow) {

      lcd.clear();
      lcd.print("Sending Alert");
      delay(1000);
      sendSMS();

      alertSent = true;
    }

    // 🔁 Reset system
    if (alertSent && millis() - accidentTime > (cancelWindow + 5000)) {

      noTone(buzzer);

      accident = false;
      alertSent = false;

      lcd.clear();
      lcd.print("Resetting...");
      delay(2000);
      lcd.clear();
    }
  }
}

// 📱 GSM SMS FUNCTION
void sendSMS() {

  gsm.println("AT+CMGF=1");
  delay(1000);

  gsm.println("AT+CMGS=\"+917012864775\"");
delay(2000); // wait for '>' prompt
gsm.println("Accident Detected!");
gsm.print("Lat: "); gsm.println(latitude);
gsm.print("Lng: "); gsm.println(longitude);
gsm.print("https://maps.google.com/?q="); gsm.print(latitude); gsm.print(","); gsm.println(longitude);
gsm.write(26); // Ctrl+Z
}