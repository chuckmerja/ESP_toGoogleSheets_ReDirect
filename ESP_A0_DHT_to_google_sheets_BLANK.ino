// BY: Akshaya Niraula  file = ESP_A0_DHT_to_google_sheets_BLANK
// was ESP_to_google_sheets_2buttes WORKED - now BLANK see psi to in/ft below!!
 /*2Buttes tank at 3705 ft, Pump1 = 3526 ft, Pump1 = 3600 ft
 * water tank 40 ft tall
 * 3705+40 = 3745 - 3600 = 145 ft ~ 63 psi
 * 4 ft lower 141 ft is pump cut in... 61
 * 1 psi = 2.30672571 ft or 1 ft = 0.43 psi
 * 
  100 psi Analog gauge, convert to psi, inches water, feet water
 * diff pressure sensor info: 
 * 14.5 psi = 401.7601 in = 33.4800083 ft water
 * 7.25 psi = 200.88 in = 16.74 ft water
 * 1.45 psi = 40.17601 in = 3.3475 ft water*/
// ON: Nov 11, 2016 
// AT: http://www.embedded-lab.com/
// http://embedded-lab.com/blog/post-data-google-sheets-using-esp8266/
// fingerprint ‎f8 1e 31 71 fa 08 5b c0 4c 83 b6 64 4b 9f 22 9f 0c ba 8e 57
// GOOGLE Sheet ...
// google script ...
// script test  ...
// change SSID, PWD, Example, and MAP

// macro built by script 

#include <ESP8266WiFi.h> 
#include <HTTPSRedirect.h>

#include<DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "OUR SSID"; //Merja2015,SCI,ATT-WIFI-8681
const char* password = "OUR PWD";  //sillycheese9,"",78061148

// The ID below comes from Google Sheets. 
// Towards the bottom of this page, it will explain how this can be obtained 
const char *GScriptId = "GScriptID - goes here";  // Script ID goes here

// Push data on this interval 
const int dataPostDelay = 20000;     // 15 minutes = 15 * 60 * 1000 = was 120000

const char* host = "script.google.com"; 
const char* googleRedirHost = "script.googleusercontent.com";

const int httpsPort =     443; 
HTTPSRedirect client(httpsPort);

// Prepare the url (without the varying data) 
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "‎f8 1e 31 71 fa 08 5b c0 4c 83 b6 64 4b 9f 22 9f 0c ba 8e 57";

// We will take analog input from A0 pin 
const int AnalogIn                 = A0;
String tag("EXAMPLE");  // change to where - IE Pump1, SRMSTank, Pump2, Confine
int WaterHtIn;
float WaterHtFt;
float h; // = dht.readHumidity();
float t; // = dht.readTemperature();
int WaterPress;

void setup() { 
    dht.begin();  // sensor
    delay(2000);
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
  WaterHtIn = map(WaterPress, 0, 1023, 43, 174);   // for 2buttes well ...43 inches water might be bottom of sensor 14.5ftx12=174
  WaterHtFt = WaterHtIn/12;
  h = dht.readHumidity();
  t = dht.readTemperature();
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
    

    // Post these information 
    postData(tag, t, h, WaterPress, WaterHtIn, WaterHtFt); 
     
    delay (dataPostDelay); 
}

