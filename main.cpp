#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "YOUR_SSID"
#define STAPSK  "YOUR_SSID_PASSWORD"
#endif

String getValue(String data, char separator, int index);

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "IP_ADDRESS_EMONCMS";
const uint16_t port = 80;

String apidata = "";

int powerconsumption_i;
int powergeneration_i;
int powerhome_i;
float energyconsumption_f;
float energygeneration_f;
float energyhome_f;
int solarradiation_i;
float dailygridcost_f;
float referencevoltage_f;
float energyexport_f;
float energyimport_f;
float energyexportpercent_f;
float energyimportpercent_f;
float energyconsumptionpercent_f;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 4);

//
// function to display the first screen of the LCD display
//

void screen1()
  {
    // Turn on the blacklight and print a message.
    lcd.clear();
  
    // Print Header on Line 0
  
    lcd.setCursor(9, 0);
    lcd.print("(w)   (kWh)");
  
    // Set Line #1
    lcd.setCursor(0, 1);
    lcd.print("GRID :");
  
    // Print Power Consumption Value (w)
    if ((abs(powerconsumption_i)) >= 1000) {
      lcd.print(" ");
    }
    else if ((abs(powerconsumption_i)) >= 100) {
      lcd.print("  ");
    }
    else if ((abs(powerconsumption_i)) >= 10) {
      lcd.print("   ");
    }
    else  {
      lcd.print("    ");
    }
  
    if (powerconsumption_i >= 0) {
      lcd.print(" ");
    }
    lcd.print(powerconsumption_i);
    lcd.print("  ");
  
    //Print Energy Consumption Value (kWhs)
    if ((abs(energyconsumption_f)) < 10)  {
      lcd.print(" ");
    }
  
    if (energyconsumption_f >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(energyconsumption_f);
  
    // Set Line #2
    lcd.setCursor(0, 2);
    lcd.print("SOLAR:");
  
    // Print Power Generation Value (w)
    if (powergeneration_i < 0) {
      powergeneration_i = 0;
    }
    if ((abs(powergeneration_i)) >= 1000) {
      lcd.print(" ");
    }
    else if ((abs(powergeneration_i)) >= 100) {
      lcd.print("  ");
    }
    else if ((abs(powergeneration_i)) >= 10) {
      lcd.print("   ");
    }
    else  {
      lcd.print("    ");
    }
  
    if (powergeneration_i >= 0) {
      lcd.print(" ");
    }
    lcd.print(powergeneration_i);
    lcd.print("  ");
  
    // Print Energy Generation Value (kWh)
    if ((abs(energygeneration_f)) < 10)  {
      lcd.print(" ");
    }
  
    if (energygeneration_f >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(energygeneration_f);
  
    //Set Line #3
    lcd.setCursor(0, 3);
    lcd.print("HOME :");
  
    // Print Home Power Use (w)
    if ((abs(powerhome_i)) >= 1000) {
      lcd.print(" ");
    }
    else if ((abs(powerhome_i)) >= 100) {
      lcd.print("  ");
    }
    else if ((abs(powerhome_i)) >= 10) {
      lcd.print("   ");
    }
    else  {
      lcd.print("    ");
    }
  
    if (powerhome_i >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(powerhome_i);
    lcd.print("  ");
  
    // Print Home Energy Use (kWh)
    if ((abs(energyhome_f)) < 10)  {
      lcd.print(" ");
    }
  
    if (energyhome_f >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(energyhome_f);
  
    delay(8000);
  }

//
// function to display the second screen of the LCD display
//
void screen2()
  {
    // Turn on the blacklight and print a message.
    lcd.clear();
    lcd.setCursor(8, 0);
    lcd.print("(kWh)   (%)");
    lcd.setCursor(0, 1);
    lcd.print("IMPORT:");
  
    // Print Import ABS Daily Value (kWh)
    if ((abs(energyimport_f)) < 10)  {
      lcd.print(" ");
    }
  
    if (energyimport_f >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(energyimport_f);
  
    // Calculate energy import percentage based on consumption
  
    energyimportpercent_f = (energyimport_f / energyhome_f) * 100;
  
    if ((abs(energyimportpercent_f)) >= 100) {
      lcd.print(" ");
    }
    else if ((abs(energyimportpercent_f)) >= 10) {
      lcd.print("  ");
    }
    else  {
      lcd.print("   ");
    }
  
    lcd.print(energyimportpercent_f);
  
  
    lcd.setCursor(0, 2);
    lcd.print("EXPORT:");
  
    // Print Export ABS Daily Value (kWh)
    if ((abs(energyexport_f)) < 10)  {
      lcd.print(" ");
    }
  
    if (energyexport_f >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(energyexport_f);
  
    // Calculate energy export percentage based on consumption
  
    energyexportpercent_f = (energyexport_f / energyhome_f) * 100;
  
    if ((abs(energyexportpercent_f)) >= 100) {
      lcd.print(" ");
    }
    else if ((abs(energyexportpercent_f)) >= 10) {
      lcd.print("  ");
    }
    else  {
      lcd.print("   ");
    }
    lcd.print(energyexportpercent_f);
  
    // Print Generation ABS Daily Value (kWh)
    lcd.setCursor(0, 3);
    lcd.print("GENERA:");
  
    if ((abs(energygeneration_f)) < 10)  {
      lcd.print(" ");
    }
  
    if (energygeneration_f >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(energygeneration_f);
  
    // Calculate energy generation percentage based on consumption
  
    energyconsumptionpercent_f = (energygeneration_f / energyhome_f) * 100;
  
    if ((abs(energyconsumptionpercent_f)) >= 100) {
      lcd.print(" ");
    }
    else if ((abs(energyconsumptionpercent_f)) >= 10) {
      lcd.print("  ");
    }
    else  {
      lcd.print("   ");
    }
    lcd.print(energyconsumptionpercent_f);
  
    delay(8000);
  }

//
// function to display the third screen of the LCD display
//
void screen3()
  {
    // Turn on the blacklight and print a message.
    lcd.clear();
    lcd.setCursor(8, 0);
    lcd.print("MISC");
    lcd.setCursor(0, 1);
    lcd.print("SOLAR RAD:");
  
    // Print Solar Radiation Value (M/2)
    if ((abs(solarradiation_i)) >= 100) {
      lcd.print(" ");
    }
    else if ((abs(solarradiation_i)) >= 10) {
      lcd.print("  ");
    }
    else  {
      lcd.print("   ");
    }
  
    if (solarradiation_i < 0) {
      solarradiation_i = 0;
    }
  
    lcd.print(solarradiation_i);
    lcd.print(" m/2");
  
    lcd.setCursor(0, 2);
    lcd.print("GRID COST:");
  
    // Print Daily Grid cost (0.22 per kwh Nationl Grid)
    if ((abs(dailygridcost_f)) >= 10) {
      lcd.print(" ");
    }
    else  {
      lcd.print("  ");
    }
  
    if (dailygridcost_f >= 0) {
      lcd.print(" ");
    }
  
    lcd.print(dailygridcost_f);
    lcd.print("$");
  
  
    lcd.setCursor(0, 3);
    lcd.print("HOME VOLT: ");
  
    //Print home reference voltage value
    lcd.print(referencevoltage_f);
    lcd.print("v");
  
    delay(2000);
  }


//
// Function to make the API query to the EmonCMS serve and get the listed feeds
//

void getdata() {
  
    String url = "/emoncms/feed/fetch.json?ids=35,142,151,56,165,166,167,58,25,169,173,1&apikey=API_KEY";
  
    //
    // Use WiFiClient class to create TCP connections
    //
    Serial.print("connecting to ");
    Serial.println(host);
  
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
  
    //
    // This will send the request to the server
    //
  
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + " \r\n" + "Connection: close\r\n\r\n");
  
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    //
    // Read all the lines of the reply from server and print them to Serial
    //
  
    apidata = "";
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
      apidata = apidata + line;
    }
  
    Serial.println();
    Serial.println("closing connection");
  
    Serial.println();
    Serial.println("API Data: ");
    Serial.println(apidata);
  }

//
// Process the string data and place it in varables
//
void processdata() {
  
    String powerconsumption_s;
    String powergeneration_s;
    String powerhome_s;
    String energyconsumption_s;
    String energygeneration_s;
    String energyhome_s;
    String energyimport_s;
    String energyexport_s;
    String solarradiation_s;
    String dailygridcost_s;
    String referencevoltage_s;
  
    apidata.remove(0, 178);
    Serial.println();
    Serial.println();
    Serial.println("Processed:");
    Serial.println(apidata);
  
    powerconsumption_s = getValue(apidata, ',', 0);
    powergeneration_s = getValue(apidata, ',', 1);
    powerhome_s = getValue(apidata, ',', 2);
    energyconsumption_s = getValue(apidata, ',', 3);
    energygeneration_s = getValue(apidata, ',', 4);
    energyhome_s = getValue(apidata, ',', 5);
    solarradiation_s = getValue(apidata, ',', 6);
    dailygridcost_s = getValue(apidata, ',', 7);
    referencevoltage_s = getValue(apidata, ',', 8);
    energyexport_s = getValue(apidata, ',', 9);
    energyimport_s = getValue(apidata, ',', 10);
  
    powerconsumption_i = powerconsumption_s.toInt();
    powergeneration_i = powergeneration_s.toInt();
    powerhome_i = powerhome_s.toInt();
    energyconsumption_f = energyconsumption_s.toFloat();
    energygeneration_f = energygeneration_s.toFloat();
    energyhome_f = energyhome_s.toFloat();
    solarradiation_i = solarradiation_s.toInt();
    dailygridcost_f = dailygridcost_s.toFloat();
    referencevoltage_f = referencevoltage_s.toFloat();
    energyimport_f = energyimport_s.toFloat();
    energyexport_f = energyexport_s.toFloat();
  
    Serial.println();
    Serial.println("Data Received: ");
    Serial.println(powerconsumption_i);
    Serial.println(powergeneration_i);
    Serial.println(powerhome_i);
    Serial.println(energyconsumption_f);
    Serial.println(energygeneration_f);
    Serial.println(energyhome_f);
    Serial.println(solarradiation_i);
    Serial.println(dailygridcost_f);
    Serial.println(referencevoltage_f);
    Serial.println(referencevoltage_f);
    Serial.println(energyimport_f);
    Serial.println(energyexport_f);
  }

//
//  Parse the string and retrun the delimited offset
//
String getValue(String data, char separator, int index)
  {
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
  
    for (int i = 0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i) == separator || i == maxIndex) {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
      }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
  }
  
  void setup()  //main setup Loop Function
  {
    Serial.begin(115200);
  
    // initialize the LCD
    lcd.begin(20, 4);
  
    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.clear();
  
    // We start by connecting to a WiFi network
  
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  
    lcd.setCursor(0, 0);
    lcd.print("Connecting to ");
    lcd.setCursor(0, 1);
    lcd.print(ssid);
  
  
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
  
    lcd.setCursor(0, 2);
    lcd.print(WiFi.localIP());
    lcd.setCursor(0, 3);
    lcd.print("Connected...");
    delay(5000);
  }

//
// Main Loop where the repeated info goes
//

void loop()
  {
    getdata();       // Get First set of data from EmonCMS
    processdata();   // Process Returned string to delimited variables
    screen1();       // Display the first screen of data
    screen2();       // Display the Second screen of data
    screen3();       // Display the Third screen of data
  }

