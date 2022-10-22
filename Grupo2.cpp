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
bool sono_buffer;
char estadoTostadora[20];

void setup()
{
	pinMode(AZUL, OUTPUT);
	pinMode(VERDE, OUTPUT);
	pinMode(ROJO, OUTPUT);
	pinMode(BTN_LEVEL_1, OUTPUT);
	pinMode(BTN_LEVEL_2, OUTPUT);
	pinMode(BTN_LEVEL_3, OUTPUT);
	pinMode(BTN_LEVEL_4, OUTPUT);
	pinMode(pinBuzzer, OUTPUT);
	Serial.begin(9600);
	myLcd.begin(16, 2);
	tempAnterior = 19;
	sono_buffer = false;
	strcpy(estadoTostadora, "No encendido");
}

void prederLed(int led)
{
	switch (led)
	{
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

void sonarBuzzer(int repeticiones)
{
	for (int i = 0; i < repeticiones; i += 1)
	{
		digitalWrite(pinBuzzer, HIGH);
		delay(500);
		digitalWrite(pinBuzzer, LOW);
		delay(500);
	}
}

void loop()
{
	if (digitalRead(BTN_LEVEL_1) == HIGH)
	{
		nivel = 1;
	}
	else if (digitalRead(BTN_LEVEL_2) == HIGH)
	{
		nivel = 2;
	}
	else if (digitalRead(BTN_LEVEL_3) == HIGH)
	{
		nivel = 3;
	}
	else if (digitalRead(BTN_LEVEL_4) == HIGH)
	{
		nivel = 4;
	}

	Serial.println(nivel);

	int temperatura = map(analogRead(SENSOR_TPM), 20, 350, 0, 49);

	if (nivel == 1)
	{
		prederLed(AZUL);
		sono_buffer = false;
		strcpy(estadoTostadora, "Sin hacer");
	}
	else if (nivel == 2)
	{
		prederLed(VERDE);
		temperatura = map(analogRead(SENSOR_TPM), 20, 350, 50, 99);
		sono_buffer = false;
		strcpy(estadoTostadora, "Hecha");
	}
	else if (nivel == 3)
	{
		prederLed(VERDE);
		temperatura = map(analogRead(SENSOR_TPM), 20, 350, 100, 149);
		sono_buffer = false;
		strcpy(estadoTostadora, "Hecha");
	}
	else if (nivel == 4)
	{
		prederLed(ROJO);
		temperatura = map(analogRead(SENSOR_TPM), 20, 350, 150, 199);
		strcpy(estadoTostadora, "Quemada");
		if (!sono_buffer)
		{
			sonarBuzzer(3);
			sono_buffer = true;
		}
	}
	if (tempAnterior != temperatura)
	{
		myLcd.clear();
		tempAnterior = temperatura;
	}

	char lcdLine1[20];
	char lcdLine2[20];

	sprintf(lcdLine1, "Nivel %d", nivel);
	sprintf(lcdLine2, "%s", estadoTostadora);

	// Serial.println(lcdLine);
	myLcd.setCursor(0, 0);
	myLcd.print(lcdLine1);
	myLcd.setCursor(0, 1);
	myLcd.print(lcdLine2);
}