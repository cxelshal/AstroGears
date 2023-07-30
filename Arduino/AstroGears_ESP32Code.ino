// including Temperature and Humidity Sensor Libray
#include <AM232X.h>

// OLED Screen Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Real time Data Transferring Library (communication header file to the website)
#include "HackPublisher.h"

// OLED Screen Setup
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RGB ring library
#include <Adafruit_NeoPixel.h>
#define PIN 14    //declaring the pin number
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Temperature and Humidity Sensor Libray Object (creating an instance)
AM232X am2320;

// other Pins Setup
const int gasSensorPin = A3; // Analog input pin for the MQ-135 Gas Sensor
const int trigPin = 13;       // Digital output pin for Ultrasonic Trigger
const int echoPin = 12;       // Digital input pin for HC-SR04 Echo
const int buzzerPin = 27;     // Digital output pin for the Buzzer
const int greenLED = 15;     // Digital output pin for the green LED
const int redLED = 33;       // Digital output pin for the red LED
const int yellowLED = 32;    // Digital output pin for the red LED

// WIFI connection for ESP32
const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";

HackPublisher publisher("AstroGears"); // publisher instance for team "AstroGears"

int gasLevel =0; //intial values of 0 to gas, distance (where we store the data for the website)
float distance =0;

//function that runs one time (initaiting wifi)
void setup() {
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

  pinMode(trigPin, OUTPUT);   //declaring the trigpin of the ultrasoinc sensor
  pinMode(echoPin, INPUT);    //declaring the echopin of the ultrasonic sensor
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

//looping function
void loop() {
  // READ DATA and checking the status of Temp&Humid sensor
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
  distance = pulseDuration * 0.0343 / 2.0; //calculating distance (speed*time)/2. we divide by 2 cause the wave goes to the obstacle and comes back

  // Storing values to be send to the website server
  publisher.store("Temperature", temperature); // store value for temperature (will use the same id later in the html code)
  publisher.store("gasLevel", gasLevel);        // store value for gas level
  publisher.store("Humidity", humidity);      // store value for humidity
  publisher.store("Distance", distance);      // stor value for distance
  publisher.send();                     // send stored data to website

  delay(200); //wait for 0.2 seconds

  // Print temperature, humidity, gas level, and distance readings to the Serial Monitor
  Serial.print("Temperature (C): ");
  Serial.println(temperature);
  
  Serial.print("Humidity (%): ");
  Serial.println(humidity);
  
  Serial.print("Gas Level (ppm): ");
  Serial.println(gasLevel);
  
  Serial.print("Distance (cm): ");
  Serial.println(distance);
  
  delay(100); // Delay 0.1 second
  
  // Display temperature, humidity, gas level, and distance on the OLED screen
  display.clearDisplay();   //clearing the display of any previous content
  display.setTextSize(1); // Set text size to 1 (6x8 pixels per character)
  display.setTextColor(SSD1306_WHITE); // Set text color to white
  display.setCursor(0, 0);             //indicating the start point of the display to show temp
  display.print("Temperature: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 16);    // one line below to show humidity info
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 32);    // one line below to show gaslevel info
  display.print("Gas Level: ");
  display.print(gasLevel);
  display.println(" ppm");

  display.setCursor(0, 48);    // one line below to show distance info
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");

  display.display();
  delay(100); // Wait for 0.1 second before the next reading

  // Turn on the buzzer if the distance is between 45 cm and 100 cm
  if (45 <= distance && distance <= 100) {
    tone(buzzerPin, 255); // Generate a tone with frequency 255 Hz
    publisher.store("DistanceInfo", "Obstacle Detected");  //another cool feature on our website to show alerts using the same method of sending data
  } else {
    noTone(buzzerPin); // Turn off the buzzer
    publisher.store("DistanceInfo", ""); //id for our website to share distance alerts
  }

   // Fire Situation Detection if temp is >32, Humidi<45 (fire alert) turning red color on, green off
   const float highTempThreshold = 32.0;
   const float lowHumidityThreshold = 45;
   int Green_LED_Value = LOW;
   int Red_LED_Value = (temperature >= highTempThreshold && humidity <= lowHumidityThreshold) ? HIGH : LOW;
   if (Red_LED_Value != HIGH) Green_LED_Value = HIGH;
   digitalWrite(redLED, Red_LED_Value);
   digitalWrite(greenLED, Green_LED_Value);
   if (temperature >= highTempThreshold) publisher.store("TemperatureInfo", "High Temperature"); //sending alerts to the website
   else publisher.store("TemperatureInfo", "");
   if (humidity <= lowHumidityThreshold) publisher.store("HumidityInfo", "Low Humidity");
   else publisher.store("HumidityInfo", "");

   // Gas Control
   const float highGasThreshold = 1500; //based on out test and out sensor values
   if (gasLevel >= highGasThreshold) {
    digitalWrite(yellowLED, HIGH); //turn on yello color to indicate gas leaks
    publisher.store("GasLevelInfo", "Gas Leak");
   }
   else {
    digitalWrite(yellowLED, LOW);
    publisher.store("GasLevelInfo", "");
   }

  delay(200); // Wait for 0.2 before the next reading
  // RGB ring
  pixels.setBrightness(100);
  if (digitalRead(redLED) == HIGH) {
     for(int i=0;i<NUMPIXELS;i++)
     {
        pixels.setPixelColor(i, pixels.Color(255,0,0));
        pixels.show();
     }
  }
  else if (digitalRead(greenLED)  == HIGH && digitalRead(yellowLED)== HIGH) {
    for(int i=0;i<6;i++)
     {
        pixels.setPixelColor(i, pixels.Color(0,255,0));
        pixels.show();
     }
     for(int i=6;i<NUMPIXELS;i++)
     {
        pixels.setPixelColor(i, pixels.Color(255,255,0));
        pixels.show();
     }
  }
  else if (digitalRead(greenLED)  == HIGH) {
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
      //delay(250); // Delay for a period of time (in milliseconds).
    }
    for(int i=0;i<NUMPIXELS;i++)
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,255,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(250); // Delay for a period of time (in milliseconds).
    }
    for(int i=0;i<NUMPIXELS;i++)
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,255)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(250); // Delay for a period of time (in milliseconds).
    }
  }
}
