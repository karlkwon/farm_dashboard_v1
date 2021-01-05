#if 1

#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11);
#define HC12_SET  12

#else

// LOLIN D32
HardwareSerial HC12(2);
#define HC12_SET  14

#endif


void hc12_setup() {
//  HC12.begin(9600);
  HC12.begin(2400);

  pinMode(HC12_SET, OUTPUT);
/*  digitalWrite(HC12_SET, LOW);

  delay(500);

  HC12.print("AT");
  delay(100);
  HC12.print("AT+B2400");
  delay(100);

  digitalWrite(HC12_SET, HIGH);
  delay(200);
  digitalWrite(HC12_SET, LOW);
  
  HC12.begin(2400);
  delay(500);
  
  HC12.print("AT+C001");
  delay(100);
  HC12.print("AT+FU1");
  delay(100);
  HC12.print("AT+P8");
  delay(100);
*/
  digitalWrite(HC12_SET, HIGH);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  hc12_setup();

//  Serial.print("Ready... \r\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(HC12.available()){
    Serial.write(HC12.read());
  }
  while(Serial.available()){
    HC12.write(Serial.read());
  }
}
