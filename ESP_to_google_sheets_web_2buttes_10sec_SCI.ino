// BY: Akshaya Niraula  file = ESP_to_google_sheets_2buttes WORKS!!
// ON: Nov 11, 2016 
// AT: http://www.embedded-lab.com/
// http://embedded-lab.com/blog/post-data-google-sheets-using-esp8266/
// google doc 19mmj0zVhGIMqWkdbYVE38X4Phoz4GOEYjNE5cEK2cYk
// fingerprint ‎f8 1e 31 71 fa 08 5b c0 4c 83 b6 64 4b 9f 22 9f 0c ba 8e 57

// GOOGLE Sheet   1jx5KI57s0RJAw39qvAaMyPHOTgH08EsVn7diabybRKU
// google script   AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8
// script test https://script.google.com/macros/s/AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8/exec?tag=test&amp;value=-1

// macro built by script /macros/s/AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8/exec?tag=Pump_1&value=161.00

#include <ESP8266WiFi.h> 
#include <HTTPSRedirect.h>

const char* ssid = "SCI"; //Merja2015
const char* password = "";  //sillycheese9

// The ID below comes from Google Sheets. 
// Towards the bottom of this page, it will explain how this can be obtained 
const char *GScriptId = "AKfycbwuNzUgyDpDvxatRFoqWeTV0JVCb4ZwUkKVw59nVX4Dk7FH6tC8"; //was 1FLI-zt0VaVzhtmpKQjIqH2gejamDOjImK5I0BgxFXB5GxcER4cXZ93TY

// Push data on this interval 
const int dataPostDelay = 10000;     // 15 minutes = 15 * 60 * 1000

const char* host = "script.google.com"; 
const char* googleRedirHost = "script.googleusercontent.com";

const int httpsPort =     443; 
HTTPSRedirect client(httpsPort);

// Prepare the url (without the varying data) 
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "‎f8 1e 31 71 fa 08 5b c0 4c 83 b6 64 4b 9f 22 9f 0c ba 8e 57";

// We will take analog input from A0 pin 
const int AnalogIn                 = A0;

void setup() { 
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
void postData(String tag, float value){ 
    if (!client.connected()){ 
            Serial.println("Connecting to client again…"); 
            client.connect(host, httpsPort); 
    } 
    String urlFinal = url + "tag=" + tag + "&value=" + String(value); 
    client.printRedir(urlFinal, host, googleRedirHost); 
    Serial.println("urlFinal etal =");
    Serial.println(host);
    Serial.println(googleRedirHost); 
    Serial.println(urlFinal);
}

// Continue pushing data at a given interval 
void loop() { 
     
    // Read analog value, in this case a soil moisture 
    int data = analogRead(AnalogIn);

    // Post these information 
    postData("__Pump_1", data); 
     
    delay (dataPostDelay); 
}

