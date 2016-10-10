#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include "DHT.h"

// Configuration
int maxTemp = 11;
int minTemp = 9;
int bedTimeHour = 6;
int wakeTimeHour = 7;
int preCoolHour = bedTimeHour - 1;
int preCoolTemp = minTemp - 2;

// Pin & device config
int relayPin = 13;
int tempPin = 12;
DHT dht(tempPin, DHT22);

void setup()  {
  Serial.begin(9600);

  // Configure the real-time clock
  // the function to get the time from the RTC
  setSyncProvider(RTC.get);   
  if(timeStatus()!= timeSet) {
     Serial.println("Unable to sync with the RTC");
  } else {
     Serial.println("RTC has set the system time");      
  }

  // Configure the temp sensor
  dht.begin();

  // Configure the relay
  pinMode(relayPin, OUTPUT);
  turnFridgeOff();
}

void loop()
{
  delay(10000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  displayClock();
  displayTemp();
  
  if (t > maxTemp) {
    turnFridgeOn();
  } else if (t < minTemp) {
    turnFridgeOff();
  }
  Serial.println("\n\n");

}


/* Private methods */

void turnFridgeOn() {
  digitalWrite(relayPin, HIGH);
  Serial.print("Fridge is ON");
  Serial.println("\n");
}

void turnFridgeOff() {
  digitalWrite(relayPin, LOW);
  Serial.print("Fridge is OFF");
  Serial.println("\n");
}

void displayTemp() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
}

// digital clock display of the time
void displayClock() {
  
  if (timeStatus() == timeSet) {
    Serial.print("UTC: ");
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(" ");
    Serial.print(month());
    Serial.print(" ");
    Serial.print(year()); 
    Serial.println();
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(4000);
  }
   
}

// utility function for digital clock display: prints preceding colon and leading 0
void printDigits(int digits) {
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
