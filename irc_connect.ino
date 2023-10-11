/*
    Code to connect a ESP32 to an irc channel

 */

#include <WiFi.h>

const char* ssid     = "your_ssid";       // Change this to your WiFi SSID
const char* password = "your_wi-fi_pw";   // Change this to your WiFi password

const char* HOST = "irc.freenode.net";    // Change this
const int PORT = 6667;                    // Change this

const String NICK = "your_name";          // Change this
const String IDENT= "your_ident";         // Change this
const String REALNAME = "your_rname";     // Change this
const String CHANNEL = "#your_channel";   // Change this

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  
  while(!Serial){delay(100);}

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println("******************************************************");
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

  if(!client.connect(HOST, PORT)){
    Serial.println("Connection to host failed");
    delay(1000);
    return;
  }else{
    Serial.println("Connected");

    client.println("NICK " + NICK);
    client.println("USER " + IDENT+ " 0 * :" + REALNAME);
  }

}

void loop(){
  if (client.connected()) {
    if (client.available()) {

      String line = client.readStringUntil('\n');
      Serial.println(line);
      if(line.startsWith("PING")){
        line.replace("PING", "PONG");
        client.println(line);
        Serial.println(line);
      }
    if (line.indexOf("MODE") != -1) {
      client.println("JOIN " + CHANNEL);
    }

      // Manage here your IRC messages

    }
  } else {
    Serial.println("Connection lost.");
    client.stop();
    while (true);
  }
}