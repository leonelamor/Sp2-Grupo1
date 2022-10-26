#define AZUL 4
#define VERDE 3
#define ROJO 2
#define SENSOR_TPM A5
#define BTN_LEVEL_1 6
#define BTN_LEVEL_2 7
#define BTN_LEVEL_3 8
#define BTN_LEVEL_4 9
#define pinBuzzer 10


#include <LiquidCrystal.h>
LiquidCrystal myLcd(A0, A1, A2, A3, A4, 5);
int nivel = 0;
int tempAnterior;
int nivelAnterior;
bool sono_buffer; 
char estadoTostadora[20];


void setup(){
  	pinMode(AZUL, OUTPUT);
  	pinMode(VERDE, OUTPUT);
  	pinMode(ROJO, OUTPUT);
   	pinMode(BTN_LEVEL_1, OUTPUT);
  	pinMode(BTN_LEVEL_2, OUTPUT);
  	pinMode(BTN_LEVEL_3, OUTPUT);
  	pinMode(BTN_LEVEL_4, OUTPUT);
    pinMode(pinBuzzer, OUTPUT);
	Serial.begin(9600);
    myLcd.begin(16,2);
  	tempAnterior = 19;
  	sono_buffer = false;
  	strcpy(estadoTostadora, "No encendido");
}

void prederLed(int led){
  switch(led){
  	case AZUL:
    	digitalWrite(VERDE, LOW);
        digitalWrite(ROJO, LOW);
    	break;
   	case VERDE:
    	digitalWrite(AZUL, LOW);
        digitalWrite(ROJO, LOW);
    	break;
   	case ROJO:
    	digitalWrite(VERDE, LOW);
        digitalWrite(AZUL, LOW);
    	break;
  };
  digitalWrite(led, HIGH);
}



void loop(){
  if(digitalRead(BTN_LEVEL_1) == HIGH){
  	nivel = 1;
  } else if(digitalRead(BTN_LEVEL_2) == HIGH){
  	nivel = 2;
  } else if(digitalRead(BTN_LEVEL_3) == HIGH){
  	nivel = 3;
  } else if(digitalRead(BTN_LEVEL_4) == HIGH){
  	nivel = 4;
  }
  
  
  int temperatura = map(analogRead(SENSOR_TPM), 20, 350, -40, 125);
  
  if(nivel == 1){
    controlarEstadoTostada(0,10,temperatura);
  }else if(nivel == 2){
    controlarEstadoTostada(20,30,temperatura);
  }else if(nivel == 3){
    controlarEstadoTostada(40,60,temperatura);
  }else if(nivel == 4){
    controlarEstadoTostada(80,110,temperatura);
  }
   
  
 if(tempAnterior != temperatura || nivelAnterior != nivel){
   myLcd.clear();
   if (nivelAnterior != nivel && strcmp(estadoTostadora, "Quemada") == 0)
   {
     sono_buffer = false;
   }
   tempAnterior = temperatura;
   nivelAnterior = nivel;
 }
  
  char lcdLine1[20];
  char lcdLine2[20];
  sprintf(lcdLine1,"Nivel %d", nivel);
  sprintf(lcdLine2,"%s", estadoTostadora);

  myLcd.setCursor(0,0);
  myLcd.print(lcdLine1);
  myLcd.setCursor(0,1);
  myLcd.print(lcdLine2);
  
  
  if (strcmp(estadoTostadora, "Quemada") != 0)
  {
    sono_buffer = false;
    
  }else if(!sono_buffer)
  {
    sonarBuzzer(3);
    sono_buffer = true;
  }
}


void sonarBuzzer(int repeticiones){
  for (int i = 0; i < repeticiones; i += 1){
    digitalWrite(pinBuzzer,HIGH);
    delay(500);
    digitalWrite(pinBuzzer,LOW);
    delay(500);
  }
}


void controlarEstadoTostada(int tempUno, int tempDos, int temperatura)
{
  if (temperatura < tempUno)
  {
    prederLed(AZUL);
    strcpy(estadoTostadora, "Sin hacer");
  }else if (temperatura < tempDos)
  {
    prederLed(VERDE);
    strcpy(estadoTostadora, "Hecha");
  }else
  {
    prederLed(ROJO);
    strcpy(estadoTostadora, "Quemada");
  }
}
