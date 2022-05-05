/*
  NIBOburger library for ARDUINO
  License: BSD-License
  (c) 2015 by Nils Springob, nicai-systems
*/

#ifndef __NIBOBURGER_H__
#define __NIBOBURGER_H__


/*! @file    NIBOburger.h
 *  @brief   NIBOburger library for ARDUINO
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-06-21
 */


#if (!defined (_NIBOBURGER_)) && (!defined (ARDUINO_AVR_NIBOBURGER)) && (!defined (ARDUINO_AVR_NIBOBURGER_1284_15)) && (!defined (ARDUINO_AVR_NIBOBURGER_1284_20))
#error "NIBOburger library should be used with a NIBOburger robot as board!"
#endif

#include "niboburger/color.h"


#define ON 1
#define OFF 0

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4

/** 
 * \brief Die Klasse repräsentiert die Kernfunktionen des NIBO burger Roboters.
 * Es gibt genau ein Objekt dieser Klasse: <b>NIBOburger</b>
 */
class NIBOburgerClass {
  public:
    NIBOburgerClass();
    
    /** 
     * Initialisierung des NIBOburger Roboters
     */
    void begin();
    
    /**
     * Versorgungsspannung messen.
     * @return Spannung in Millivolt (4.8V ≙ 4800)
     */
    unsigned int getVoltage();
    
    /**
     * Zu überprüfende Spannung setzen, wird mit checkVoltage() überprüft
     * @param millivolt Spannung in Millivolt
     */
    void setMonitorVoltage(unsigned int millivolt);
    
    /**
     * Versorgungsspannung überprüfen, im Fehlerfall anhalten und blinken
     */
    void checkVoltage();
    
    /**
     * Alle LEDs auf einmal ein/ausschalten
     */
    void setLeds(bool led1, bool led2, bool led3, bool led4);
    
    /**
     * LED ein/ausschalten:
     * @param led LED1=1, LED2=2, LED3=3, LED4=4
     * @param value OFF=0=false, ON=1=true
     */
    void setLed(int led, int value);
    
    /**
     * LED abfragen:
     * @param led LED1=1, LED2=2, LED3=3, LED4=4
     * @return OFF=0=false, ON=1=true
     */
    int getLed(int led);
    
    /**
     * Warten bis ein neuer Satz Messwerte seit dem letzten Aufruf gemessen wurde.
     */
    void waitAnalogUpdate();

    /**
     * Rohwert eines analogen Kanals auslesen.
     * @param adc_channel Kanalnummer
     * @param active 0 = LED aus, 1 = LED an
     */
    unsigned int getAnalog(unsigned char adc_channel, unsigned char active=0);
    
    /**
     * aktuellen Tastenzustand auslesen
     */
    unsigned int getKeyState();
      
    /**
     * nächstes Tastaturereignis auslesen
     * @return kein Event: 0 \n Taste runtergedrückt: 'A', 'B' oder 'C' \n Taste losgelassen: 'a', 'b' oder 'c'
     */
    unsigned int getKeyChar();

    /** 
     * Zufallszahlen-Basis anhand des Rauschens der Sensorwerte liefern
     */
    unsigned int getRandomSeed();
    
    /**
     * Basis des Zufallszahlengenerators setzen. Bei gleichen Wert von seed werden die selben Zufallszahlen zurückgeliefert
     * @param seed Basiswert
     */
    void randomize(unsigned int seed);
    
    /**
     * Basis des Zufallszahlengenerators zufällig setzen. Entspricht dem Aufruf von randomize(getRandomSeed()).
     */
    void randomize();

    /**
     * Zufallszahl aus dem Interval [lo, hi] generieren. Alle Ganzzahl-Werte inklusive der Grenzen treten mit der gleichen Wahrscheinlichkeit auf.
     * @param lo untere Intervalgrenze
     * @param hi obere Intervalgrenze
     */
    int getRandomInt(int lo, int hi);
};

extern NIBOburgerClass NIBOburger;


/*---------------------------------------------------------------------------*/


    
/**
 * Eine einzelne Antriebsseite
 */    
class EnginePartClass {
  private:
    unsigned char id;
  public:
    EnginePartClass(unsigned char id);
    
    /**
     * Aktuellen Zählerstand auf 0 setzen
     */
    void resetTicks();

    /**
     * Aktueller Zählerwert des Rades in Ticks, im Precise Mode entsprechen 200 Ticks (Fast: 40 Ticks)
     * einer Radumdrehung (141 mm Strecke)
     */
    int getTicks();
    //int getSpeed();
};


/*---------------------------------------------------------------------------*/


    
/**
  * Die Klasse steht für den kompletten Antrieb des Roboters
  */    
class EngineClass {
  public:
    
    /**
     * linke Seite
     */    
    EnginePartClass left;
    
    /**
     * rechte Seite
     */    
    EnginePartClass right;
    EngineClass();
    
    /**
     * Initialisierung
     */
    void begin();
    
