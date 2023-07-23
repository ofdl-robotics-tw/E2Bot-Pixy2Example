#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MBC_Robot.h>
#include <Pixy2I2C.h>

double angle_rad = PI / 180.0;
double angle_deg = 180.0 / PI;
void Init();
double notTouch;
MBC Robot;
Pixy2I2C pixy; //connect pixy2 by I2C

void Init()
{
  Robot.setBuzzer(349, 0.1);
  Robot.setLCDclear();
  Robot.setLCDshow("STBY", 60, 96, 2016);
  //Wait touch press
  notTouch = !(Robot.CompareTouchSensor());
  while (!(((notTouch) == (Robot.CompareTouchSensor()))))
  {
    _loop();
  }
  Robot.setBuzzer(523, 0.3);
}

void printBlock(int x, int y, int w, int h) {
  Robot.setLCDclear();
  x = map(x, 0, 316, 0, 160);
  y = map(y, 0, 208, 0, 128);
  w = map(w, 0, 316, 0, 160);
  h = map(h, 0, 208, 0, 128);
  for (int i = x; i <= x + w; i++) // outer loop
  {
    for (int j = y; j <= y + h; j++) // inner loop
    {
      Robot.setLCDpixel(i, j, 65535);
    }
  }
  _delay(0.001);
}

void setup() {
  Robot.init();
  pixy.init();
  Init();
}

void loop()
{
  _loop();
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) //only print Sig 1 now.
  {
    printBlock(pixy.ccc.blocks[0].m_x, pixy.ccc.blocks[0].m_y, pixy.ccc.blocks[0].m_width, pixy.ccc.blocks[0].m_height);
  }
}

void _delay(float seconds) {
  long endTime = millis() + seconds * 1000;
  while (millis() < endTime)_loop();
}

void _loop() {
  Robot.MpuUpdata();
}
