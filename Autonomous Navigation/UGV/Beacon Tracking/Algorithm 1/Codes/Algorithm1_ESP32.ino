// Generated with a lot of love//
// with TUNIOT FOR ESP32     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include <WiFi.h>

String  ClientRequest;
WiFiServer server(80);

WiFiClient client;

String myresultat;

String ReadIncomingRequest(){
while(client.available()) {
ClientRequest = (client.readStringUntil('\r'));
 if ((ClientRequest.indexOf("HTTP/1.1")>0)&&(ClientRequest.indexOf("/favicon.ico")<0)){
myresultat = ClientRequest;
}
}
return myresultat;
}

void setup()
{
ClientRequest = "";
Serial.begin(115200);
Serial2.begin(9600, SERIAL_8N1, 16, 17);

pinMode(5, OUTPUT);
pinMode(4, OUTPUT);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("SOD","sachin009");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  server.begin();

}


void loop()
{

   if (WiFi.status() == WL_CONNECTED){
   long rssi = WiFi.RSSI();
   long val;
  //for(val=-70;val<-60;val++)
    //{
      
    Serial2.write(rssi);
    delay(100);
    Serial.println(rssi);
  //}
  }

}
