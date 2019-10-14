int Pulso; //Variável para a quantidade de pulsos
int j=0; //Variável para contagem
float vazaoagua; //Variável para armazenar o valor em L/min
float valormedia=0; //Variável para tirar a média a cada 1 minuto

void setup()
{ 
Serial.begin(9600);
pinMode(5, INPUT);
attachInterrupt(0, incrpulso, RISING); //Configura a porta digital 2, para interrupção
}


void loop ()
{
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

}


void incrpulso ()
{ 
Pulso++;
}
