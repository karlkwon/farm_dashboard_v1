/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

//  board: Nano

#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

#define BTN_1  5
#define BTN_2  6
#define SW_1  7

int id = 1;

int btn_1_past = 0;
int alarm_en = 0;
float temp = 0;

//////////////////////////////////////////////////////////////////////////////////
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

//////////////////////////////////////////////////////////////////////////////////
SoftwareSerial HC12(10, 11);
#define HC12_SET  12

void hc12_setup() {
  HC12.begin(9600);

  pinMode(HC12_SET, OUTPUT);
  digitalWrite(HC12_SET, LOW);

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

  digitalWrite(HC12_SET, HIGH);

  sensors.begin();
}

char buffer[30];
void make_message(int id, int door_status, int alarm, int temp) {
  sprintf(buffer, "{\"ID\":%d,\"D\":%d,\"A\":%d,\"T\":%d}", id, door_status, alarm, temp);
  Serial.println(buffer);
  HC12.println(buffer);
}

//////////////////////////////////////////////////////////////////////////////////

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

// initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);

  btn_1_past = digitalRead(BTN_1);

  pinMode(SW_1, OUTPUT);
  digitalWrite(SW_1, HIGH);

  hc12_setup();
}

int cnt = 0;

// the loop function runs over and over again forever
void loop() {
  int btn_1 = digitalRead(BTN_1);
  int btn_2 = digitalRead(BTN_2);

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(50);                       // wait for a second

//  Serial.print("BTN_1 - ");
//  Serial.println(btn_1);
//  Serial.print("BTN_2 - ");
//  Serial.println(btn_2);

  if (btn_2 == 1) {
    alarm_en = 1;
  }

  if (btn_1 != btn_1_past) {
    alarm_en = 0;
  }

  cnt += 1;

  if(cnt >= 5) {
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0) * 10;
    Serial.println(temp);
    
    make_message(id, btn_2, alarm_en, temp);
    cnt = 0;
  }

  if (alarm_en == 1) {
    digitalWrite(SW_1, LOW);
  } else {
    digitalWrite(SW_1, HIGH);
  }

  btn_1_past = btn_1;
}
