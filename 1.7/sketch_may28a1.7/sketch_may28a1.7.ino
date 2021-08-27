#include <genieArduino.h>
#include <DHT.h>
#include <Sensirion.h>
#include <Time.h>
#include <TimeLib.h>
#include <SD.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          Control dels sensors
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  //DHT22

#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temperatura;
float humitat;
float puntrosada;


  //GUVA-12SD

int sensorValue;
float voltatge;
float IndexUV;


  //FS200-SHT10


const uint8_t dataPin  =  2;
const uint8_t clockPin =  3;
float temperaturaS;
float humitatS;
float puntrosada2;
Sensirion tempSensor = Sensirion(dataPin, clockPin);




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          Control de l'hora
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

time_t t;
  // HORA

int buttonCount0 = 0;
int button_val0   = 0;
  // MES

int buttonCount1 = 0;
int button_val1 = 0;
  // SEGONS

int buttonCount2 = 0;
int button_val2 = 0;
  // DIA

int buttonCount3 = 0;
int button_val3 = 0;
  // MES

int buttonCount4 = 0;
int button_val4 = 0;
  // ANY

int buttonCount5 = 2021;
int button_val5 = 0;

  //ACTUALITZAR HORA
  
int button_val25 = 0;
int button_val26 = 0;
int buttonCount25 = 0;
int buttonCount26 = 0;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          Genie
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Genie genie;
#define RESETLINE 4



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          Control de la SD
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int CS = 9;
File file;

int button_val16 = 0;





void setup() 
{    
  Serial.begin(9600);
  genie.Begin(Serial);
  genie.AttachEventHandler(myGenieEventHandler); // Processa la data que ve de la pantalla
  
  
  pinMode(4, OUTPUT);  // Set D4 on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
  digitalWrite(4, 1);  // Reset the Display via D4
  delay(100);
  digitalWrite(4, 0);  // unReset the Display via D4
  delay (3500); //let the display start up
  
  //ARRENCAR DHT22
  
  dht.begin(); //iniciar dht22
  
  
  //HORA PREDETERMINADA

  setTime(0, 0, 0, 13, 6, 2021); //hora - minut - segon - dia - mes - any


  //ARRENCAR SD



  genie.WriteStr(8,"Sistema Iniciat");
  delay(1000);
    genie.WriteStr(8,"");
  genie.WriteStr(8, "Iniciant SD...");
  delay(1000);  
  if(!SD.begin(CS)){
    genie.WriteStr(8, "Error SD");
    }
  else{
    genie.WriteStr(8, "SD Operativa");
    }
   delay(1000);
   genie.WriteStr(8, "BENVINGUT, configuri l'hora");

    


/*   
 
  if(SD.exists("file.txt")){
    genie.WriteStr(8, "Arxiu amb el mateix nom ");
  if(SD.remove("file.txt") == true){
    genie.WriteStr(8, "ta' to reseteao");
    }
  else{
    genie.WriteStr(8, "pue no se a podio");
    }
  }
  
 */
}


void loop() 
{ 
  static long waitPeriod = millis();

  //////////////////////////////////////////////////////////////////////////
//                          (GUVA12SD) SENSOR INDEX UV
//////////////////////////////////////////////////////////////////////////



  //Voltatge
  
  int sensorValue = analogRead(A0);              // Llegir valor d'entrada al A0 (Analògic)
  float voltatge = (((sensorValue * (5.0 / 1023.0))*100));  // Convertir lectura analògica (de 0 fins a 1023) fins a un voltatge de (0 - 5V)
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 2, voltatge); // write voltage value to LedDigit2
  genie.WriteStr(2, voltatge);

  //Index UV
  
  float IndexUV = (voltatge/.1);
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 3, IndexUV); // write Index UV value to LedDigit3
  genie.WriteObject(GENIE_OBJ_LED_DIGITS, 12, IndexUV); // write Index UV value to LedDigit12
  genie.WriteStr(3, IndexUV);
    

  



 
//////////////////////////////////////////////////////////////////////////
//                          DHT22
//////////////////////////////////////////////////////////////////////////
  
  //LLegir del sensor cada 100ms (els sensor triguen a registrar les dades)
  if (millis() >= waitPeriod) 
  {

    
   temperatura=(float)dht.readTemperature();;

   temperatura = (temperatura * 100); // Celsius 00,00 per pantalla
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0, temperatura); //  temp value to LedDigit0
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 8, temperatura); //  temp  value to LedDigit8
   genie.WriteStr(0, temperatura);
   

   humitat=(float)dht.readHumidity();;

  
   humitat = (humitat * 100); //Lectura d'humitat 
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 1, humitat); //  humitat value to LedDigit1
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 9, humitat); //  hum  value to LedDigit8
   humitat = (humitat); //Lectura d'humitat 
   genie.WriteStr(1, humitat);

   // Punt de rosada
   puntrosada = (float)((temperatura - (100 - humitat)) / 5); 
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 7, puntrosada); // write insideTempSensorC value to LedDigit7
   genie.WriteStr(7, puntrosada);
   

  


