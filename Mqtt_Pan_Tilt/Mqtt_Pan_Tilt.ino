/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald

 modified for the ESP32 on March 2017
 by John Bennett

 see http://www.arduino.cc/en/Tutorial/Sweep for a description of the original code

 * Different servos require different pulse widths to vary servo angle, but the range is 
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 * 
 * Circuit: (using an ESP32 Thing from Sparkfun)
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 * 
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pin 18.
 * 
 * In this example, we assume a Tower Pro MG995 large servo connected to an external power source.
 * The published min and max for this servo is 1000 and 2000, respectively, so the defaults are fine.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
 */
 #if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  
  #include <ESP8266WiFi.h>
 #endif

 #include <PubSubClient.h>
int posx, posy;
//MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

int lengthp,lengtht,lengthr, length;

String rxString = "";
String comma = ",";
std::string rxValue;  // rxValue gathers input data

String panString,tiltString ;

 void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  rxString = "";
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
   rxString += (char)payload[i]; }

   Serial.println("receivedinfo : " + rxString);
   Serial.println("Split :" + rxString.split(","));
   length = rxString.length();
   lengthr = length;
   Serial.println("receivedStringLength :" + lengthr);

   panString = rxString;
   
  int  comma = rxString.indexOf(",");
   if (comma != -1) {
    panString = rxString.substring(0, comma);}
    Serial.println("receivedStringTillComma :" + panString);
    
    length = panString.length();
    lengthp = length;
    Serial.println("receivedStringLengthTillComma :" + lengthp);


String    tiltSring = rxString.substring(lengthp,lengthr);
   
    Serial.println("receivedStringAfterComma :" + tiltString);
   
   length = tiltString.length();
  
    lengtht = length;
    Serial.println("receivedStringLengthAfterComma :" + lengtht);

    
   String Blah = "Rambabu";
   Serial.println(Blah + Blah.length());

    

    
    
    

    

    Serial.print("PanValue : " + rxString.substring(3,5));
    posx = (rxString.substring(3,5)).toInt();
    
    Serial.println("  TiltValue : " + rxString.substring(0,2));
    posy = (rxString.substring(0,2)).toInt();
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
        mqttClient.subscribe("Pan_Tilt");
      }
      
  }
}

 #define WIFI_SSID "ACT101649192057 DOWN"
 #define WIFI_PASSWORD "82786560"
//#define WIFI_SSID "radsnw"
//#define WIFI_PASSWORD "rads1961"

// #define WIFI_SSID "ACT 101642812658 up"
// #define WIFI_PASSWORD "71350109"


#include <ESP32_Servo.h>

Servo myservo;  // create servo object to control a servo
                // 16 servo objects can be created on the ESP32

Servo myservo1;                

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 27;
int servoPin1 = 25;
int Joyx = 33;
int Joyy = 34;



void setup() {
  myservo.attach(servoPin, 500, 2400);   // attaches the servo on pin 18 to the servo object
                              // using default min/max of 1000us and 2000us
                              // different servos may require different min/max settings
                              // for an accurate 0 to 180 sweep
  myservo1.attach(servoPin1, 500, 2400);

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
//  WiFiFlag = true;
  

  setupMQTT();
                              
}

void loop() {
 if (!mqttClient.connected())
       reconnect();
       mqttClient.loop();

 // posx = analogRead(Joyx);
 // posx = map(posx, 0, 1023, 0, 180);
  Serial.println(posx);

//  posy = analogRead(Joyy);
//  posy = map(posy, 0, 1023, 0, 180);
  Serial.println(posy);
 
  

  myservo1.write(posx);             
  delay(300);   

  myservo.write(posy);             
  delay(300);   
    
    
 
    
                         
}
