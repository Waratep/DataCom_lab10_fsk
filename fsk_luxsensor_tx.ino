#define delay0 30
#define delay1 60
#define delay2 120
#define delay3 240

int state;    

void setup() {
  Serial.begin(115200);

}

void loop() {

    int i = 0, x = 0;
    char inData = "";

//     1 char / 3840 sec
    
    if (Serial.available() > 0) {

    inData = Serial.read();
    Serial.print("data : ");
    Serial.print(inData);
    Serial.print("   0x");
    Serial.println(inData, HEX);
    state = !state;


    if (state and inData != 0x0D and inData != 0x0A) {
      for (int k = 0; k < 4; k++) { //00 01 10 11
        int tmp = inData & 3;
        Serial.print("inData =  ");
        Serial.print(inData, BIN );
        Serial.print("   tmp =  ");
        Serial.print(tmp );

        if(tmp == 0){
        for (int sl = 0; sl < 16; sl++) {
            digitalWrite(4,1);
            digitalWrite(5,1);                          
            delay(delay0);
            digitalWrite(4,0); 
            digitalWrite(5,0); 
            delay(delay0);
        }
        }
        else if(tmp == 1){
          for (int sl = 0; sl < 8; sl++) {
            digitalWrite(4,1); 
            digitalWrite(5,1);                                      
            delay(delay1);
            digitalWrite(4,0); 
            digitalWrite(5,0); 
            delay(delay1);
        }
        }
        else if(tmp == 2){
          for (int sl = 0; sl < 4; sl++) {
            digitalWrite(4,1); 
            digitalWrite(5,1);                                      
            delay(delay2);
            digitalWrite(4,0); 
            digitalWrite(5,0); 
            delay(delay2);
        }
        }
        else if(tmp == 3){
          for (int sl = 0; sl < 2; sl++) {
            digitalWrite(4,1);
            digitalWrite(5,1);                                       
            delay(delay3);
            digitalWrite(4,0); 
            digitalWrite(5,0); 
            delay(delay3);
        }
        }

        inData >>= 2;
        Serial.println();
      }
      state = !state;
    }
  }
}

double mapX(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