//////////////////////////////////////////////////////////////////////////
//                          FS200-SHT10
//////////////////////////////////////////////////////////////////////////


    
   tempSensor.measure(&temperaturaS, &humitatS, &puntrosada2);

   temperaturaS = (temperaturaS * 100); // Celsius 00,00 per pantalla
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 4, temperaturaS); // write tempS value to LedDigit4
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 10, temperaturaS); // write tempS value to LedDigit4
   genie.WriteStr(4, temperaturaS);
   
   humitatS = (humitatS * 100); // Celsius 00,00 per pantalla
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 5, humitatS); // write humS value to LedDigit5
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 11, humitatS); // write humS value to LedDigit11
   genie.WriteStr(5, humitatS);

      puntrosada2 = (puntrosada2 * 100); // Celsius 00,00 per pantalla
   genie.WriteObject(GENIE_OBJ_LED_DIGITS, 6, puntrosada2); // write pt2 value to LedDigit6
   genie.WriteStr(6, puntrosada2);

   waitPeriod = millis() + 100; // rerun this code in another 100ms time. 
  

  
  genie.DoEvents(); // Do this every loop, checks the input buffer for data from the display and runs myGenieEventHandler()


//////////////////////////////////////////////////////////////////////////
//                          CONTROL DE L'HORA
//////////////////////////////////////////////////////////////////////////

// Declaració de variables temps
int t;
int minuts;
int segons;
int hores;
int dia;
int mes;
int any;

t = now();
hores  = hour();
minuts = minute();
segons = second();
dia = day();
mes = month();
any = year();

genie.WriteObject(GENIE_OBJ_LED_DIGITS, 16, hores);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 25, hores);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 31, hores);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 34, hores);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 37, hores);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 17, minuts);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 26, minuts);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 32, minuts);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 35, minuts);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 38, minuts);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 18, segons);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 27, segons);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 33, segons);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 36, segons);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 39, segons);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 22, dia);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 28, dia);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 23, mes);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 29, mes);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 24, any);
genie.WriteObject(GENIE_OBJ_LED_DIGITS, 30, any);



//////////////////////////////////////////////////////////////////////////
//                          CONTROL OPERATIVITAT USERBUTTONS
//////////////////////////////////////////////////////////////////////////

//DHT22

    if (temperatura > 00) {
     
      genie.WriteObject (GENIE_OBJ_USER_LED,0, 1);   
 
    } else {
  
      genie.WriteObject (GENIE_OBJ_USER_LED,0, 0);
 
    }

//FS200-SHT10

    if (temperaturaS > 00) {
     
      genie.WriteObject (GENIE_OBJ_USER_LED,1, 1);   
 
    } else {
  
      genie.WriteObject (GENIE_OBJ_USER_LED,1, 0);
 
    }

//GUVA-S12SD

    if (voltatge < 200) {
     
      genie.WriteObject (GENIE_OBJ_USER_LED,2, 1);   
 
    } else {
  
      genie.WriteObject (GENIE_OBJ_USER_LED,2, 0);
 
    }
}
}

//////////////////////////////////////////////////////////////////////////
//                          Bucle per captura dades SD
//////////////////////////////////////////////////////////////////////////

void writeFile(){
  file = SD.open("DADES.txt", FILE_WRITE);
  if(file){
    
    file.print("Hora," );
    file.print((int)hour());
    file.print(":");
    file.print((int)minute());
    file.print(":");
    file.print((int)second());
    file.print(",Data,");
    file.print((int)day());
    file.print("/");
    file.print((int)month());
    file.print("/");
    file.print((int)year());
    file.print(",Temperatura ambiental,");
    file.print(temperatura/100);
    file.print(",Humitat ambiental,");
    file.print(humitat/100);
    file.print(",Punt de rosada,");
    file.print(puntrosada/100);
    file.print(",Index UV,");
    file.print((((analogRead(A0)* (5.0 / 1023.0))*100)/.1)/100);
    file.print(",Temperatura del sòl,");
    file.print(temperaturaS/100);
    file.print(",Humitat del sòl,");
    file.print(humitatS/100);
    file.print(",Punt de rosada del sòl,");
    file.println(puntrosada2/100);   

    
    file.close();
  }
  else{
    genie.WriteStr(9,"No s'ha pogut enregistrar");
  }
}


