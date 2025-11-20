#include "DHT.h"

// === DEFINIÇÕES DE PINOS ===
#define DHTPIN 13
#define DHTTYPE DHT22
#define LEDPIN 2
#define BUZZERPIN 4
#define LDRPIN 35
#define TRIGPIN 12
#define ECHOPIN 14

DHT dht(DHTPIN, DHTTYPE);

// === SETUP INICIAL ===
void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  dht.begin();

  Serial.println("Sistema iniciado. Monitorando fadiga e postura...");
}

// === FUNÇÃO DE MEDIR DISTÂNCIA ===
float medirDistancia() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  long duracao = pulseIn(ECHOPIN, HIGH);
  float distancia = duracao * 0.034 / 2;
  return distancia;
}

// === LOOP PRINCIPAL ===
void loop() {
  float temperature = dht.readTemperature();
  int lightValue = analogRead(LDRPIN);
  float distancia = medirDistancia();

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C | Luz: ");
  Serial.print(lightValue);
  Serial.print(" | Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Alerta de fadiga (calor + pouca luz)
  if (temperature > 28 && lightValue < 200) {
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(BUZZERPIN, HIGH);
    Serial.println("[ALERTA] Ambiente desconfortável. Pausa recomendada.");
  }
  // Alerta de postura (muito perto da tela)
  else if (distancia < 30) {
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(BUZZERPIN, HIGH);
    Serial.println("[ALERTA] Se afaste do computador e endireite sua coluna.");
  }
  else {
    digitalWrite(LEDPIN, LOW);
    digitalWrite(BUZZERPIN, LOW);
  }

  delay(2000);
}