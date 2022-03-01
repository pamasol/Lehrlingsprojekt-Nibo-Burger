#include <niboburger/robomain.h>
#include <stdlib.h>
#include <string.h>

#include "maroon.h"

// Linienfolge für schwarze Linie auf hellem Boden

// mittel-grau:
//#define FLOOR_VALUE 30

// hell-grau:
//#define FLOOR_VALUE 40
#define FLOOR_VALUE 40

// x > FLOOR_VALUE : Boden
// x < FLOOR_VALUE : Linie

int8_t stear; // linie links: >0 / linie rechts: <0

volatile int8_t state;
uint8_t color;
uint8_t colstabcnt;
uint8_t halted;

uint8_t dispmode;
uint8_t maroon_mode;
uint8_t maroon_cnt;
uint16_t ignore_color;

uint8_t obst_max;

uint8_t display_cnt = 50;


/* diese Funktion wird automatisch aufgerufen. Der Aufruf erfolgt 
   immer nachdem alle analogen Kanäle einen neuen Wert gemessen
   haben. Da die Funktion so häufig aufgerufen wird, sollte sie 
   möglichst kurz sein und keine Wartezeiten (delay(), etc...)
   oder ausgiebige Berechnungen enthalten!
*/

static void check_color(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t col=0;
  if (r<10) {
    // maybe blue
    if ((g>30)&&(g<70)&&(b>50)) {
      // is blue
      col=1;
    }
  } else if (r>50) {
    // maybe red
    if ((g<20)&&(b<20)) {
      // is red
      col=2;
    }    
  }
  if (color==col) {
    if (col) {
      if (colstabcnt<255) {
        colstabcnt++;
      }
    } else {
      colstabcnt /= 2;
    }
  } else {
    if (col==0) {
      colstabcnt /= 2;
    }
    color = col;
  }
}


void analog_irq_hook() {
  /*

  */
  uint8_t bcl = min(127, surface_get(SURFACE_CL)/8);
  uint8_t bl  = min(127, surface_get(SURFACE_L) /8);
  uint8_t bc  = min(127, surface_get(SURFACE_C) /8);
  uint8_t br  = min(127, surface_get(SURFACE_R) /8);
  uint8_t bcr = min(127, surface_get(SURFACE_CR)/8);
    
  int8_t dir = (int8_t)bcr-(int8_t)bcl;
  stear = constrain(dir, -64, +64);
  
  if ((dir < -12)||(dir > +12)) {
    color = 0;
  } else {
    check_color(br, bc, bl);
  }
//  uint16_t bcl = analog_getValueExt(ANALOG_BCL,2);
//  uint16_t bc = analog_getValueExt(ANALOG_BC,2);
//  uint16_t bcr = analog_getValueExt(ANALOG_BCR,2);
  
  if (bc<FLOOR_VALUE/4) {
    state = 0; // Linie liegt sehr mittig!
  } else if (bc<FLOOR_VALUE) {
    // Linie detektiert
    if (bcl<bcr)  state = -1; // Linie liegt links
    if (bcl>bcr)  state = +1; // Linie liegt rechts
    if (bcl==bcr) state =  0; // Linie liegt mittig
  } else {
    // Linie verloren
    if (state==-1) state = -2; // Linie lag vorher links
    if (state==0)  state = +2; // Linie lag vorher mittig -> Raten
    if (state==+1) state = +2; // Linie lag vorher rechts
    /*
    if (min(bcl,bcr) < 10) {
      if (bcl < bcr) {
        state = -2;
      } else if (bcr < bcl) {
        state = +2;
      } else {
        state = 0;
      }
    }
    */
  }

  switch (color) {
    case 0: led_setall(0,0,0,0); break;
    case 1: led_setall(0,1,1,0); break;
    case 2: led_setall(1,0,0,1); break;
  }
  /*
  switch (state) {
    case -2: led_setall(1,0,0,0); break;
    case -1: led_setall(0,1,0,0); break;
    case  0: led_setall(0,1,1,0); break;
    case +1: led_setall(0,0,1,0); break;
    case +2: led_setall(0,0,0,1); break;
  }
  */
}


/* Zeichen für das maroon SHIELD, abhängig vom aktuellen Sensorwert */


/* 8 Bars für das maroon SHIELD zusammenbauen */



static void display_ArrowL() {
  usart_write(MAROON_LOAD() MAROON_GFX("182442e72424243c") MAROON_TXBACK("."));
}

static void display_ArrowR() {
  usart_write(MAROON_LOAD() MAROON_GFX("3c242424e7422418") MAROON_TXBACK("."));
}

