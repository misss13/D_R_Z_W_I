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
  servo1.attach(servoPin);

  // Jak ma być AP to wystarczy wywalić haselo
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  
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
  WiFiClient client = server.available();

  if (client) {                             
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
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

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c; 
        }
      }

    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
