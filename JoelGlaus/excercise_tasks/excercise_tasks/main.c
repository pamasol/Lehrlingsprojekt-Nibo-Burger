/* Includes f√ºr den Zugriff auf die NIBO Burger Bibliothek */
#include "niboburger/robomain.h"

void setup()
{
	/* Programmcode zum einmaligen Setup des Roboters */
<<<<<<< HEAD
	led_init();									//Anweisung um Led zu initialisieren
=======
	led_init();				//Anweisung um Led zu initialisieren
>>>>>>> 39c2f1189949c07f8c443ea6779c34caf6c09363
}

void loop()
{
<<<<<<< HEAD
	/* Programmcode, den der NIBO Burger immer wieder ausf¸hren soll */
	int ledNr;
	for (ledNr=1; ledNr<=4; ledNr+=1)			// f¸hrt Anweisungen mehrmals durch
	{
		led_set(ledNr, 1);						// Led auf Zustand 1 setzen
		delay(1000);							// Zeitverzˆgerung um 1000ms um Vorgang besser zu beobachten
	}
}
=======
	/* Programmcode, den der NIBO Burger immer wieder ausf√ºhren soll */
	led_set(1,1);				//Led 1 (rote LED) auf Zustand 1 setzen
	delay(500);					//Zeitverz√∂gerung um 500ms
	led_set(1,0);				//Led 1 (rote LED) auf Zustand 0 setzen
	delay(500);					//Zeitverz√∂gerung um 500ms
}
>>>>>>> 39c2f1189949c07f8c443ea6779c34caf6c09363