static void display_ArrowD() {
  usart_write(MAROON_LOAD() MAROON_GFX("10305f81815f3010") MAROON_TXBACK("."));
}

static void display_X() {
  usart_write(MAROON_LOAD() MAROON_GFX("8142241818244281") MAROON_TXBACK("."));
}

void displaySym(char c) {
  static char lastSym = 0;
  if (c==lastSym) {
    display_cnt=0;
    return;
  }
  lastSym = c;
  switch (c) {
    case 'X': display_X(); break;
    case 'L': display_ArrowL(); break;
    case 'R': display_ArrowR(); break;
    case 'D': display_ArrowD(); break;    
  }
}

void updateDisplay() {
  if (usart_txempty()) {
    if (obst_max>8) {
      displaySym('X');
    } else if (state==-2) {
      displaySym('R');
    } else if (state==+2) {
      displaySym('L');
    } else {
      displaySym('D');
    }
  }
}

void led_blink(uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4) {
  cli();
  for (uint8_t i=0; i<5; i++) {
    led_setall(l1,l2,l3,l4);
    delay(100);
    led_setall(0,0,0,0);
    delay(100);
  }
  sei();
}

void calibrate() {
  led_blink(1,1,1,1);
  while (1) {
    char c = key_get_char();
    if (c=='a') {
      delay(200);
      surface_calibrateBlack();
      led_blink(1,0,0,0);
    } else if  (c=='b') {
      delay(200);
      surface_calibrateWhite();
      led_blink(0,1,0,0);
    } else if  (c=='c') {
      surface_writePersistent();
      return;
    }
  }
}

void setup() {
  led_init();
  analog_init();
  motpwm_init();
  surface_init();
  usart_setbaudrate(38400);
  usart_enable();
  delay(200);
  usart_write(MAROON_FONT_PROP() "Follow Line\n");
  // für Linienfolge grüne Seitensensoren einschalten (Licht vom mittleren 
  // Sensor (BC), Messwerte von den äußeren Sensoren (BL+BR))
  analog_setExtToggleMode(ANALOG_BCL, 1);
  analog_setExtToggleMode(ANALOG_BCR, 1);
  delay(100);
  // auf einen Tastendruck warten...
  while (1) {
    char c = key_get_char();
    if (c=='c') {
      calibrate();
      dispmode=2;
      break;
    } else if (c=='a') {
      dispmode=1;
      break;
    } else if (c=='b') {
      dispmode=2;
      break;
    }    
  }
  usart_write(MAROON_IMM_CLEAR());
  led_blink(0,1,1,0);
}


uint16_t loose_cnt = 550;




uint8_t show_maroon_process(uint8_t pos) {
  memset(maroon_gfxdata+MAROON_BAR_OFFSET, '0', 8);
  if (pos==0) {
    ;
  } else if (pos==1) {
    maroon_gfxdata[MAROON_BAR_OFFSET+0] = 'M';
    maroon_gfxdata[MAROON_BAR_OFFSET+7] = 'M';    
  } else if (pos<8) {
    pos-=2;
    maroon_gfxdata[MAROON_BAR_OFFSET+1+pos] = '1';
    maroon_gfxdata[MAROON_BAR_OFFSET+7] = '2'+pos;
    maroon_gfxdata[MAROON_BAR_OFFSET+6-pos] = '8';
    maroon_gfxdata[MAROON_BAR_OFFSET+0] = '7'-pos;
  } else if (pos==8) {
    maroon_gfxdata[MAROON_BAR_OFFSET+1] = 'L';
    maroon_gfxdata[MAROON_BAR_OFFSET+6] = 'L';
  } else if (pos==9) {
    maroon_gfxdata[MAROON_BAR_OFFSET+2] = 'K';
    maroon_gfxdata[MAROON_BAR_OFFSET+5] = 'K';
  } else if (pos==10) {
    maroon_gfxdata[MAROON_BAR_OFFSET+3] = 'J';
    maroon_gfxdata[MAROON_BAR_OFFSET+4] = 'J';
  }     
    
    
  if (pos==1) {
    usart_write(MAROON_BRIGHT(1));
  } else if (pos==8) {
    usart_write(MAROON_BRIGHT(6));
  } else if (pos==9) {
    usart_write(MAROON_BRIGHT(9));
  } else if (pos==10) {
    usart_write(MAROON_BRIGHT(*));
  }
  usart_write(maroon_gfxdata);
  return 1;
}

