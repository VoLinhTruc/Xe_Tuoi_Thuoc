#include <ESP8266WiFi.h>

char station_ssid[] = "myrio";
char station_pass[] = "thanhquyen1416";

WiFiServer my_server(80);

IPAddress static_ip(192,168,1,69);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  my_server.begin();

  WiFi.config(static_ip, gateway, subnet);
  WiFi.begin(station_ssid, station_pass);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println();
  Serial.println(String(station_ssid) + " has connected");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient new_client = my_server.available();
  if(new_client)
  {
    Serial.println("Client connected !!!");
    while(new_client.connected())
    {
      if(new_client.available())
      {
        uint8_t data = new_client.read();
        Serial.write(data);
      }
    }
    new_client.stop();
    Serial.println("Client stop !!!");   
  }
}



