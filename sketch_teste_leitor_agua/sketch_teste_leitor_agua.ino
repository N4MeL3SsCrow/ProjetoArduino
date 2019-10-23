
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX <- Usando as portas digitais 2 e 3 como seriais para conectar no ESP8266

float teste1 = 0;
float teste2 = 0;
float teste3 = 0;

int Pulso; //Variável para a quantidade de pulsos
int j=0; //Variável para contagem
float vazaoagua; //Variável para armazenar o valor em L/min
float valormedia=0; //Variável para tirar a média a cada 1 minuto
float volumeAcumulado = 0;

//Variaveis para delay...
unsigned long timeAtu = 0;
unsigned long timeAnt = 0;
int myDelay = 1000; //Espera 1 segundo

void setup()
{ 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  
  pinMode(5, INPUT);
  attachInterrupt(5, incrpulso, RISING); //Configura a porta digital 5, para interrupção


  // Aguarda o ESP subir
  delay(2000);
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);

}


void loop ()
{
  Pulso = 0; //Começa do 0 variável para contar os giros das pás internas, ek segundos
  //sei(); //liga interrupção
  //delay (2000); //Espera 2 segundos
  //cli(); //Desliga interrupção
  
  //vazaoagua = Pulso / 5.5; //Converte para Litros/minuto
  
  vazaoagua = Pulso / 0.02; //Converte para Litros/segundos
  valormedia=valormedia+vazaoagua; //Soma a vazão para o calculo da valormedia
  j++;
  
  /*
  if(j==60)
  {
    valormedia = valormedia/60; //Tira a valormedia dividindo por 60
    Serial.print("\n Media por minuto = "); //Imprime a frase valormedia por minuto =
    Serial.print(valormedia); //Imprime o valor da valormedia
    Serial.println(" Litros/segundos - "); //Imprime L/min
    valormedia = 0; //Torna variável valormedia = 0, para uma nova contagem
    j=0; //Torna a variável 0,para uma nova contagem
  }
  */
  

  /* Evina dados para o ESP8266 a cada 500 milisegundos (myDelay) */
  timeAtu = millis();
  if (timeAtu > (timeAnt + myDelay)){

    timeAnt = timeAtu;

    Serial.print(vazaoagua); //Imprime na serial o valor da vazão
    Serial.println(" L/segundos"); //Imprime L/min
    
    volumeAcumulado = volumeAcumulado + vazaoagua;
    float custo = volumeAcumulado * 2.478;

    teste1 = teste1 + 10.1;
    teste2 = teste2 + 11.2;
    teste3 = teste3 + 12.3;
    

    Serial.println("");
    //Serial.println("#@{'vazao' : '" + String(vazaoagua) + " Litros/seg', 'volume' : '" + String(volumeAcumulado) +" Litros ', 'custo' : 'R$ " + String(custo) + "' }@#");
    //mySerial.print("#@{'vazao' : '" + String(vazaoagua) + " Litros/seg', 'volume' : '" + String(volumeAcumulado) +" Litros ', 'custo' : 'R$ " + String(custo) + "' }@#");

    //TESTE
    mySerial.print("#@{'vazao' : '" + String(teste1) + " Litros/seg', 'volume' : '" + String(teste2) +" Litros ', 'custo' : 'R$ " + String(teste3) + "' }@#");
    

  }

  // Redireciona a saida serial do ESP8266 para a saida do Arduino
  if (mySerial.available())
    Serial.write(mySerial.read());

  Serial.println("");

}


void incrpulso ()
{ 
Pulso++;
}
