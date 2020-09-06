#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
//#include <DHT.h>
#include <DHT11.h>


int SENSOR = 4;
int temp, humedad;
//DHT dht (SENSOR, DHT11);
DHT11 dht11(SENSOR); 

int valor_leido = 0;
int variable_invertida = 0;
int porcentaje = 0;





 
void setup() {

  
 
  Serial.begin(115200);                            //Serial connection


  //dht.begin();//Temperatura inicio 
  
  WiFi.begin("LOTAR_2.4", "Lamc1994");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 

 
    //ADC_Value = analogRead (A0);
    valor_leido = analogRead(A0);

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

    /*humedad = dht.readHumidity();
    temp = dht.readTemperature();
    Serial.print("Temperatura:");
    Serial.print(temp);
    Serial.print(" °C humedad: ");
    Serial.print(humedad);
    Serial.println("%");*/
    
    variable_invertida = 1024 - valor_leido;
    
    porcentaje = map(variable_invertida, 0, 694, 0, 100);
    Serial.print("Humedad del la planta en % es de =  ");
    Serial.print(porcentaje);
    Serial.println(" % ");
    
    JSONencoder["humidity"] = porcentaje;
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://plant-286601.ue.r.appspot.com/lastMeasurement");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(3000);  //Send a request every 30 seconds
 
}
