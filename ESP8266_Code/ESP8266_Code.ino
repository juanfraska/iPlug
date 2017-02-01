#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define RELE 12 //We connect the relay to the GPIO12

ESP8266WebServer server (80); //Listen to port 80
WiFiClient client; 

float value = 0;
String url;
const int sensorIn = A0;
int mVperAmp = 141; // use 100 for 20A Module and 66 for 
                    //30A Module sensibility 0.141 V/A
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;

const char* host = "192.168.1.161"; //Static IP. Using a local net.

const char* ssid     = "***";   //Router SSID
const char* password = "***";   //Router password

void handleOn(){
  digitalWrite(RELE, 1);
  Serial.println("Rele ON");

}

void handleOff(){
  digitalWrite(RELE, 0);  
  Serial.println("Rele Off");
}

float getVPP()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }

void setup() {
  pinMode(RELE, OUTPUT);
  Serial.begin(115200);
  delay(10);
  
  //Testing the relay
  digitalWrite(RELE, 0);
  delay(1000);
  digitalWrite(RELE, 1);
  delay(1000);
  digitalWrite(RELE, 0);
  delay(1000);
  digitalWrite(RELE, 1);
  delay(1000);
  //Testing the relay 

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/releOff", handleOff);
  server.on("/releOn", handleOn);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections

  const int httpPort = 3000;
  while (!client.connect(host, httpPort)) {
    Serial.println("connection failed. Retrying");
    delay(500);
   
  } 
  Serial.println("connection OK");

  server.begin();
  
}


void loop() {
  delay(500);
  Voltage = getVPP();
  VRMS = (Voltage/2.0) *0.707; 
  AmpsRMS = (VRMS * 1000)/mVperAmp;
// Serial.print(AmpsRMS);
// Serial.println(" Amps RMS");
  value = AmpsRMS;
   
  url = "/sensValue/";
  url += String(value);

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n"); //Petition to server
               
  server.handleClient();
}