    /**
     * PWM Werte (-1023 ... 0 ... +1023) für die beiden Motoren setzen
     */    
    void setPWM(int left, int right);  
    
    /**
     * Geschwindigkeit für die beiden Motoren setzen. Die Werte werden in Ticks/Sekunde angegeben. Im Precise
     * Mode entsprechen 200 Ticks (Fast: 40 Ticks) einer Radumdrehung (141 mm Strecke)
     */    
    void setSpeed(int left, int right);
    
    /**
     * Zielposition für Räder setzen. Die Werte werden in Ticks angegeben. Im Precise Mode entsprechen 200 Ticks 
     * (Fast: 40 Ticks) einer Radumdrehung (141 mm Strecke)
     */    
    void setTargetAbs(int left, int right, unsigned int speed);
    
    /**
     * Zielposition für Räder setzen. Die Werte werden relativ zur aktuellen Position in Ticks angegeben. Im Precise 
     * Mode entsprechen 200 Ticks (Fast: 40 Ticks) einer Radumdrehung (141 mm Strecke)
     */    
    void setTargetRel(int left, int right, unsigned int speed);
    
    /**
     * Verbleibende Distanz zum Ziel bei Durchführung von setTargetAbs() bzw. setTargetRel(). Die Distanz ist der 
     * maximale Wert von den verbleibenden linken und rechten Ticks
     */    
    int getTargetDist();
    
    /**
     * Motoren anhalten (passiv Stoppen - rollt aus)
     */    
    void stop();
    
    /**
     * Motoren unverzüglich stoppen (aktiv Stoppen - bremst)
     */    
    void stopImmediate();
};

extern EngineClass Engine;


/*---------------------------------------------------------------------------*/

    
    /**
     * Farbe im RGB Farbmodell.
     * Die Farbe ist aus den drei Werten Rot- Grün- und Blauanteil zusammengesetzt.
     */    
class ColorRGB {
  private:
    unsigned long value;
  public:
    static const unsigned long CALIBRATE_BLACK  = COLOR_RGB_CAL_BLACK;  ///< 24 Bit RGB-Konstante für Kalibrier-Farbe: schwarz
    static const unsigned long CALIBRATE_WHITE  = COLOR_RGB_CAL_WHITE;  ///< 24 Bit RGB-Konstante für Kalibrier-Farbe: weiss
    static const unsigned long CALIBRATE_RED    = COLOR_RGB_CAL_RED;    ///< 24 Bit RGB-Konstante für Kalibrier-Farbe: rot
    static const unsigned long CALIBRATE_BLUE   = COLOR_RGB_CAL_BLUE;   ///< 24 Bit RGB-Konstante für Kalibrier-Farbe: blau
    static const unsigned long CALIBRATE_GREEN  = COLOR_RGB_CAL_GREEN;  ///< 24 Bit RGB-Konstante für Kalibrier-Farbe: grün
    static const unsigned long CALIBRATE_YELLOW = COLOR_RGB_CAL_YELLOW; ///< 24 Bit RGB-Konstante für Kalibrier-Farbe: gelb

    /**
     * Konstruktor aus einzelnen Farbkomponenten: Rot, Grün und Blau
     */    
    ColorRGB(unsigned char r, unsigned char g, unsigned char b);

    /**
     * Konstruktor für 24 Bit RGB-Farbwert Bsp. Rot: 0xff0000
     */    
    explicit ColorRGB(unsigned long val);

    /**
     * Destruktor
     */    
    ~ColorRGB();
    
    /**
     * Farbabweichung (RGB) zu zweiter RGB Farbe
     */    
    unsigned int diff(const ColorRGB & col2) const;
    
    /**
     * Farbabweichung (RGB) zu zweiter Farbe (als 24 Bit RGB-Farbwert)
     */    
    unsigned int diff(unsigned long val2) const;
};

/*---------------------------------------------------------------------------*/

    
    /**
     * Farbe im HSV Farbmodell.
     * Die Farbe ist aus den drei Werten Hue (Farbton), Saturation (Sättigung) und Value (Helligkeit) zusammengesetzt.
     */    
class ColorHSV {
  private:
    unsigned long value;
  public:
    static const unsigned long CALIBRATE_BLACK  = COLOR_HSV_CAL_BLACK;  ///< 24 Bit HSV-Konstante für Kalibrier-Farbe: schwarz
    static const unsigned long CALIBRATE_WHITE  = COLOR_HSV_CAL_WHITE;  ///< 24 Bit HSV-Konstante für Kalibrier-Farbe: weiss
    static const unsigned long CALIBRATE_RED    = COLOR_HSV_CAL_RED;    ///< 24 Bit HSV-Konstante für Kalibrier-Farbe: rot
    static const unsigned long CALIBRATE_BLUE   = COLOR_HSV_CAL_BLUE;   ///< 24 Bit HSV-Konstante für Kalibrier-Farbe: blau
    static const unsigned long CALIBRATE_GREEN  = COLOR_HSV_CAL_GREEN;  ///< 24 Bit HSV-Konstante für Kalibrier-Farbe: grün
    static const unsigned long CALIBRATE_YELLOW = COLOR_HSV_CAL_YELLOW; ///< 24 Bit HSV-Konstante für Kalibrier-Farbe: gelb

