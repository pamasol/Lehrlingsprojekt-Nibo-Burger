/*! \mainpage Übersicht
 *
 * \section intro_sec Einleitung
 *
 * Dies ist die Dokumentation der C Bibliothek f&uuml;r den <a href="http://burger.nicai.eu" target="_blank">NIBO burger Roboter</a>.
 * Weitere Informationen befinden sich im <a href="http://www.nibo-roboter.de/wiki/NIBO burger" target="_blank">Wiki</a> und auf der
 * <a href="http://sourceforge.net/projects/nibo/" target="_blank">SourceForge Projektseite der NiboRoboLib</a>
 *
 * Die enthaltenen .lib Dateien benötigen WinAVR-20100110, siehe <a href="http://www.nibo-roboter.de/wiki/NIBObee/FAQ" target="_blank">FAQ</a>!
 *
 * Die Bibliothek besteht aus folgenden Teilen:
 * - <b>libniboburger_class.a</b>
 *  - NIBOburger.h - C++ Klassen
 *
 * - <b>libniboburger_base.a</b> (verwendet ADC, Timer1, INT0 und INT1)
 *  - analog.h - analoge Eing&auml;nge
 *  - base.h - Basisfunktionen / Initialisierung
 *  - clock.h - Funktionen zur genauen Zeiterfassung/Verz&ouml;gerung (optional)
 *  - iodefs.h - digitale Ein-/Ausg&auml;nge allgemein
 *  - iodefs_niboburger.h - digitale Ein-/Ausgänge Zuordnung der Port-Pins
 *  - led.h - Ein-/Ausschalten der LEDs
 *  - motpwm.h - Ansteuerung der Motoren
 *  - odometry.h - Auswertung der Drehimpulse der Räder
 *  - key.h - Taster
 *  - color.h - Funktionen für Farbwerte im RGB und HSV Farbmodell
 *  - utils.h - Praktische Hilfs-Funktionen (min/max/constrain, etc...)
 *
 * - <b>libniboburger_surface.a</b>
 *  - surface.h - kalibrierter Boden/Farbsensor
 *
 * - <b>libniboburger_pid.a</b> (verwendet Timer1)
 *  - motpid.h - PID-Motorregelung
 *
 * - <b>libniboburger_usart.a</b> (verwendet USART0)
 *  - usart.h - serielle Schnittstelle (X13)
 *
 * - <b>libniboburger_utils.a</b>
 *  - delay.h - Funktionen zur Verzögerung
 *
 * 
 * @author Nils Springob, nicai-systems, Stolberg, Germany
 * @author Contributions by mifritscher, BirgerT, Georg and several other people.
 */