void showAnimation() {
  for (unsigned int i=1; i<=7; i++) {
    show_maroon_process(i);
    delay(50);
  }
  for (unsigned int i=1; i<=7; i++) {
    show_maroon_process(i);
    delay(50);
  }
  for (unsigned int i=8; i<=11; i++) {
    show_maroon_process(i);
    delay(50);
  }
  delay(100);
}

void maroon_rx_wait(char c) {
  while (1) {
    if (!usart_rxempty()) {
      if (usart_getchar()==c) {
        return;
      }
    }
  }
}

void maroon_rx_drain() {
  while (!usart_rxempty()) {
    usart_getchar();
  }
}

void bot_halt(int16_t speed) {
  motpwm_setLeft(speed); 
  motpwm_setRight(speed);
  delay(200);
  speed/=2;
  motpwm_setLeft(speed); 
  motpwm_setRight(speed);
  delay(200);
  speed/=2;
  motpwm_setLeft(speed); 
  motpwm_setRight(speed);
  delay(200);
  speed=0;
  motpwm_setLeft(speed); 
  motpwm_setRight(speed);

  maroon_rx_drain();
  if (color==1) {
    showAnimation();
    usart_write(" BLUE home!\n" MAROON_TXBACK("~"));
    maroon_rx_wait('~');
  } else if (color==2) {
    showAnimation();
    usart_write(" RED home!\n" MAROON_TXBACK("~"));
    maroon_rx_wait('~');
  } else {
    // nothing...
    usart_write(MAROON_CLEAR() "?\n" MAROON_TXBACK("~"));
    maroon_rx_wait('~');
    delay(100);
    displaySym(0);
    return;
  }
  
  while (key_get_char()) {
  }
  uint8_t pos=1;
  for (uint16_t i=0; i<600; i++) {
    // wait two minutes or button pressed...
    show_maroon_process(pos++);
    if (pos==8) pos=1;
    delay(200);
    if (key_get_char()) {
      break;
    }
  }
  ignore_color=500;
  maroon_rx_drain();
  usart_write(MAROON_BRIGHT(*)" GO!\n" MAROON_TXBACK("~"));
  maroon_rx_wait('~');
  delay(100);
  displaySym(0);
}


void loop() {
  nibo_checkMonitorVoltage();
  // auf neue Messwerte warten...
  analog_wait_update();
  // nach jedem 25. Durchlauf, das Display aktualisieren
  if (display_cnt-- == 0) {
    display_cnt = 25;
    if (dispmode==1) {
      updateDisplay();
    } else if (dispmode==2) {
      updateDisplay2();
    }
  }
  
  int16_t left=0, right=0;

  // Geschwindigkeiten in Abhängigkeit vom aktuellen Linienstatus setzen
  switch (state) {
    case -2: left=-600; right= 900; break;
    case -1: left= 600; right= 1000; loose_cnt = 550; break; // loose_cnt zurücksetzen
    case  0: left= 900-stear; right= 900+stear; loose_cnt = 550; break; // loose_cnt zurücksetzen
    case +1: left= 1000; right= 600; loose_cnt = 550; break; // loose_cnt zurücksetzen
    case +2: left= 900; right=-600; break;
  }
  
  // Wenn die Linie verloren wurde, kurz warten und danach eine Zeit
  // lang den Roboter drehen, um die Linie wiederzufinden.
  if (loose_cnt>500) {
    // warten...
    loose_cnt--;
  } else if (loose_cnt) {
    // auf der Stelle drehen...
    loose_cnt--;    
    if (state>0) {left=+800; right=-800;}
    if (state<0) {left=-800; right=+800;}
  } else {
    // Linie endgültig verloren :-(
    left = right = 0;
  }
  
  if (ignore_color) {
    ignore_color--;
  } else {
  
    if (color) {
      left /= 2;
      right /= 2;
    }
  
    if (colstabcnt>=8) {
      bot_halt((left+right)/2);
      left=right=0;
    }
  }
  
  uint8_t l  = min(127, analog_getValueExt(ANALOG_FL, 2) /8);
  uint8_t r  = min(127, analog_getValueExt(ANALOG_FR, 2) /8);
  uint8_t ll = min(127, analog_getValueExt(ANALOG_FLL, 2)/8);
  uint8_t rr = min(127, analog_getValueExt(ANALOG_FRR, 2)/8);
  
  obst_max = max4(l,r,ll,rr);
  if (obst_max>8) {
    // Hindernis
    left = right = 0;
  }
  
  // Leistung der Motoren setzen:
  motpwm_setLeft(left); 
  motpwm_setRight(right);
}
