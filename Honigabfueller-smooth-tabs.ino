/*
  Honigabfüll -Automat  by Dennis Deutschmann  06/2020
*/


// Includes
#include <Servo.h> // Bibliothek für den Öffner Servo
#include <HX711_ADC.h> // https://github.com/olkal/HX711_ADC
#include <Wire.h>
//  #include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C library
#include <U8g2lib.h>
#include <Arduino.h>
#include "variablen.h";

// Display
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Button Code
//const int buttonPin = 2;                // Start Button an Pin "2"
//const int buttonPin2 = 3;               // ABBRUCH Knopf an Pin "3"
int buttonState = 0;                    // Button Status Variablen
int buttonState2 = 0;

// Code für den Servo
Servo myservo;                          // Servo Objekt erzeugen
int val;                                // Variable für die Servo Position (20 - 160 in Grad)
// int servoport = 9;                      // Da hängt der Servo dran

// Wägezelle
HX711_ADC Waage(4, 5);                  // Parameter: dt pin, sck pin
//float kalib = 419.9;                    // Variable für Kalibrierung - nur für aktuellen Aufbau
float messwert1;                        // Variable für ungenullten Messwert
float messwert2;                        // Honiggewicht minus Glas
float honig_minus = honig - sicherheitsgewicht;
//const float honig = 500.0;              // Ziel nettogewicht Honigglas
//const int glas_min = 200;               // Mindestgewicht, welches ein Glas haben muss
float aktuellesGlas;                    // Gewicht des aktuell aufgestellten Glases
int disp;                               // Variable für den Ladebalken

// LC Display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // nicht ändern - ist für das 0.96 " OLED Display

// Display Funktion mit 3 Zeilen
void drawDisplay(const char *line1, const char *line2 = "", const char *line3 = "")
{
  u8g2.clearBuffer();                   // clear the internal memory
  u8g2.setFont(u8g2_font_6x12_tf);      // choose a suitable font
  u8g2.drawStr(0,20,line1);             // Definition erste Zeile
  u8g2.setFont(u8g2_font_6x12_tf);      // Ändern bei Bedarf
  u8g2.drawStr(0,40,line2);             // Zeile 2
  u8g2.drawStr(0,60,line3);             // Zeile 3
  u8g2.sendBuffer();                    // internen Speicher aufs Display schicken
  
}

// Display Funktion mit 2 Zeilen und Balken für die Anzeige des Füllstands als Balken
void drawDisplay2(const char *line1, const char *line2 = "", int *line3 = 20)
{
  u8g2.clearBuffer();                   // clear the internal memory
  u8g2.setFont(u8g2_font_6x12_tf);   // choose a suitable font
  u8g2.drawStr(0,20,line1);             // Definition erste Zeile
  u8g2.setFont(u8g2_font_6x12_tf);   // Ändern bei Bedarf
  u8g2.drawStr(0,40,line2);             // Zeile 2
  u8g2.setDrawColor(1);                 // Farbe setzen
  u8g2.drawBox(0,60,line3,15);          // Zeichnet einen Balken in bestimmter Länge
  u8g2.sendBuffer();                    // internen Speicher aufs Display schicken
  
}

// sonstige Variablen
int vorgang;                            // Variable um den Vorgang zu starten und zu beenden

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
    vorgang = 0;                        // Zur Sicherheit am Anfang auf Null setzen
 
    u8g2.begin();                         // setzt das Display auf Bereitschaft
    u8g2.clearBuffer();                   // clear the internal memory

    // Button:
    pinMode(buttonPin, INPUT);
    pinMode(buttonPin2, INPUT);

    myservo.attach(servoport);            // Servo gehört an diesen Port
    val = servo_min;                      // zu Beginn Servo auf ZU Stellung 
    myservo.write(val);

    // Waage initialisieren
    Waage.begin();                        // start connection to HX711
    Waage.start(2000);                    // load cells gets 2000ms of time to stabilize
    Waage.setCalFactor(kalib);            // calibration factor for load cell => strongly dependent on your individual setup
    Waage.update();
    messwert1 = Waage.getData();          // schreibt gemessenen Wert in Variable

    String text = String(messwert1) + "g";                          // Umwandlung Zahl in String zur Ausgabe
    drawDisplay("Loading...", "Aktuell messe ich:",text.c_str());   // ...und dann alles ausgeben lassen
 
} ////////////////// setup zu  ////////////////////////////////////

