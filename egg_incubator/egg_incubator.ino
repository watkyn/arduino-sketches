// For the sensor
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// For the switch / LED
// Connect positive to pin SWITCHPIN
// Connect negative to 330 ohm resistor to negative on breadboard 
// Connect 5V to positive on breadboard


#include "DHT.h"

#define DHTPIN 5 // pin for the sensor
#define SWITCHPIN 13 // pin for the switch / LED
#define DHTTYPE DHT22   // DHT 22  (AM2302)

#define DESIRED_TEMP 101.0
#define LETHAL_TEMP 104.0
#define HUMIDITY_MINIMUM 15.0
#define HUMIDITY_MAXIMUM 60.0
#define SENSOR_READ_INTERVAL 15

// Morse Code signals
#define LOW_HUMIDITY ".-.."
#define HIGH_HUMIDITY "...."
#define HIGH_TEMP "-----"
#define MALFUNCTION "--"


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 

  // setup the power switch 
  pinMode(13, OUTPUT);

  // start the sensor
  dht.begin();
}


void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  float degreesInCelsius = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(degreesInCelsius) || isnan(humidity)) {
    sensorReadMalfunction();
  } else {
    float temp = degreesInCelsius * (9.0/5.0) + 32.0;

    // check if humidity is below minimum humidity
    if (humidity < HUMIDITY_MINIMUM) {
      morse(LOW_HUMIDITY);
    }

    if (humidity > HUMIDITY_MAXIMUM) {
      morse(HIGH_HUMIDITY);  // H for high humidity
    }

    if (temp > LETHAL_TEMP) {
      morse(HIGH_TEMP);
    }

    // check if temp is below desired temp 
    if (temp < DESIRED_TEMP) {
      turnOnLights(); 
    } else {
      turnOffLights();
    } 

    Serial.print("Humidity: "); 
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(temp);
    Serial.println(" *F");
  }
  
  delay (SENSOR_READ_INTERVAL * 1000); 
}


void sensorReadMalfunction() {
  Serial.println("Failed to read from DHT");
  morse(MALFUNCTION);  // M for malfunction
}

void morse(String code) {
   turnOffLights();
   delay(1000);
   for (int i = 0; i < code.length(); i++) {
      turnOnLights();
      if (code.charAt(i) == '-') {
        delay(1000);
      } else {
        delay(250);
      }
      turnOffLights();
      delay(100);         
   }
   delay(1000);
}

void turnOnLights() {
  digitalWrite(SWITCHPIN, HIGH);
}

void turnOffLights() {
  digitalWrite(SWITCHPIN, LOW);
}


