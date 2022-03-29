#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
//DS18B20 is connected to
const int oneWireBus = 5;     
const int ledPin = 15; 
// OneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
//OneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
const char* ssid = "SmartLab-TS2030";
const char* password = "";
const char* host = "dweet.io";
void setup() {
  // Serial Monitor Start
  Serial.begin(115200);
  // Start DS18B20 sensor
  sensors.begin();
  pinMode(ledPin, OUTPUT); 
  Serial.begin(115200);
  delay(10);
  // Start by connecting to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0)+ 148;
  float temperatureF = sensors.getTempFByIndex(0)+ 266;
  delay(5000);
  Serial.print("connecting to ");
  Serial.println(host);
    //WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Room Temperature");
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(5000);
  if (temperatureC > 20){
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
    Serial.println(" It's Hot!");
    }
  // Created a URI for the request
  String url = "/dweet/for/my-thing-name?";
    // Send request
  Serial.print("Requesting URL: ");
  Serial.println(url); 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
    // Read lines from the answer
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  // Close connecting
  Serial.println();
  Serial.println("closing connection");
}
