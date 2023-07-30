// Temperature and Humidity Sensor Libray
#include <AM232X.h>

// OLED Screen Library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Real time Data Transferring Library
#include "HackPublisher.h"

// OLED Screen Setup
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RGB ring
#include <Adafruit_NeoPixel.h>
#define PIN 14
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Temperature and Humidity Sensor Libray Object
AM232X am2320;

// Pins Setup
const int gasSensorPin = A3; // Analog input pin for the MQ-135 Gas Sensor
const int trigPin = 13;       // Digital output pin for Ultrasonic Trigger
const int echoPin = 12;       // Digital input pin for HC-SR04 Echo
const int buzzerPin = 27;     // Digital output pin for the Buzzer
const int greenLED = 15;     // Digital output pin for the green LED
const int redLED = 33;       // Digital output pin for the red LED
const int yellowLED = 32;    // Digital output pin for the red LED

// WIFI for ESP32
const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";

HackPublisher publisher("AstroGears"); // publisher instance for team "AstroGears"

int gasLevel =0;
float distance =0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) continue;

  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize publisher
  publisher.begin();

  // OLED startup
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x64
  display.display(); // Initialize with an empty screen
  display.clearDisplay();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as OUTPUT
  pinMode(greenLED, OUTPUT);  // Set the green LED pin as OUTPUT
  pinMode(redLED, OUTPUT);    // Set the red LED pin as OUTPUT
  pinMode(yellowLED, OUTPUT); // Set the yellow LED pin as OUTPUT

  // Startup for the temperature sensor
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(AM232X_LIB_VERSION);
  Serial.println();
  if (!am2320.begin() )
  {
    Serial.println("Sensor not found");
    while (1);
  }
  am2320.wakeUp();
  delay(200);

  Serial.println("Type,\tStatus,\tHumidity (%),\tTemperature (C)");
  
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  // READ DATA
  Serial.print("am2320, \t");
  int status = am2320.read();
  switch (status)
  {
    case AM232X_OK:
      Serial.print("OK,\t");
      break;
    default:
      Serial.print(status);
      Serial.print("\t");
      break;
  }
  
  // Read temperature and humidity data from the AM2320 sensor
  float temperature = am2320.getTemperature();
  float humidity = am2320.getHumidity();

  // Read gas level from the MQ-135 sensor in parts per million (ppm)
  gasLevel = analogRead(gasSensorPin);

  // Read distance from the Ultrasonic Sensor
  // Send a pulse to the Trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Measure the pulse duration on the Echo pin
  unsigned long pulseDuration = pulseIn(echoPin, HIGH);
  // Calculate the distance based on the speed of sound in air (approximately 343 meters/second at 20°C)
  distance = pulseDuration * 0.0343 / 2.0;

  // Storing values to be send to the website server
  publisher.store("Temperature", temperature); // store value for temperature
  publisher.store("gasLevel", gasLevel);        // store value for gasLevel
  publisher.store("Humidity", humidity);      // store value for humidity
  publisher.store("Distance", distance);      //// store value for distance
  publisher.send();                     // send stored data to website

  delay(200);

  // Print temperature, humidity, gas level, and distance readings to the Serial Monitor
  Serial.print("Temperature (C): ");
  Serial.println(temperature);
  
  Serial.print("Humidity (%): ");
  Serial.println(humidity);
  
  Serial.print("Gas Level (ppm): ");
  Serial.println(gasLevel);
  
  Serial.print("Distance (cm): ");
  Serial.println(distance);
  
  delay(100); // Delay 1 second
  
  // Display temperature, humidity, gas level, and distance on the OLED screen
  display.clearDisplay();
  display.setTextSize(1); // Set text size to 1 (6x8 pixels per character)
  display.setTextColor(SSD1306_WHITE); // Set text color to white
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 16);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 32);
  display.print("Gas Level: ");
  display.print(gasLevel);
  display.println(" ppm");

  display.setCursor(0, 48);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");

  display.display();
  delay(100); // Wait for 1 second before the next reading

  // Turn on the buzzer if the distance is less than or equal to 15 cm
  if (distance <= 15) {
    tone(buzzerPin, 255); // Generate a tone with frequency 255 Hz
  } else {
    noTone(buzzerPin); // Turn off the buzzer
  }

   // Fire Situation Detection
   const float highTempThreshold = 32.0;
   const float lowHumidityThreshold = 45;
   int Green_LED_Value = LOW;
   int Red_LED_Value = (temperature >= highTempThreshold && humidity <= lowHumidityThreshold) ? HIGH : LOW;
   if (Red_LED_Value != HIGH) Green_LED_Value = HIGH;
   digitalWrite(redLED, Red_LED_Value);
   digitalWrite(greenLED, Green_LED_Value);

   // Gas Control
   const float highGasThreshold = 1000;
   if (gasLevel >= highGasThreshold) digitalWrite(yellowLED, HIGH);
   else digitalWrite(yellowLED, LOW);

  delay(200); // Wait for 0.2 second before the next reading
  // RGB ring
  pixels.setBrightness(125);
  if (digitalRead(redLED) == HIGH) {
     for(int i=0;i<NUMPIXELS;i++)
     {
        pixels.setPixelColor(i, pixels.Color(255,0,0));
        pixels.show();
     }
  }
  else if (digitalRead(greenLED)  == HIGH ) {
    for(int i=0;i<NUMPIXELS;i++)
     {
        pixels.setPixelColor(i, pixels.Color(0,255,0));
        pixels.show();
     }
  }
  else if (digitalRead(yellowLED)== HIGH) {
    for(int i=0;i<NUMPIXELS;i++)
     {
        pixels.setPixelColor(i, pixels.Color(255,255,0));
        pixels.show();
     }
  }
  else {
    for(int i=0;i<NUMPIXELS;i++)
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255,0,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(250); // Delay for a period of time (in milliseconds).
    }
    for(int i=0;i<NUMPIXELS;i++)
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,255,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(250); // Delay for a period of time (in milliseconds).
    }
    for(int i=0;i<NUMPIXELS;i++)
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,255)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(250); // Delay for a period of time (in milliseconds).
    }
  }
}