void loop() {

  Waage.update();                                                      // Waage Daten holen
  messwert1 = Waage.getData();                                         // Waage auslesen und in Variable speichern

  String text = String(messwert1) + "g";  
  drawDisplay("Glas aufstellen", "Dann Knopf dr\201cken",text.c_str());  

  buttonState = digitalRead(buttonPin);                                // Button lesen, der soll den Ablauf starten
  buttonState2 = digitalRead(buttonPin2);                              // Abbuch Knopf bereit machen

  // Wenn der Knopf 1 gedrück ist...

  if (buttonState == HIGH) {
      vorgang = 1;                              // Abfüllvorgang starten - vorgang = 1 bedeutet, es läuft. Später auf 0 stellen...
       // Debug:
      //DebugPrint();
      } 
  else {
      vorgang = 0;
       }

  while (vorgang == 1) {                           // Wenn Vorgang gestartet wurde...:
      Waage.update();                              // Waage Daten holen
      messwert1 = Waage.getData();          

      if (messwert1 < glas_min) {                  // nachsehen, ob ein Glas auf der Waage steht, nicht dass es Sauerei gibt
          // debug
          //DebugPrint();
          drawDisplay("ACHTUNG", "Kein Glas erkannt!","...Du Depp...");
          vorgang = 0;                              // wieder zurück setzen
          buttonState = LOW;
          delay(2000);
        } // if glas min zu
       
      
      else {
          // einmal aktuell wiegen - messwert1 sollte jetzt das Glas sein.
          Waage.update();                             // Waage Daten holen
          messwert1 = Waage.getData();                // Waage auslesen und in messwert1 speichern
          aktuellesGlas = messwert1;                  // aktuelles Gewicht ist das Gewicht des Glases ohne Honig   
          messwert2 = messwert1 - aktuellesGlas;       
          
          String text2 = String(messwert2) + "g";
          String servo = "Servowert: " + String(val); //Dies zeigt den aktuellen Servowert an und dient zum DEBUG
          drawDisplay("Es geht gleich los",servo.c_str(),text2.c_str());
                    
while (messwert2 < honig_minus) {
     
      Waage.update();
      messwert1 = Waage.getData();
      messwert2 = messwert1 - aktuellesGlas;
      buttonState2 = digitalRead(buttonPin2);
      if (buttonState2 == HIGH) { 
        messwert2 = honig;
        val = servo_min; 
        }
      val = map(messwert2, 0, honig_minus, servo_max, servo_min );         // übersetzt das Gewicht des Honigs (messwert2) in den Servowert (140-20)
      disp = map(messwert2, 0, honig_minus, 0, 100 );         // übersetzt das Gewicht des Honigs (messwert2) in die Länge für den Lade-Balkens
      myservo.write(val);                             // Servo stellt sich auf die Position ein, die map vorgibt
      //DebugPrint();
      String text2 = String(messwert2) + "g";
      drawDisplay2("Honig fliesst",text2.c_str(),disp);
      }

    if (messwert2 >= honig_minus)
    {
        val = servo_min;
        myservo.write(val);
        //Serial.print("FERTIG - Schotten dicht");
        String text2 = String(messwert2) + "g";
        drawDisplay("Glas voll","aktuelles Glasgewicht:",text2.c_str());
        delay(2000);
        vorgang = 0; // alles auf Anfang
    }

     } // else zu
  } // while vorgang == 1 zu
} // LOOP zu