//////////////////////////////////////////////////////////////////////////
//                          Bucle per guardar l'hora
//////////////////////////////////////////////////////////////////////////

void hora (){

 setTime(buttonCount0, buttonCount1, buttonCount2, buttonCount3, buttonCount4, buttonCount5);
}

void data (){

 setTime(buttonCount0, buttonCount1, buttonCount2, buttonCount3, buttonCount4, buttonCount5);
}

void myGenieEventHandler(void) 
{
  genieFrame Event;
  genie.DequeueEvent(&Event);


// Configurar hora


  // Hora ++
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 27)
      {
        button_val0 = genie.GetEventData(&Event);
        if(button_val0 == 0){
          buttonCount0++;  // increment button count when button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 13, buttonCount0); // Write the button count to the Leddigits
          }
      }
    }

    
    // Hora --
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 28)
      {
        button_val0 = genie.GetEventData(&Event);
        if(button_val0 == 0){
          buttonCount0--;  // reset the button count when reset button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 13, buttonCount0);  // Write a zero value to the Leddigits
          }
      }
    }
  }


  // Minuts ++
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 29)
      {
        button_val1 = genie.GetEventData(&Event);
        if(button_val1 == 0){
          buttonCount1++;  // increment button count when button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 14, buttonCount1); // Write the button count to the Leddigits
          }
      }
    }

    
    // Minuts --
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 30)
      {
        button_val1 = genie.GetEventData(&Event);
        if(button_val1 == 0){
          buttonCount1--;  // reset the button count when reset button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 14, buttonCount1);  // Write a zero value to the Leddigits
          }
      }
    }
  }


    // Segons ++
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 31)
      {
        button_val2 = genie.GetEventData(&Event);
        if(button_val2 == 0){
          buttonCount2++;  // increment button count when button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 15, buttonCount2); // Write the button count to the Leddigits
          }
      }
    }

    
    // Segons --
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 32)
      {
        button_val2 = genie.GetEventData(&Event);
        if(button_val2 == 0){
          buttonCount2--;  // reset the button count when reset button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 15, buttonCount2);  // Write a zero value to the Leddigits
          }
      }
    }
  }
    // Dia ++
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 35)
      {
        button_val3 = genie.GetEventData(&Event);
        if(button_val3 == 0){
          buttonCount3++;  // increment button count when button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 19, buttonCount3); // Write the button count to the Leddigits
          }
      }
    }

    
    // Dia --
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 36)
      {
        button_val3 = genie.GetEventData(&Event);
        if(button_val3 == 0){
          buttonCount3--;  // reset the button count when reset button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 19, buttonCount3);  // Write a zero value to the Leddigits
          }
      }
    }
  }


  // Mes ++
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 37)
      {
        button_val4 = genie.GetEventData(&Event);
        if(button_val4 == 0){
          buttonCount4++;  // increment button count when button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 20, buttonCount4); // Write the button count to the Leddigits
          }
      }
    }

    
    // Mes --
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 38)
      {
        button_val4 = genie.GetEventData(&Event);
        if(button_val4 == 0){
          buttonCount4--;  // reset the button count when reset button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 20, buttonCount4);  // Write a zero value to the Leddigits
          }
      }
    }
  }


    // Any ++
  if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
  {
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 39)
      {
        button_val5 = genie.GetEventData(&Event);
        if(button_val5 == 0){
          buttonCount5++;  // increment button count when button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 21, buttonCount5); // Write the button count to the Leddigits
          }
      }
    }

    
    // Any --
    if (Event.reportObject.object == GENIE_OBJ_WINBUTTON)
    {
      if (Event.reportObject.index == 40)
      {
        button_val5 = genie.GetEventData(&Event);
        if(button_val5 == 0){
          buttonCount5--;  // reset the button count when reset button is released
          genie.WriteObject(GENIE_OBJ_LED_DIGITS, 21, buttonCount5);  // Write a zero value to the Leddigits
          }
      }
    }

     if (Event.reportObject.index == 48)
      {
        button_val16 = genie.GetEventData(&Event);
        if(button_val16 == 0){
          writeFile();
          genie.WriteStr(10, "Dada guardada");
          delay(500);
          genie.WriteStr(10, "");
          }
      }

           if (Event.reportObject.index == 46)
      {
        button_val25 = genie.GetEventData(&Event);
        if(button_val25 == 0){
        hora();
          }
      }
           if (Event.reportObject.index == 47)
      {
        button_val26 = genie.GetEventData(&Event);
        if(button_val26 == 0){
        data();
          }
      } 
  }   
}
