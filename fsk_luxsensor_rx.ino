#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

unsigned long count = 0;
int check = 0;
int fre = 0;
int bit_count = 0;
int k = 0;
int data[] = {0, 0, 0, 0};

int count_bit3 = 0;
int count_bit2 = 0;
int count_bit1 = 0;
int count_bit0 = 0;

void setup(void) {
  Serial.begin(2000000);

  sensor_t sensor;
  tsl.getSensor(&sensor);

  if (!tsl.begin())
  {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  tsl.setGain(TSL2561_GAIN_16X);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);

}



void loop(void) {

  sensors_event_t event;
  tsl.getEvent(&event);
  int lux = event.light;
//  Serial.println(lux);
  lux = lux <= 30 ? 30 : lux;
  lux = lux >= 45 ? 45 : lux;
  
  if (lux == 45) {
    fre++;
    
  }
  else {
    check_bit(fre);
    fre = 0;
  }
//  Serial.println(lux);

  delay(1);
}

void check_bit(int i) {

  if (i > 12) {
    count_bit3++;
    if (count_bit3 == 2) {
      data[k] = 3;
      count_bit3 = 0;
      k++;
    }
  }
  else if (i >= 5 and i <= 8) {
    count_bit2++;
    if (count_bit2 == 4) {
      data[k] = 2;
      count_bit2 = 0;
      k++;
    }
  }
  else if (i >= 3 and i <= 4) {
    count_bit1++;
    if (count_bit1 == 8) {
      data[k] = 1;
      count_bit1 = 0;
      k++;
    }
  }
  else if (i >= 1 and i <= 2) {
    count_bit0++;
    if (count_bit0 >= 16) {
      data[k] = 0;
      count_bit0 = 0;
      k++;
    }
  }

  if (k == 4) {
    for (int i = 0 ; i < 4 ; i++) {
//      Serial.print(data[i]);
    }
//    Serial.println();
    k = 0;
    inttochar(data);
  }
}

void inttochar(int data[]){

    int i = 0;
    for (int j = 1 ; j < 5 ; j++){
      i |= data[j-1] << 2*j-2;
    }
    if(i == 0x1a){
      Serial.println();
    }else{
      Serial.print((char)i);
    }
   
}

