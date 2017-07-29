#include <OneWire.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <Adafruit_Sensor.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <SoftwareSerial.h> 
#include <TinyGPS.h> 
#include <DallasTemperature.h> 
#include<Arduino.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; 
EthernetClient client;


#define DHTPIN 7         //define as DHTPIN the Pin 3 used to connect the Sensor
#define DHTTYPE DHT11    //define the sensor used(DHT11)
#define ONE_WIRE_BUS 2    
DHT dht(DHTPIN, DHTTYPE);

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);


TinyGPS gps;
SoftwareSerial ss(19, 18);
const int xpin = A3;  // x-axis of the accelerometer
const int ypin = A2;  // y-axis
const int zpin = A1;  // z-axis (only on 3-axis models)
const int ecgpin = A4;
volatile int Signal;  // holds the incoming raw data

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = true; 
unsigned long myChannelNumber = 255736; //thingspeak channel
const char * myWriteAPIKey = "6YCO1FPJAWQH56R7"; //thingspeak key

void setup()

  {

  Serial.begin(115200); //Begin serial communication ss.begin(9600);
  Serial.println("healthcare"); //Print a message
  sensors.begin();
  Ethernet.begin(mac);
  //dht.begin(); 
  ThingSpeak.begin(client);

  void interruptSetup(); // sets up to read Pulse Sensor signal every 2mS

  pinMode(10, INPUT); // Setup for leads off detection LO + pinMode(11, INPUT); // Setup for leads off detection LO -
}

void loop() {

  float moisture, fahrenheit, pulse, acc, results[2], x, y,z; // Send the command to get temperatures
  fahrenheit = temperature (); Serial.print(fahrenheit); Serial.print(" Fahrenheit ");  
  moisture = humidity();
    {
    if( moisture >0)
      {
      z = moisture; Serial.print(z); Serial.print(" moisture ");
      }

    else
      {   
      z= 69; Serial.print(z);
      Serial.print(" moisture ");
      }
    }   
  acc = acclerometer (); Serial.print(acc); Serial.print(" Acc ");
  pulse = ECG(); Serial.print(pulse); Serial.println(" BPM ");
  getData(results); if ( results[0] > 0 )
      {

      if ( results[1]> 0 )
        { 
          x = results[0]; Serial.print( x ); 
          Serial.print(" lat "); 
          y = results[1] ; Serial.print( y ); 
          Serial.println(" lon ");
        }       
      }   

//  Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
//  pieces of information in a channel. Here, we write to field 1.

    ThingSpeak.setField(1, fahrenheit );
    ThingSpeak.setField(2, z );
    ThingSpeak.setField(3, pulse );
    ThingSpeak.setField(4, acc );
    ThingSpeak.setField(5, x );
    ThingSpeak.setField(6, y );

    
//  Write the fields that you've set all at once. 
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//  ThingSpeak will only accept updates every 15 seconds.

              }

int temperature ()
  {
  sensors.requestTemperatures();   // reading Humidity 
  float t = sensors.getTempCByIndex(0); 
  float temp, fahrenheit;
  temp = t; fahrenheit = t * 1.8 + 32.0;
  return fahrenheit;
  }

int humidity()
  {
  float h = dht.readHumidity();    // reading Humidity
  return h; 
  }

float acclerometer()
{
  float acc,xpin,ypin,zpin; xpin= analogRead(xpin); ypin= analogRead(ypin); zpin= analogRead(zpin);
  xpin = map(xpin, 0, 1023, 0, 255); ypin = map(ypin, 0, 1023, 0, 255); zpin = map(zpin, 0, 1023, 0, 255);
  acc = ( xpin + ypin + zpin)/3; return acc;
}

float ECG()
  {
  float ecg;
  if((digitalRead(10) == 1)||(digitalRead(11) == 1))
  {Serial.println('!');}
  else
  {ecg = analogRead(ecgpin); return ecg;}
  delay(1);} 

void getData(float *dest)
  {
  bool newData = false; float flat, flon;
// For one second we parse GPS data and report some key values 
for (unsigned long start = millis(); millis() - start < 1000;)
  {
  while (ss.available())
  {
    char c = ss.read();
// Serial.write(c); // uncomment this line if you want to see the GPS data flowing
  if (gps.encode(c)) // Did a new valid sentence come in? 
  newData = true;
  }
  }

if (newData)
{
  gps.f_get_position(&flat, &flon);
  flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6; flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6;
}

dest[0] = flat; dest[1] = flon; 
  }

