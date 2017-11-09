#include <HologramSIMCOM.h>
#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 11  // Digital temperature and humidity sensor
#define DHTTYPE DHT11 // Temperature sensor model
#define LUXPIN A1  // the photo cell and 10K pulldown are connected to a0
#define POLPIN A5  // Analog Air Quality Control (NH3,Benzene,Alcohol,smoke)
#define RX_PIN 2  // SIM800 RX pin
#define TX_PIN 3  // SIM800 TX pin
#define RESET_PIN 4 // SIM800 reset pin
#define HOLO_KEY "rfL=>#O-"  //replace w/your SIM id

char mChar[100];
int hum;
int temp;
int lux;
int pol;

DHT dht(DHTPIN, DHTTYPE);
HologramSIMCOM Hologram(TX_PIN, RX_PIN, RESET_PIN, HOLO_KEY); // Instantiate Hologram

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Hologram.debug();
  
  Hologram.begin(9600);
  dht.begin();
}

void loop() {
  // Interact directly with the SIM800 module
  // Send AT commands and see Serial output from module
   Hologram.debug();

  // Read Humidity
  hum = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  temp = dht.readTemperature(true);
  // Reading temperature or humidity takes about 250 milliseconds!
  delay(250);

  // Read analog photocell sensor, returns illuminance lux value
  lux = floor(analogRead(LUXPIN)/10)*10;

  // Read analog value from MQ135 gas sensor (general air pollution)
  pol = analogRead(POLPIN); 

  createMessage();
  //Hologram.send(mChar);

  Serial.println(Hologram.gpsStatus());
  
  delay(10000);
}

// Create message
void createMessage() {
  // check if there are any new values before constructing message
  String message = "{\"lux\":";
  message += lux;

  message += ",\"temp\":";
  message += temp;

  message += ",\"hum\":";
  message += hum;

  message += ",\"pollution\":";
  message += pol;
  
  message += "}";
  
  message.toCharArray(mChar, 100);
}

