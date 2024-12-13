#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int pinoBotao = 18;  // numeração do pino do botão
int estadoDoBotao = 0;  // variável para leitura do estado do botão

const int pinoLDR = 4;  // numeração do pino do sensor LDR
int limite=600;

int ldrStatus = 0;

bool cicloAberto = true;

void modoEscuro();
void modoClaro();
void funcaoCicloAberto();
void funcaoCicloFechado();


void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);

  // Inicialização das entradas
  pinMode(pinoBotao, INPUT); // Inicialização do pino do botão como input de leitura
  pinMode(pinoLDR, INPUT); // Inicialização do pino do sensor LDR como input de leitura

  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Calebe S10+", "calebes10plus"); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  estadoDoBotao = digitalRead(pinoBotao);
  if (!cicloAberto) {
    if (estadoDoBotao == HIGH) {
      Serial.println("Botão pressionado!");
      delay(1000);

    } else {
      Serial.println("Botão não pressionado!");
    }
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String endpoint = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(endpoint.c_str());

    int codigoRespostaHTTP = http.GET(); // Código do Resultado da Requisição HTTP

    if (codigoRespostaHTTP>0) {
      Serial.print("Código da resposta HTTP: ");
      Serial.println(codigoRespostaHTTP);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(codigoRespostaHTTP);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {
  ldrStatus=analogRead(pinoLDR);

  if(cicloAberto){
    funcaoCicloAberto();
  } else {
    funcaoCicloFechado();
  }

  if(ldrStatus<=limite){
    Serial.print("Está escuro, ligar o LED: ");
    Serial.println(ldrStatus);
    modoEscuro();

  }else{
    Serial.print("Está claro, desligar o LED: ");
    cicloAberto = true;
    Serial.println(ldrStatus);
    modoClaro();

    estadoDoBotao = digitalRead(pinoBotao);
    if (!cicloAberto) {
      if (estadoDoBotao == HIGH) {
        Serial.println("Botão pressionado!");
        delay(1000);
        funcaoCicloAberto();
      } else {
        Serial.println("Botão não pressionado!");
      }
    }
  }
}

void modoEscuro(){
  digitalWrite(led_amarelo, HIGH);
  delay(1000);
  digitalWrite(led_amarelo, LOW);
}

void modoClaro(){
  digitalWrite(led_verde, HIGH);
  delay(3000);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_amarelo, HIGH);
  delay(2000);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, HIGH);
  delay(5000);
  digitalWrite(led_vermelho, LOW);
}

void funcaoCicloAberto(){
  digitalWrite(led_verde, HIGH);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);
}

void funcaoCicloFechado(){
  digitalWrite(led_verde, HIGH);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);
}