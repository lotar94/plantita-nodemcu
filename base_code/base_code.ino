#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <DHT11.h>


//Variables necesarias para tomar lectura de sensor
//de temperatura y humedad del aire.
int SENSOR = 4;
DHT11 dht11(SENSOR); 

int valor_leido = 0;
int variable_invertida = 0;
int porcentaje = 0;





 
void setup() {
  //Serial connection
  Serial.begin(115200);
 
  WiFi.begin("LOTAR_2.4", "Lamc1994");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    /************************HUMEDAD DE LA TIERRA*******************************/
    valor_leido = analogRead(A0);
    variable_invertida = 1024 - valor_leido;
    porcentaje = map(variable_invertida, 0, 694, 0, 100);
    Serial.print("Humedad del la planta en % es de =  ");
    Serial.print(porcentaje);
    Serial.println(" % ");
    /************************HUMEDAD DE LA TIERRA*******************************/


    /************************TEMPERATURA Y HUMEDAD DEL AIRE*********************/
    int err;
    float temp, humi;
    if((err=dht11.read(humi, temp))==0)
    {
      Serial.print("temperature:");
      Serial.print(temp);
      Serial.print(" humidity:");
      Serial.print(humi);
      Serial.println();
    }
    else
    {
      Serial.println();
      Serial.print("Error No :");
      Serial.print(err);
      Serial.println();    
    }
    /************************TEMPERATURA Y HUMEDAD DEL AIRE*********************/


    /***********CREACION DE JSON Y ENVIAO DE PETICION POST A SERVIDOR**********/
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
    
    JSONencoder["humidity"] = porcentaje;
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
    //Specify request destination
    http.begin("http://plant-286601.ue.r.appspot.com/lastMeasurement");
    //Specify content-type header
    http.addHeader("Content-Type", "application/json");  
    //Send the request
    int httpCode = http.POST(JSONmessageBuffer);
    //Get the response payload
    String payload = http.getString();
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
    /***********CREACION DE JSON Y ENVIAO DE PETICION POST A SERVIDOR**********/
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
  delay(3000);  //Send a request every 3 seconds
 
}