    /**
     * Konstruktor aus einzelnen Farbkomponenten: Hue, Saturation und Value
     */    
    ColorHSV(unsigned char h, unsigned char s, unsigned char v);

    /**
     * Konstruktor für 24 Bit HSV-Farbwert
     */    
    explicit ColorHSV(unsigned long val);

    /**
     * Destruktor
     */    
    ~ColorHSV();
    
    /**
     * Farbabweichung (HSV) zu zweiter HSV-Farbe
     */    
    unsigned int diff(const ColorHSV & col2, unsigned int hTol=0) const;
    
    /**
     * Farbabweichung (HSV) zu zweiter Farbe (als 24 Bit HSV-Farbwert)
     */    
    unsigned int diff(unsigned long val2, unsigned int hTol=0) const;
};

/*---------------------------------------------------------------------------*/

    
    /**
     * Eine Klasse für die drei Bodensensor BL, BC und BR. 
     * Mit dieser Klasse wird aus den drei einzelnen Sensoren ein kombinierter, der eine 
     * gemeinsame Kalibrierung und die Abfrage von Farbwerten ermöglicht.
     */    
class SurfaceSensorClass {
  public:
    SurfaceSensorClass();
    
    /**
     * Initialisierung
     */    
    void begin();
    
    /**
     * Messwert mit Index abfragen
     */    
    unsigned int get(unsigned char index, unsigned char absolute=0);
    
    /**
     * Messwert linker Bodensensor
     */    
    unsigned int getL(unsigned char absolute=0);
    
    /**
     * Messwert mittlerer Bodensensor
     */    
    unsigned int getC(unsigned char absolute=0);
    
    /**
     * Messwert rechter Bodensensor
     */    
    unsigned int getR(unsigned char absolute=0);
    
    /**
     * Messwert linker Bodensensor mit Licht vom mittleren Sensor
     */    
    unsigned int getCL(unsigned char absolute=0);
    
    /**
     * Messwert rechter Bodensensor mit Licht vom mittleren Sensor
     */    
    unsigned int getCR(unsigned char absolute=0);
    
    /**
     * RGB-Farbwert aus den drei Sensoren berechnen und zurückliefern
     */    
    ColorRGB getColorRGB();
    
    /**
     * HSV-Farbwert aus den drei Sensoren berechnen und zurückliefern
     */    
    ColorHSV getColorHSV();
    
    /**
     * Kalibrierung auf weissem Untergrund, entspricht dem Wert 4096 (0x1000)
     */    
    void calibrateWhite();
    
    /**
     * Kalibrierung auf schwarzem Untergrund, entspricht dem Wert 0 (0x0000)
     */    
    void calibrateBlack();
    
    /**
     * Kalibrierdaten im EEPROM speichern
     */    
    void storeCalibration();
};

extern SurfaceSensorClass SurfaceSensor;



/*---------------------------------------------------------------------------*/


enum {
  SENSOR_DISABLED = 0,
  SENSOR_PULSE    = 1,
  SENSOR_LOW      = 2,
  SENSOR_HIGH     = 3
};

/**
 * Klasse für die Sensor-Bricks in den Sensor-Slots
 */
class SensorClass {
  private:
    unsigned char id;
  public:
    SensorClass(unsigned char id);
    
    /**
     * Initialisierung
     */    
    void begin();
    
    /**
     * Sensor-Modus setzen:
     * * SENSOR_DISABLED: Digitaler Pin frei
     * * SENSOR_PULSE: Digitaler Pin Pulssignal
     * * SENSOR_LOW: Digitaler Pin 0 Volt
     * * SENSOR_HIGH: Digitaler Pin 5 Volt
     */    
    void activate(unsigned char mode);
    
    /**
     * Digitalen Pin freigeben
     */    
    void deactivate();
    
    /**
     * Sensor abfragen
     */    
    unsigned int get(unsigned char absolute=0);
    //unsigned int getRaw(unsigned char variant);
};

    
/**
* Sensor-Brick im Sensor-Slot FL
*/    
extern SensorClass SensorFL;
    
/**
* Sensor-Brick im Sensor-Slot FR
*/    
extern SensorClass SensorFR;
    
/**
* Sensor-Brick im Sensor-Slot FLL
*/    
extern SensorClass SensorFLL;
    
/**
* Sensor-Brick im Sensor-Slot FRR
*/    
extern SensorClass SensorFRR;
    
/**
* Sensor-Brick im Sensor-Slot BL
*/    
extern SensorClass SensorBL;
    
/**
* Sensor-Brick im Sensor-Slot BC
*/    
extern SensorClass SensorBC;
    
/**
* Sensor-Brick im Sensor-Slot BR
*/    
extern SensorClass SensorBR;


#endif
