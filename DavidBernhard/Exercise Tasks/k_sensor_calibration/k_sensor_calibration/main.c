#include <niboburger/robomain.h>

unsigned long previousMillis = 0;
unsigned long interval = 5000;
unsigned char tick = 0;

void setup() {
	//  NIBOburger.begin();
	//  NIBOburger.checkVoltage();
	
	//  initialize serial communication at 38400 bits per second:
	Serial.begin(38400);            // Maroon
	//  Serial.begin(9600,SERIAL_8N1);  // XBEE @ BrayTerm
	
	delay(1000);
	delay(1000);
	delay(1000);
	//    Fragezeichen Fade In / Fade Out
	Serial.write("\33d0\33l?\33T*100 \33P1000 \33T0100 \33d*");
	//  Serial.print("\33d0\33l?\33T*100 \33P1000 \33T0100 \33d*");
	
}

void loop() {
	//  interrupts();
	//  char key = NIBOburger.getKeyChar();
	
	//  switch (key) {
	//    case 'A':
	//      NIBOburger.setLed(LED1, ON);
	////      Serial.write("\33d0\33l?\33T*100 \33P1000 \33T0100 \33d*");
	////      Serial.print("\33d0\33l?\33T*100 \33P1000 \33T0100 \33d*");
	//      break;
	//
	//    case 'B':
	//      NIBOburger.setLed(LED2, ON);
	//      NIBOburger.setLed(LED3, ON);
	//      break;
	//
	//    case 'C':
	//      NIBOburger.setLed(LED4, ON);
	// //     Serial.write("hello\33t!");
	// //     Serial.write("hello\n");
	//      Serial.print("hello\n");
	//     break;
	//
	//    case 'a':
	//      NIBOburger.setLed(LED1, OFF);
	//      break;
	//
	//    case 'b':
	//      NIBOburger.setLed(LED2, OFF);
	//      NIBOburger.setLed(LED3, OFF);
	//      break;
	//
	//    case 'c':
	//      NIBOburger.setLed(LED4, OFF);
	//      break;
	//  }
	
	unsigned long currentMillis = millis();
	
	if(currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;
		
		interval = 100;     // Stress
		
		//      XBEE Kommunikation Testen
		//      Serial.println(tick++,DEC);
		
		//   Maroon Samples aus dem Wiki
		
		//    Fragezeichen Fade In / Fade Out
		//    Serial.write("\33d0\33l?\33T*100 \33P1000 \33T0100 \33d*");
		//      Serial.print("\33d0\33l?\33T*100 \33P1000 \33T0100 \33d*");
		
		//    Schachbrett Muster
		if(tick++ %2) {
			//        Serial.write("\33l\33Gaa55aa55aa55aa55 ");
			Serial.write("\33l\33G0055aa55aa55aa00 ");
		}
		else {
			Serial.write("\33l\33G55aa55aa55aa55aa ");
		}
	}
	
	
	delay(1);        // delay in between reads for stability
}