


#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define L1 18   //Left Motor MB2
#define L2 19   //Left Motor MB1
#define R1 22  //Right Motor MA1
#define R2 23  //Right Motor MA2

BluetoothSerial SerialBT;

const int freq = 1000;
const int ledChannel_L1 = 0;
const int ledChannel_L2 = 1;
const int ledChannel_R1 = 2;
const int ledChannel_R2 = 3;

const int resolution = 8;

int speed;
char data;
//*************************************************//

float cc = 0.5;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test2");  //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  ledcSetup(ledChannel_L1 , freq, resolution);
  ledcSetup(ledChannel_L2 , freq, resolution);
  ledcSetup(ledChannel_R1 , freq, resolution);
  ledcSetup(ledChannel_R2 , freq, resolution);

  ledcAttachPin(L1 , ledChannel_L1);
  ledcAttachPin(L2 , ledChannel_L2);
  ledcAttachPin(R1 , ledChannel_R1);
  ledcAttachPin(R2 , ledChannel_R2);
}

void setSpeed(int sp) {
  speed = sp;
}

void sensorRead() {
  //Serial.print("h");
  if (SerialBT.available()) {

    data = SerialBT.read();

    Serial.println(data);
    // Serial.write(Serial.read());
  }
}

void goForward() {
  ledcWrite(ledChannel_R1, speed);
  ledcWrite(ledChannel_R2, LOW);
  ledcWrite(ledChannel_L1, LOW);
  ledcWrite(ledChannel_L2, speed);
}
void goForwardLeft() {
  ledcWrite(ledChannel_R1, speed);
  ledcWrite(ledChannel_R2, LOW);
  ledcWrite(ledChannel_L1, LOW);
  float x = speed * cc;
  int xx = int(x);
  ledcWrite(ledChannel_L2, speed - xx);
}


void goLeft() {
  ledcWrite(ledChannel_R1, speed);
  ledcWrite(ledChannel_R2, LOW);
  ledcWrite(ledChannel_L1, LOW);
  ledcWrite(ledChannel_L2, LOW);
}
void goBackwardLeft() {
  ledcWrite(ledChannel_R1, LOW);
  ledcWrite(ledChannel_R2, speed);
  float x = speed * cc;
  int xx = int(x);
  ledcWrite(ledChannel_L1, speed - xx);
  ledcWrite(ledChannel_L2, LOW);
}

void goBackwardRight() {
  ledcWrite(ledChannel_R1, LOW);
  float x = speed * cc;
  int xx = int(x);
  ledcWrite(ledChannel_R2, speed - xx);
  ledcWrite(ledChannel_L1, speed);
  ledcWrite(ledChannel_L2, LOW);
}

void goRight() {
  ledcWrite(ledChannel_R1, LOW);
  ledcWrite(ledChannel_R2, LOW);
  ledcWrite(ledChannel_L1, LOW);
  ledcWrite(ledChannel_L2, speed);
}

void goForwardRight() {
  float x = speed * cc;
  int xx = int(x);
  ledcWrite(ledChannel_R1, speed - xx);
  ledcWrite(ledChannel_R2, LOW);
  ledcWrite(ledChannel_L1, LOW);
  ledcWrite(ledChannel_L2, speed);
}

void goBackward() {
  ledcWrite(ledChannel_R1, LOW);
  ledcWrite(ledChannel_R2, speed);
  ledcWrite(ledChannel_L1, speed);
  ledcWrite(ledChannel_L2, LOW);
}

void goStop() {
  ledcWrite(ledChannel_R1, LOW);
  ledcWrite(ledChannel_R2, LOW);
  ledcWrite(ledChannel_L1, LOW);
  ledcWrite(ledChannel_L2, LOW);
}

void loop() {
  sensorRead();
  if ('1' <= data && data <= '9') {
    int sp = data - '0';
    speed = map(sp, 1, 9, 80, 250);
  } else if (data == 'q') speed = 255;

  setSpeed(speed);

  if (data == 'F') {
    goForward();
  } else if (data == 'G') {
    goForwardLeft();
  } else if (data == 'L') {
    goLeft();
  } else if (data == 'H') {
    goBackwardLeft();
  } else if (data == 'B') {
    goBackward();
  } else if (data == 'J') {
    goBackwardRight();
  } else if (data == 'R') {
    goRight();
  } else if (data == 'I') {
    goForwardRight();
  } else if (data == 'S') {
    goStop();
  }
}