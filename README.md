# D_R_Z_W_I
Esp32 project to control door lock without destroying doors and door frame.
## Items
- esp32 3V3
- servo Tower Pro MG-995R (180° range)
- male-to-female cables
- old phone charger
- carton
- double-sided tape
- powerbank
- tape
- utility knife
- time
## Code
```
#include <Servo.h>
#include <WiFi.h>

static const int servoPin = 27;
const char* ssid     = "_d_r_z_w_i_"; //ssid
const char* password = "silne_haslo"; //haslo jakies

Servo servo1;
WiFiServer server(80);

String header;
int pos;
String statu = "";
String output27State = "on";

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin);

  Serial.print("Setting AP (Access Point)…");
  // Jak ma być AP to wystarczy wywalić haselo
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
  
  //ustawia serwo na 
  pos=servo1.read();
  if(pos>90){
    for(int a=pos; a>=90; a--){
        servo1.write(a);
        delay(15);
      }
  }else if(pos<90){
    for(int a=pos; a<=90; a++){
        servo1.write(a);
        delay(15);
    }
  }
  output27State = "on";
  statu = "otwarte";
  delay(5000);  
  for(int a=90; a<=180; a++){
      servo1.write(a);
      delay(15);
    }
}
void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            //KRADZIONY KAWAŁEK KODU Z LOGAMI TEGO CO SIĘ DZIEJE
            
            // STRONKA
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style> html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} body{font-family: 'Montserrat', sans-serif; margin:0}");
            client.println(".btn { flex: 1 1 auto; margin: 10px; padding: 30px; text-align: center; text-transform: uppercase; transition: 0.5s; background-size: 200% auto; color: white; box-shadow: 0 0 20px #eee; border-radius: 10px}");
            client.println(".btn-1 {background-image: linear-gradient(to right, #ff5e62 0%,#f6d365 51%, #ffecd2 100%);}");
            client.println("a { text-decoration: none; }");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");
                
            if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              for(int posDegrees = 0; posDegrees <=180 ; posDegrees++) {
                servo1.write(posDegrees);
                delay(15);
                }
              statu = "otwarte";
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              for(int posDegrees = 180; posDegrees >=0 ; posDegrees--) {
                servo1.write(posDegrees);
                delay(15);
                }
              statu = "zamkniete";
            }
            // Web Page Heading
            client.println("<body><h1>D R Z W I</h1>");
            client.println("<p>Stan drzwi: " + statu + "</p>");
            // If the output27 State is off, it displays the ON button
            if (output27State=="off") {
              client.println("<a href=\"/27/on\"><div class=\"btn btn-1\"><h>otworz</h></div></a>");
            } else {
              client.println("<a href=\"/27/off\"><div class=\"btn btn-1\"><h>zamknij</h></div></a>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c; 
        }
      }

    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
```
Also there must be following links in Preferences>additional Url adresses:
- http://arduino.esp8266.com/stable/package_esp8266com_index.json, https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
## Troubleshooting
Sometimes there is need to long click boot button to upload project to esp.
