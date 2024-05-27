#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

RTC_DS3231 rtc;
Servo myservo;

const int feedHour = 22; // Feeding hour
const int feedMinute = 59; // Feeding minute

bool hasFedToday = false;
int feedCounter = 0; // Counter to track servo operations

void setup() {
  Wire.begin();
  Serial.begin(57600);
  myservo.attach(9); // Attaches the servo on pin 9 to the servo object

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // The following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Ensure the servo is at the initial position (0 degrees)
  myservo.write(0);
  delay(1000); // Allow servo to reach the initial position
}

void loop() {
  DateTime now = rtc.now();

  // Print current date and time to Serial Monitor
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // Check if it's time to feed the fish
  if (now.hour() == feedHour && now.minute() == feedMinute && !hasFedToday) {
    Serial.println("Feeding time!");
    feedFish();
    hasFedToday = true;
  }

  // Reset hasFedToday at midnight
  if (now.hour() == 0 && now.minute() == 0 && now.second() == 0) {
    hasFedToday = false;
  }

  delay(1000);
}

void feedFish() {
  feedCounter = 0;

  while (feedCounter < 1) {
    // Move servo to 35 degrees
    for (int pos = 0; pos <= 40; pos += 1) {
      myservo.write(pos);
      delay(20);
    }

    // Move servo back to 0 degrees
    for (int pos = 40; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(20);
    }

    feedCounter++;
    delay(1000); // Delay between the two feeding operations
  }
}
