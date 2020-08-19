#include <SPI.h>
#include <WiFiNINA.h>
#include <SparkFunBME280.h>

BME280 sensor;

char ssid[] = "[INSERT SSID]"; //insert desired SSID
char pass[] = "[INSERT PASSWORD]"; //insert desired network password
unsigned long poll = 20000; //insert polling time (in ms)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

int wind_direction = 0;
unsigned long wind_speed = 0;
double wind_freq = 0.0;

float humidity;
float pressure;
float altitude;
float temp;
bool atmo = true;

int w_d_pin = A5;
int w_s_pin = 2;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(w_s_pin, INPUT);

  if (sensor.beginI2C() == false)
  {
    atmo = false;
  }

  while(status != WL_CONNECTED)
  {
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  server.begin();
}

void loop()
{
  WiFiClient client = server.available();
  while (client && client.connected())
  {
    if (client.available())
    {
      //wind speed portion
      //read in time between peaks
      wind_speed = pulseIn(w_s_pin, HIGH, 10000000);
      //manual catch for very low wind speed;
      //longest possible sample is 10 second long
      if(!wind_speed){client.print("Wind Speed: 0km/h\r\n");}
      else
      {
        //convert to seconds
        wind_freq = wind_speed/500000.0;
      
        //then convert to Hz
        wind_freq = 1/wind_freq;
      
        //sccording to the datasheet, the windspeed is (2.4*f)km/h
        wind_freq = wind_freq*2.4;
    
        client.print("Wind Speed: ");
        client.print(wind_freq);
        client.print("km/h\r\n");
      }
      
      //wind direction portion
      wind_direction = analogRead(w_d_pin);
    
      //2 layer decision tree (for speed and readability)
      client.print("Wind Direction: ");
      if (wind_direction < 150)
      {
          if (0 < wind_direction && wind_direction < 70){client.print("East-South-East (ESE)");}
          if (70 < wind_direction && wind_direction < 85){client.print("East-North-East (ENE)");}
          if (85 < wind_direction && wind_direction < 100){client.print("East (E)");}
          if (100 < wind_direction && wind_direction < 150){client.print("South-South-East (SSE");}
      }
      else if (wind_direction < 450)
      {
          if (150 < wind_direction && wind_direction < 200){client.print("South-East (SE)");}
          if (200 < wind_direction && wind_direction < 260){client.print("South-South-West (SSW)");}
          if (260 < wind_direction && wind_direction < 300){client.print("South (S)");}
          if (300 < wind_direction && wind_direction < 450){client.print("North-North-East (NNE)");}
      }
      else if (wind_direction < 750)
      {
          if (450 < wind_direction && wind_direction < 500){client.print("North-East (NE)");}
          if (500 < wind_direction && wind_direction < 610){client.print("West-South-West (WSW)");}
          if (610 < wind_direction && wind_direction < 650){client.print("South-West (SW)");}
          if (650 < wind_direction && wind_direction < 750){client.print("North-North-West (NNW)");}
      }
      else
      {
          if (750 < wind_direction && wind_direction < 800){client.print("North (N)");}
          if (800 < wind_direction && wind_direction < 850){client.print("West-North-West (WNW)");}
          if (850 < wind_direction && wind_direction < 900){client.print("North-West (NW)");}
          if (900 < wind_direction){client.print("West (W)");}
      }
    
      humidity = sensor.readFloatHumidity();
      pressure = sensor.readFloatPressure();
      altitude = sensor.readFloatAltitudeMeters();
      temp = sensor.readTempC();
      if (atmo)
      {
        client.print("\r\nHumidity(%RH): ");
        client.print(humidity);
    
        client.print("\r\nPressure(Pascals): ");
        client.print(pressure);
    
        client.print("\r\nAltitude(Meters): ");
        client.print(altitude);
    
        client.print("\r\nTemperature(Celsius): ");
        client.print(temp);
      }
      else
      {
        client.print("\r\n The atmospheric sensor is unresponsive.");
        if(sensor.beginI2C())
        {
          atmo = true;
        }
      }
      
      
      client.print("\r\n");
      //cycle every 20 seconds (accounting for sampling time)
      delay(poll-(wind_speed/1000));
    }
  }
}
