#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ArduinoJson.h"
#include <math.h>

ESP8266WebServer server(80);

#define PIN_SENSOR A0
#define PIN_RELE D5

const int DELAY = 2000; //2s 
const char *rede = "***omitido por seguranca***"; //ssid da rede
const char *senha = "***omitido por seguranca***"; //senha da rede

int valor;
int valor_porcento;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SENSOR, INPUT);
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);

  conectarWifi();
  rotas();

  server.begin();
}

void loop() {
  server.handleClient();
  valor = analogRead(PIN_SENSOR);
  valor_porcento = paraPorcento(valor);
  imprimeDado();

  if(valor_porcento >= 70){
    //liga
    digitalWrite(PIN_RELE, LOW);
  }else{
    //desliga
    digitalWrite(PIN_RELE, HIGH);
  }
  delay(DELAY);
}

void rotas(){
  server.on("/esp8266/dados", HTTP_GET, [](){
    enviarDados();  
  });
}


void conectarWifi(){
  Serial.print("Rede selecionada: ");
  Serial.println(rede);

  WiFi.begin(rede, senha);

  Serial.print("Conectando");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Conectado com sucesso! Endereco IP ESP8266 -> ");
  Serial.print(WiFi.localIP());
  Serial.println();
}

void enviarDados(){
  StaticJsonDocument<200> doc;
  doc["dados"]["valor_bruto"] = valor;
  doc["dados"]["valor_pct"] = paraPorcento(valor); //valor entre 0 e 1

  // Serialize o objeto JSON para uma string
  String jsonData;
  serializeJson(doc, jsonData);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", jsonData);
}

void imprimeDado(){
  Serial.print("Valores: ");
  Serial.print(valor);
  Serial.print(", ");
  Serial.print(valor_porcento);
  Serial.print("%");
  Serial.println();
}

int paraPorcento(int value){
  const int VALOR_MAXIMO = 1025; //Valor com solo seco
  const int VALOR_MINIMO = 390; //Valor com solo umido

  return map(value, VALOR_MINIMO, VALOR_MAXIMO, 100, 0);
}
