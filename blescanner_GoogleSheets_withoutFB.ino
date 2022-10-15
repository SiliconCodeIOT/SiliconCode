/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "time.h"
#include <WiFi.h>
#include <HTTPClient.h>


 #define ONBOARD_LED 2

bool initialWiFi = false;

String stringValue;
float floatValue;
bool signupOK = false;

bool WiFi_ON = false;
bool BLESCAN = true;


String NameTemp;
String AddressTemp;
String AddressComp = "ff:ff";
String Name[4];
String Address[4];
int rssitemp;
int rssi[4];
int timeinmins;
String fbtag;



int scanTime = 25; //In seconds

int I = 1;
int D =0;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;
 int hrs;
 int mins;

 int getCurrentHrsMins()
 {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
    
  }
  hrs = timeinfo.tm_hour;
  mins = timeinfo.tm_min;

  Serial.print("Hours:");
  Serial.println(hrs);

  Serial.print("Minutes:");
  Serial.println(mins);

  return(hrs * 60 + mins);
 }


BLEScan* pBLEScan;


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) { 
  if(BLESCAN) {
    Serial.println("I am still in BLESCAN ");
if(I<5) {
  NameTemp = advertisedDevice.getName().c_str();   
  BLEAddress devAddress = advertisedDevice.getAddress();
  
//  Serial.println(devAddress.toString().c_str()); 
  AddressTemp = devAddress.toString().c_str();

  rssitemp = advertisedDevice.getRSSI();
//  Serial.println(AddressTemp.substring(0, 5));
  Serial.println(AddressComp);
  Serial.println("**************************************************");
  if((AddressTemp.substring(0, 5) == AddressComp)&& (( AddressTemp != Address[1]) || ( AddressTemp != Address[2]) || (AddressTemp != Address[3]) || ( AddressTemp != Address[4]))){  
  Address[I] = AddressTemp; 
  Name[I] = NameTemp; 
  Name[I].trim();
  rssi[I] = rssitemp;
  Serial.println(I);
  Serial.println(Address[I]);  
  Serial.println(Name[I]);
  Serial.println(rssi[I]);
  I=I+1;
    
   }

   
  }
 else {
  I=0;
  WiFi_ON = true;
  BLESCAN = false;
  BLEDevice::getAdvertising()->stop();
  btStop();
  esp_bt_controller_disable();
 // delay(1000);
  Serial.println("BT STOP");
  initialWiFi = true;
  delay(300);
  if(WiFi_ON) {
  WiFi.begin("ACT101649192057", "82786560");
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
    digitalWrite(ONBOARD_LED,HIGH);   
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  digitalWrite(ONBOARD_LED,LOW);

  if(WiFi_ON){
  configTime(gmtOffset_sec, daylightOffset_sec,ntpServer);
  timeinmins = getCurrentHrsMins();
  Serial.println(timeinmins);
 }

 
      }
      initialWiFi = false;
      Serial.println("End of WiFiSetup");

       //init and get the time
 
  
    
  for(D=1 ; D<5 ; D++) {
  
 String urlFinal = "https://script.google.com/macros/s/AKfycbx2F2toy1XUQ2Ot8drqLmO_NvL6q-4WW2HJLkEYB43d7mUYGi24RMBnAEsChdeDfzb-ZQ/exec?time="+String(timeinmins)+"&name="+Name[D]+"&id="+Address[D];


// String urlFinal =   "https://script.google.com/macros/s/AKfycbwKWU8KyXnSCRB8yRP0ZvsIjGEoOGbtn2-0tiBvRASetzlwmIX7YIxwc2PtG0ZadIZRyA/exec?time=90&name=%22Radhanand%22&id=12345678";
    Serial.print("POST data to spreadsheet");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------

     http.end();
 delay(1000);  
  

      }   
     
    WiFi_ON = true;
   Serial.println("Stage1");  
   if(BLESCAN){ Serial.println("BSCAN is true");} else { Serial.println("BSCAN is false");}
   if(WiFi_ON) { Serial.println("WIFi_ON is true");} else { Serial.println("WIFi_ON is false");}
   if(initialWiFi) { Serial.println("initialWiFi is true");} else { Serial.println("initialWiFi is false");}
   
    }

    Serial.println("Stage2");
    if(BLESCAN){ Serial.println("BSCAN is true");} else { Serial.println("BSCAN is false");}
   if(WiFi_ON) { Serial.println("WIFi_ON is true");} else { Serial.println("WIFi_ON is false");}
   if(initialWiFi) { Serial.println("initialWiFi is true");} else { Serial.println("initialWiFi is false");}
   
 
  }

    }
 // Serial.println("Stage3");
};

void setup() {
  Serial.begin(115200);

  
  
  pinMode(ONBOARD_LED,OUTPUT);

 

if(BLESCAN){
 Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  
}

  }

 



void loop() {

Serial.println("I am in the loop ");
 // put your main code here, to run repeatedly:
 // Serial.println(getCurrentHrsMins());

 

 //if(initialWiFi){
 // Serial.println("I am in initialWiFi ");
 // SetupWifi();
 //}


if(BLESCAN) {

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.println("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(300);
} 


}



    
