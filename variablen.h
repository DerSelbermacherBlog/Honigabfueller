///////////////////////////////////////////////
// Alles was die Waage und Gewichte betrifft //
// Bei Bedarf ändern                         //
///////////////////////////////////////////////

const int glas_min = 200;               // ab wieviel Gewicht soll die Waage ein leeres Honigglas erkennen (bei mir 200g)
const float kalib = 419.9;                    // Variable für Kalibrierung - nur für aktuellen Aufbau
const float honig = 500.0;              // Ziel nettogewicht Honigglas / Füllung (normalerweise 500g)
const int sicherheitsgewicht = 5;       // Grammzahl, die der Honigverschluss vor erreichen des Zielgewichts schließen soll

///////////////////////////////////////////////
// Hier nun ein paar Dinge zu deinem Arduino //
///////////////////////////////////////////////

const int servoport = 9;                      // Da hängt der Servo dran
const int buttonPin = 2;                // Start Button an Pin "2"
const int buttonPin2 = 3;               // ABBRUCH Knopf an Pin "3"

// Achtung: hängt mit deinem Servo zusammen, hier meine Werte mit meinem Servo

const int servo_min = 20;               // bei welchem Wert ist der Servo auf "ZU" Position?
const int servo_max = 140;              // bei welchem Wert ist der Servo auf Maximal "AUF" Stellung?


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// Was hier kommt ist für Dich nicht wichtig, dienst nur der Ordnung in der Hauptdatei //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void DebugPrint() {                       // für Debug Zwecke
  Serial.print("Vorgang Variable: ");
  Serial.println(vorgang);
  Serial.println();
 
  Serial.print("Waage: ");
  Serial.println(messwert1);
  Serial.println();
 
  Serial.print("Buttons: ");
  Serial.println(buttonState);
  Serial.println();
  Serial.println(buttonState2);
  Serial.println();
 
  Serial.print("Servo: ");
  Serial.println(val);
  Serial.println();
}

*/
