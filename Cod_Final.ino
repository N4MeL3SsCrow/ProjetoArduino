 #include "ESP8266.h"
    #include "SoftwareSerial.h"
        
    // Cria uma serial nas portas 2 (RX) e 3 (TX)
    SoftwareSerial minhaSerial(2,3); 
    // Define que o modulo ira utilizar a serial minhaSerial
    ESP8266 wifi(minhaSerial);
    // Configuracao ID e senha da rede Wireless
    #define SSID        "LIVE TIM_9C09_2G"
    #define PASSWORD    "ttkdnjrjgk"
    int Pulso; //Variável para a quantidade de pulsos
    int j=0; //Variável para contagem
    float vazaoagua; //Variável para armazenar o valor em L/min
    float valormedia=0; //Variável para tirar a média a cada 1 minuto
    pinMode(5, INPUT);
    attachInterrupt(0, incrpulso, RISING); //Configura a porta digital 2, para interrupção
    
    int status = WL_IDLE_STATUS; //STATUS TEMPORÁRIO ATRIBUÍDO QUANDO O WIFI É INICIALIZADO E PERMANECE ATIVO
    //ATÉ QUE O NÚMERO DE TENTATIVAS EXPIRE (RESULTANDO EM WL_NO_SHIELD) OU QUE UMA CONEXÃO SEJA ESTABELECIDA
    //(RESULTANDO EM WL_CONNECTED)
    WiFiEspServer server(80); //CONEXÃO REALIZADA NA PORTA 80
    
    RingBuffer buf(8); //BUFFER PARA AUMENTAR A VELOCIDADE E REDUZIR A ALOCAÇÃO DE MEMÓRIA

void setup() {
        Serial.begin(9600); 
         if(WiFi.status() == WL_NO_SHIELD){
    while (true);
  }
  while(status != WL_CONNECTED){
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
  //FIM - VERIFICA SE O ESP8266 ESTÁ CONECTADO AO ARDUINO, CONECTA A REDE SEM FIO E INICIA O WEBSERVER
  
}

void loop() {
 
Pulso = 0; //Começa do 0 variável para contar os giros das pás internas,ek segundos
sei(); //liga interrupção
delay (2000); //Espera 2 segundos
cli(); //Desliga interrupção

vazaoagua = Pulso / 5.5; //Converte para Litros/minuto
valormedia=valormedia+vazaoagua; //Soma a vazão para o calculo da valormedia
j++;

Serial.print(vazaoagua); //Imprime na serial o valor da vazão
Serial.print(" L/minuto - "); //Imprime L/min
if(j==60)
{
valormedia = valormedia/60; //Tira a valormedia dividindo por 60
Serial.print("\n Media por minuto = "); //Imprime a frase valormedia por minuto =
Serial.print(valormedia); //Imprime o valor da valormedia
Serial.println(" Litros/minutos - "); //Imprime L/min
valormedia = 0; //Torna variável valormedia = 0, para uma nova contagem
j=0; //Torna a variável 0,para uma nova contagem
}
  WiFiEspClient client = server.available(); //ATENDE AS SOLICITAÇÕES DO CLIENTE

  if (client) { //SE CLIENTE TENTAR SE CONECTAR, FAZ
    buf.init(); //INICIALIZA O BUFFER
    while (client.connected()){ //ENQUANTO O CLIENTE ESTIVER CONECTADO, FAZ
      if(client.available()){ //SE EXISTIR REQUISIÇÃO DO CLIENTE, FAZ
        char c = client.read(); //LÊ A REQUISIÇÃO DO CLIENTE
        buf.push(c); //BUFFER ARMAZENA A REQUISIÇÃO

        //IDENTIFICA O FIM DA REQUISIÇÃO HTTP E ENVIA UMA RESPOSTA
        if(buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }

        
//MÉTODO DE RESPOSTA A REQUISIÇÃO HTTP DO CLIENTE
void sendHttpResponse(WiFiEspClient client){
  client.println("HTTP/1.1 200 OK"); //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
  client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
  client.println("");
  client.println("<!DOCTYPE HTML>"); //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
  client.println("<html>"); //ABRE A TAG "html"
  client.println("<head>"); //ABRE A TAG "head"
  client.println("<link rel='icon' type='image/png' href='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/icon_mws.png'/>"); //DEFINIÇÃO DO ICONE DA PÁGINA
  client.println("<link rel='stylesheet' type='text/css' href='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/webpagecss.css' />"); //REFERENCIA AO ARQUIVO CSS (FOLHAS DE ESTILO)
  client.println("<title>MasterWalker Shop - Modulo WiFi ESP8266 para Arduino</title>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("</head>"); //FECHA A TAG "head"
  
  //AS LINHAS ABAIXO CRIAM A PÁGINA HTML
  client.println("<body>"); //ABRE A TAG "body"
  client.println("<img alt='masterwalkershop' src='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/logo_mws.png' height='156' width='700' />"); //LOGO DA MASTERWALKER SHOP
  client.println("<p style='line-height:2'><font>Modulo WiFi ESP8266 para Arduino</font></p>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("<font>ESTADO ATUAL DO LED</font>"); //ESCREVE O TEXTO NA PÁGINA
  
//  if (statusLed == HIGH){ //SE VARIÁVEL FOR IGUAL A HIGH (1), FAZ
//    client.println("<p style='line-height:0'><font color='green'>LIGADO</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
//    client.println("<a href=\"/L\">APAGAR</a>"); //COMANDO PARA APAGAR O LED (PASSA O PARÂMETRO /L)
//  }else{ //SENÃO, FAZ
//    if (statusLed == LOW){ //SE VARIÁVEL FOR IGUAL A LOW (0), FAZ
//    client.println("<p style='line-height:0'><font color='red'>DESLIGADO</font></p>"); //ESCREVE "DESLIGADO" NA PÁGINA
//    client.println("<a href=\"/H\">ACENDER</a>"); //COMANDO PARA ACENDER O LED (PASSA O PARÂMETRO /H)
//    }
//  }
  client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
  client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
  client.println("</body>"); //FECHA A TAG "body"
  client.println("</html>"); //FECHA A TAG "html"
  delay(1); //INTERVALO DE 1 MILISSEGUNDO
}


void incrpulso ()
{ 
Pulso++;
}
