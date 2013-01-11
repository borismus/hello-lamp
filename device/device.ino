
/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Circuit:
 * WiFi shield attached
 
 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <SPI.h>
#include <WiFi.h>

char ssid[] = "SSID"; //  your network SSID (name) 
char pass[] = "PASSWORD";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(173,194,73,105);  // numeric IP for Google (no DNS)
char server[] = "lamp-server.appspot.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

String received;
boolean polling = false;

int ledPin = 3;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    //status = WiFi.begin(ssid);
    status = WiFi.begin(ssid, pass);

    // wait some seconds for connection:
    delay(2000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {
  // Read bytes from server if they are available.
  if (client.connected()) {
    while (client.available()) {
      char c = client.read();
      received += c;
    }
  }

  // If the server disconnects, parse the response and disconnect the client.
  if (!client.connected()) {
    String comp = received.substring(received.length() - 1, received.length());

    if (comp.compareTo("1") == 0) {
      digitalWrite(ledPin, HIGH);
      Serial.println("Light On");
    }
    else if (comp.compareTo("0") == 0) {
      digitalWrite(ledPin, LOW);
      Serial.println("Light Off");
    }

    received = "";
    client.stop();

    // Continue polling.
    pollServer();
  }
}

void pollServer() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
    // Make a HTTP request:
    client.println("GET / HTTP/1.1");
    client.println("Host:lamp-server.appspot.com");
    client.println("Connection: close");
    client.println();
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
