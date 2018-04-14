#include <ESP8266WiFi.h>

char station_ssid[] = "myrio";
char station_pass[] = "thanhquyen1416";

IPAddress station_ip(192,168,1,69);
IPAddress station_gateway(192,168,1,1);
IPAddress station_subnet(255,255,255,0);

char AP_ssid[] = "ESP01-NANO";

IPAddress AP_ip(192,168,4,1);
IPAddress AP_gateway(192,168,4,1);
IPAddress AP_subnet(255,255,255,0);

WiFiServer my_server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  my_server.begin();

  WiFi.config(station_ip, station_gateway, station_subnet);
  WiFi.begin(station_ssid, station_pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.println(String(station_ssid) + " has connected");
  Serial.println(WiFi.localIP());

  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(AP_ip, AP_gateway, AP_subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient new_client = my_server.available();
  if(new_client)
  {
    Serial.println("Client connected");
    while(new_client.connected())
    {
      if(new_client.available())
      {
        Serial.write(new_client.read());
      }
    }
    new_client.stop();
    Serial.println("Client stop");   
  }
}



