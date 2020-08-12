int wind_direction = 0;
unsigned long wind_speed = 0;
double wind_freq = 0.0;


int w_d_pin = A5;
int w_s_pin = 2;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(w_s_pin, INPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:

  //wind speed portion
  //read in time between peaks
  wind_speed = pulseIn(w_s_pin, HIGH, 10000000);
  //manual catch for very low wind speed;
  //longest possible sample is 10 second long
  if(!wind_speed){Serial.print("Wind Speed: 0km/h\r\n");}
  else
  {
  //Serial.print(wind_speed);
  //Serial.print("us|");
  //convert to seconds
  wind_freq = wind_speed/500000.0;
  
  //Serial.print(wind_freq);
  //Serial.print("s|");
  //then convert to Hz
  wind_freq = 1/wind_freq;
  
  //Serial.print(wind_freq);
  //Serial.print("Hz|");
  //sccording to the datasheet, the windspeed is (2.4*f)km/h
  wind_freq = wind_freq*2.4;

  Serial.print("Wind Speed: ");
  Serial.print(wind_freq);
  Serial.print("km/h\r\n");
  }
  
  //wind direction portion
  wind_direction = analogRead(w_d_pin);

  //2 layer decision tree (for speed and readability)
  //Serial.print(wind_direction);
  Serial.print("Wind Direction: ");
  if (wind_direction < 150)
  {
      if (0 < wind_direction && wind_direction < 70){Serial.print("East-South-East (ESE)");}
      if (70 < wind_direction && wind_direction < 85){Serial.print("East-North-East (ENE)");}
      if (85 < wind_direction && wind_direction < 100){Serial.print("East (E)");}
      if (100 < wind_direction && wind_direction < 150){Serial.print("South-South-East (SSE");}
  }
  else if (wind_direction < 450)
  {
      if (150 < wind_direction && wind_direction < 200){Serial.print("South-East (SE)");}
      if (200 < wind_direction && wind_direction < 260){Serial.print("South-South-West (SSW)");}
      if (260 < wind_direction && wind_direction < 300){Serial.print("South (S)");}
      if (300 < wind_direction && wind_direction < 450){Serial.print("North-North-East (NNE)");}
  }
  else if (wind_direction < 750)
  {
      if (450 < wind_direction && wind_direction < 500){Serial.print("North-East (NE)");}
      if (500 < wind_direction && wind_direction < 610){Serial.print("West-South-West (WSW)");}
      if (610 < wind_direction && wind_direction < 650){Serial.print("South-West (SW)");}
      if (650 < wind_direction && wind_direction < 750){Serial.print("North-North-West (NNW)");}
  }
  else
  {
      if (750 < wind_direction && wind_direction < 800){Serial.print("North (N)");}
      if (800 < wind_direction && wind_direction < 850){Serial.print("West-North-West (WNW)");}
      if (850 < wind_direction && wind_direction < 900){Serial.print("North-West (NW)");}
      if (900 < wind_direction){Serial.print("West (W)");}
  }
	
  Serial.print("\r\n");
  //cycle every 10 seconds (accounting for sampling time)
  delay(10000-(wind_speed/1000));

}
