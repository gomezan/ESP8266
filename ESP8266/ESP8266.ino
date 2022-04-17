#define THINGER_SERIAL_DEBUG

#include <ThingerESP8266.h>
#include "arduino_secrets.h"
#include <DHT.h>
 
#define DHTPIN D5
#define DHTTYPE DHT11

 float h = 0;
 float t = 0;
 int proximaMedida=8000;
 int intervalo=5000;
 
DHT dht(DHTPIN, DHTTYPE);

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
 
  Serial.begin(9600);
  dht.begin();
  
  pinMode(LED_BUILTIN, OUTPUT);

  // Credenciales de wi-fi
  thing.add_wifi(SSID, SSID_PASSWORD);

  //Basico
  thing["millis"] >> outputValue(millis());
  thing["humedad"] >> outputValue(h);
  thing["temperatura"] >> outputValue(t);

 //Estructura
  pson data;
  data["temperatura"] = t;
  data["humedad"] = h;

  //Estructura apuntador
  thing["Datos"] >> [] (pson& out){
    out["nodo"]=1;
    out["temp"]=t;
    out["hum"]=h;
  };

  //configuracion end-point
  thing.call_endpoint("primerEndpoint", data);
  
}

void loop() {
  thing.handle();

  if(millis() > proximaMedida){

     h = dht.readHumidity();
     t = dht.readTemperature();

     if (isnan(h) || isnan(t)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
       proximaMedida=millis()+intervalo;
      return;
     }
 
     Serial.print("Humedad: ");
     Serial.print(h);
     Serial.print(" %\t");
     Serial.print("Temperatura: ");
     Serial.print(t);
     Serial.println(" *C ");

     proximaMedida=millis()+intervalo;
  }
 
}
