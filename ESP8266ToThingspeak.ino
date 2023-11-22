#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#include "DHT.h"

const char* ssid = "";   // Your Network SSID, it works with hidden and visible networks so just add the ssid
const char* password = "";       // Your Network Password

int val;
int pin = 12; // LM35 Pin Connected at A0 Pin

#define DHTPIN 12     // Define the pin connected to the DHT11 sensor
#define DHTTYPE DHT11   // Define the sensor type

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

unsigned long myChannelNumber = 1234567; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "SOME API KEY"; //Your Write API Key

void setup()
{
  Serial.begin(9600);
  delay(10);
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  dht.begin();
}

void loop()
{
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float temperatureF = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float heatIndexF = dht.computeHeatIndex(temperatureF, humidity);
  // Compute heat index in Celsius (isFahreheit = false)
  float heatIndexC = dht.computeHeatIndex(temperature, humidity, false);

  delay(5000);
  ThingSpeak.writeField(myChannelNumber, 1,temperature, myWriteAPIKey); //Update in ThingSpeak
  ThingSpeak.writeField(myChannelNumber, 2,humidity, myWriteAPIKey);
}
