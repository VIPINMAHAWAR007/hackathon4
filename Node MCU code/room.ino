#include <IRrecv.h>
uint16_t RECV_PIN = D4;
IRrecv irrecv(RECV_PIN);
decode_results results;


#include <ESP8266WiFi.h>
const char* ssid = "Jarvis"; //Network_SSID
const char* password = "9828049058"; //Network_Password
WiFiServer server(80);

#include <Keypad.h>
const byte ROWS = 2;
const byte COLS = 2;
char hexaKeys[ROWS][COLS] = {
  {'1','2'},
  {'3','4'}
};
byte rowPins[ROWS] = {D1, D0};
byte colPins[COLS] = {D3, D2};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int comp[6] = {D5, D6, D7, D9};
int out[4] = {1,1,1,1};
int i;
unsigned int ircode;


void output(){
  for(i=0;i<4;i++){
    out[i] = out[i] % 2;
    digitalWrite(comp[i], out[i]);
  }
}

void setup() {
  irrecv.enableIRIn();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  // Start the server
  server.begin();

  for(i=0;i<4;i++){
    pinMode(comp[i], OUTPUT);
  }
  output();
  
}

void loop() {
  if (irrecv.decode(&results)) {
    ircode = results.value;
    if(ircode == 33444015 || ircode == 16)
      out[0]++;
    if(ircode == 33478695 || ircode == 2064)
      out[1]++;
    if(ircode == 33486855 || ircode == 1040)
      out[2]++;
    if(ircode == 33435855 || ircode == 3088)
      out[3]++;
    
    if(ircode == 33480735 || ircode == 2704){
      for(i=0;i<4;i++){
        out[i] = 1;
      }
    }

    irrecv.resume();
    output();
  }
  
  char customKey = customKeypad.getKey();
  if (customKey){
    if(customKey == '1')
      out[0]++;
    if(customKey == '2')
      out[1]++;
    if(customKey == '3')
      out[2]++;
    if(customKey == '4')
      out[3]++;
    output();
  }

  WiFiClient client = server.available();
  if(client){
    String request = client.readStringUntil('\r');
    
    if (request.indexOf("/BED=ON") != -1) out[0] = 0;
    if (request.indexOf("/BED=OFF") != -1) out[0] = 1;
    if (request.indexOf("/WASH=ON") != -1) out[1] = 0;
    if (request.indexOf("/WASH=OFF") != -1) out[1] = 1;
    if (request.indexOf("/KIT=ON") != -1) out[2] = 0;
    if (request.indexOf("/KIT=OFF") != -1) out[2] = 1;
    if (request.indexOf("/COR=ON") != -1) out[3] = 0;
    if (request.indexOf("/COR=OFF") != -1) out[3] = 1;
    if (request.indexOf("/All=OFF") != -1) for(i=0;i<4;i++) out[i] = 1;
    
    // Return the client response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
  
    //HTML code for controling devices from browser
    client.println("<br><br>");
    client.println("Bedroom's Light: <a href=\"/BED=ON\">ON</a> or <a href=\"/BED=OFF\">OFF</a><br>");
    client.println("<br>");
    client.println("Washroom's Light: <a href=\"/WASH=ON\">ON</a> or <a href=\"/WASH=OFF\">OFF</a><br>");
    client.println("<br>");
    client.println("Kitchen's Light: <a href=\"/KIT=ON\">ON</a> or <a href=\"/KIT=OFF\">OFF</a><br>");
    client.println("<br>");
    client.println("Coridor's Light: <a href=\"/COR=ON\">ON</a> or <a href=\"/COR=OFF\">OFF</a><br>");
    client.println("<br>");
    client.println("Turn All: <a href=\"/All=OFF\">OFF</a><br>");
    client.println("<br>");
    client.println("");
    client.println("</html>");
    output();
  }
  
 
}
