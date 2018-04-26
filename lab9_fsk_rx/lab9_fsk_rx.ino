#include "Adafruit_ADS1015.h"
#include "Adafruit_MCP4725.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define r_slope 5.3

#define f0min 1500
#define f0max 1600

#define f1min 900
#define f1max 980

#define f2min 580
#define f2max 620

#define f3min 380
#define f3max 410



int _max = 0, i = 0, check_count = 0;
int prev = 0;
int output = -1;
int c[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int sum[] = {0, 0};
int keep,loop_pulse = 0;;
int pulse[10];
int sum_count = 0 , start_count = 0;;
unsigned long duration = 0;
unsigned long start_time;
unsigned long stop_time;
int check = 0;
int bit0,bit1,bit2,bit3;
int sum_bit = 0;

void setup() {
  Serial.begin(115200);
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  cls();
  cli();//stop interrupts

//set timer interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);


  sei();//allow interrupts


}

ISR(TIMER2_COMPA_vect){//timer1 interrupt 8kHz 
//generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
  int tmp = analogRead(A0);
  if (tmp - prev > r_slope && check == 0) {
    _max = 0;
    check = 1;
  }

  if (tmp > _max) {
    _max = tmp;
  }
    
  if (_max - tmp > r_slope) {
    
    if (check) {
          sum_bit += pulse[loop_pulse] = pulseIn(A0,1);
//          Serial.println(pulse[loop_pulse]);
//          Serial.print("  ");
          loop_pulse++;
          
          
          if(loop_pulse == 10){
            loop_pulse = 0;
            bit0 = (pulse[0] + pulse[1]) / 2;
            bit1 = (pulse[2] + pulse[3] + pulse[4]) / 3;
            bit2 = (pulse[5] + pulse[6]) / 2;
            bit3 = (pulse[7] + pulse[8] + pulse[9]) / 3;  

//            Serial.print(check_bit(bit0));
//            Serial.print(" ");
//            Serial.print(check_bit(bit1));
//            Serial.print(" ");
//            Serial.print(check_bit(bit2));
//            Serial.print(" ");
//            Serial.print(check_bit(bit3));
//            Serial.println(" ");

            int res;
            res |= check_bit(bit3) << 6;
            res |= check_bit(bit2) << 4;
            res |= check_bit(bit1) << 2;
            res |= check_bit(bit0) << 0;

            Serial.print(char(res));
            Serial.print("   ");
            Serial.println(res,BIN);
            
          }
     }
      _max = 0;
  }
  prev = tmp;

}


void loop() {


}

int check_bit(int bitx){
  if(bitx > 1500 and bitx < 1600){
    return 0;
  }else if(bitx > 900 and bitx < 980){
    return 1;
  }else if(bitx > 580 and bitx < 620){
    return 2;
  }else if(bitx > 380 and bitx < 410){
    return 3;
  }
}

void cls(){
  for(int i = 0 ; i < 10 ; i++){
      pulse[i];
  }
}

//
   
