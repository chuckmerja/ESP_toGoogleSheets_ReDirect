// BY: Akshaya Niraula  file = ESP32_A0_noDHT_GS_CS_water
// ON: Nov 11, 2016 
// AT: http://www.embedded-lab.com/
// http://embedded-lab.com/blog/post-data-google-sheets-using-esp8266/
// google doc 19mmj0zVhGIMqWkdbYVE38X4Phoz4GOEYjNE5cEK2cYk 
// old script 451 5/3 AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8
// fingerprint ‎f8 1e 31 71 fa 08 5b c0 4c 83 b6 64 4b 9f 22 9f 0c ba 8e 57
// test gets "ok" https://script.google.com/macros/s/AKfycbyKtZWc4Lymbo8JCPOLtH13WaqRGtAZzAdpjzr1HXsratFbqmU/exec
// 5/3/19 https://script.google.com/macros/s/AKfycbwAAbR_OHcmZa9fm5eMtXHFupwUXg-iGnD2Uv4xiqSroFnDp4k/exec
// GOOGLE Sheet   1jx5KI57s0RJAw39qvAaMyPHOTgH08EsVn7diabybRKU
// oldgoogle script   AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8
// google script for SRMS 5/3/19 AKfycbwAAbR_OHcmZa9fm5eMtXHFupwUXg-iGnD2Uv4xiqSroFnDp4k
// google script for SRMS pop https://script.google.com/macros/s/AKfycbwAAbR_OHcmZa9fm5eMtXHFupwUXg-iGnD2Uv4xiqSroFnDp4k/exec?temperature=21&humidity=82&WatPrs=500&WatHtIn=81&WatHtFt=7
// script test https://script.google.com/macros/s/AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8/exec?tag=test&amp;value=-1

// macro built by script /macros/s/AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8/exec?tag=Pump_1&value=161.00

//#include <ESP8266WiFi.h> comment cuz won't compile for ESP32
//#include <WIFI.h>
#include <HTTPSRedirect.h>

//#include<DHT.h>
//#define DHTPIN 2
//#define DHTTYPE DHT11 

//DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Merja2015"; //Merja2015,SCI,ATT-WIFI-8681
const char* password = "sillycheese9";  //sillycheese9," ",78061148

// The ID below comes from Google Sheets. 
// Towards the bottom of this page, it will explain how this can be obtained 
const char *GScriptId = "AKfycbwAAbR_OHcmZa9fm5eMtXHFupwUXg-iGnD2Uv4xiqSroFnDp4k"; 

// Push data on this interval 
const int dataPostDelay = 120000;     // 15 minutes = 15 * 60 * 1000  120000 is min

const char* host = "script.google.com"; 
const char* googleRedirHost = "script.googleusercontent.com";

const int httpsPort =     443; 
HTTPSRedirect client(httpsPort);

// Prepare the url (without the varying data) 
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "‎f8 1e 31 71 fa 08 5b c0 4c 83 b6 64 4b 9f 22 9f 0c ba 8e 57";

// We will take analog input from A0 pin 
const int AnalogIn                 = A0;
String tag("__SRMS");
int WaterHtIn;
float WaterHtFt;
int h; //was float 
int t; // was float
int WaterPress;

void setup() { 
  //  dht.begin();  // sensor
  //  delay(2000);  // for dht.begin
    Serial.begin(115200); 
    Serial.println("Connecting to wifi: "); 
    Serial.println(ssid); 
    Serial.flush();

    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED) { 
            delay(500); 
            Serial.print("."); 
    } 
    Serial.println(" IP address: "); 
    Serial.println(WiFi.localIP());
     
    Serial.print(String("Connecting to ")); 
    Serial.println(host);

    bool flag = false; 
    for (int i=0; i<5; i++){ 
            int retval = client.connect(host, httpsPort); 
            if (retval == 1) { 
                        flag = true; 
                        break; 
            } 
            else 
                    Serial.println("Connection failed. Retrying…"); 
    }

    // Connection Status, 1 = Connected, 0 is not. 
    Serial.println("Connection Status: " + String(client.connected())); 
    Serial.flush(); 
     
    if (!flag){ 
            Serial.print("Could not connect to server: "); 
            Serial.println(host); 
            Serial.println("Exiting…"); 
            Serial.flush(); 
            return; 
    }

    // Data will still be pushed even certification don’t match. 
    if (client.verify(fingerprint, host)) { 
            Serial.println("Certificate match."); 
    } else { 
            Serial.println("Certificate mis-match"); 
    } 
}

// This is the main method where data gets pushed to the Google sheet 
void postData(String tag, int tem, int hum, int WatPrs, int WatIn, float WatFt) 
    {
      if (!client.connected()){ 
            Serial.println("Connecting to client again…"); 
            client.connect(host, httpsPort); 
            delay (1000);  // added delay to connect 5/3/19 11am
    } 

  //String  url = "/macros/s/" + GScriptId + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity + "&WatPrs=" + string_WatPress + "&WatHtIn=" + string_WatHtIn + "&WatHtFt=" + string_WatHtFt;
  //String urlFinal = url + "&tag=" + tag + "&temperature=" + string_temperature + "&humidity=" + string_humidity + "&WatPrs=" + string_WatPress + "&WatHtIn=" + string_WatHtIn + "&WatHtFt=" + string_WatHtFt; //"&value=" + String(value);
    String urlFinal = url + "&tag=" + tag + "&temperature=" + t + "&humidity=" + h + "&WatPrs=" + WaterPress + "&WatHtIn=" + WaterHtIn + "&WatHtFt=" + WaterHtFt; //"&value=" + String(value);  
    client.printRedir(urlFinal, host, googleRedirHost); 
    Serial.println("urlFinal etal =");
    Serial.println(host);
    Serial.println(googleRedirHost); 
    Serial.println(urlFinal);
}

// Continue pushing data at a given interval 
void loop() { 
     
  // Read analog value, in this water pressure on differntial pressure sensor 
  WaterPress = analogRead(A0);
  /*43 inches water might be bottom of sensor 14.5ftx12=174 map was 0,1023,43,174, 
  added hose to get to 10"  and max voltage out of divider will be 0.84, so make max
  1023*0.84 - 860 and atmospheric appears to be 120-160, so 120,860,10,174*/
  WaterHtIn = map(WaterPress, 120, 860, 10, 174);   
  WaterHtFt = WaterHtIn/12;
  h = 0; //was dht.readHumidity();
  t = 0; //was dht.readTemperature();
  Serial.print("tag =");
  Serial.print(tag);
  Serial.print(t);
  Serial.print("Temp = ");
  Serial.print(t);
  Serial.print(" HUM= ");
  Serial.print(h);
  Serial.print(" WaterPress=  ");
  Serial.print(WaterPress);
  Serial.print(" WaterHtFt=  ");
  Serial.println(WaterHtFt, 2);

    if (!client.connected()){    // added 5/8/19 1238 pm
            Serial.println("Connecting to client again…"); 
            client.connect(host, httpsPort); 
            delay (1000);  // added delay to connect 5/3/19 11am
    }
    // Post these information 
    postData(tag, t, h, WaterPress, WaterHtIn, WaterHtFt); 
    
     
    delay (dataPostDelay); 
}

