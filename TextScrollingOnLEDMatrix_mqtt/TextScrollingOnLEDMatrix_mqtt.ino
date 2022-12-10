// THis program works
// Use bluefruit app, UART tool to send information.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include <FastLED.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  
  #include <ESP8266WiFi.h>
 #endif

#define PIN 4
#define CLOCK_PIN 13
#define NUM_LEDS 160

#include <PubSubClient.h>


bool deviceConnected = false;
float txValue = 0;
const int button = 0;      // button on PIN G0
const int readPin = 32;    // analog pin G32
const int LEDpin = 2;      // LED on pin G2
bool convert = false;
String rxString = "";
std::string rxValue;       // rxValue gathers input data
bool WiFiFlag = false;


//MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;


void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  rxString = "";
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) 
   // Serial.println((char)payload[i]);
   rxString += (char)payload[i];

    Serial.println("receivedinfo : " + rxString);

}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("Light");
      }
      
  }
}

// Insert your network credentials
// #define WIFI_SSID "ACT101649192057 DOWN"
// #define WIFI_PASSWORD "82786560"
//#define WIFI_SSID "radsnw"
//#define WIFI_PASSWORD "rads1961"

#define WIFI_SSID "ACT 101642812658 up"
#define WIFI_PASSWORD "71350109"



unsigned long sendDataPrevMillis = 0;
String stringValue;
float floatValue;
bool signupOK = false;




// Define the array of leds
CRGB leds[NUM_LEDS];

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN, // It was (32, 8, PIN,
NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT +
NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};

void setup() { 

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  matrix.setTextColor(colors[0]);

Serial.begin(115200);  
//pinMode(LEDpin, OUTPUT); 
// pinMode(button, INPUT);

 // pinMode(LED_BUILTIN,OUTPUT);
 //  pinMode(2,OUTPUT);
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
 //   digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    delay(300);   
  } 
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  WiFiFlag = true;
  

  setupMQTT();
  
 
}

int x    = matrix.width();
int pass = 0;

void loop() { 
 matrix.fillScreen(0);    //Turn off all the LEDs
  matrix.setCursor(x, 0);

   if (!mqttClient.connected())
       reconnect();
       mqttClient.loop();
//    mqttClient.publish("Light", "123");
//   rxString = "";
       
    
  matrix.print(rxString);
 
  if( --x < -100 ) {
    x = matrix.width();
    if(++pass >= 7) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  matrix.show();
  delay(40);


}
