#include <MD_MAX72xx.h>
#include <SPI.h>

#define PRINT(s, v) \
  {                 \
    Serial.print(F(s)); \
    Serial.print(v);    \
  }

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN 13 // or SCK
#define DATA_PIN 11 // or MOSI
#define CS_PIN 10   // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

#define CHAR_SPACING 1

#define BUF_SIZE 75
char message[BUF_SIZE] = "HIT IT";
int IRPin = 2;
int butPin = 3;
float minTime = 0.2;
float time = 0;
int score =0;
bool isHit = false;
float start;
float end;

void printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
{
  uint8_t state = 0;
  uint8_t curLen;
  uint16_t showLen;
  uint8_t cBuf[8];
  int16_t col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do
  {
    switch (state)
    {
    case 0:
      if (*pMsg == '\0')
      {
        showLen = col - (modEnd * COL_SIZE);
        state = 2;
        break;
      }

      showLen = mx.getChar(*pMsg++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state++;

    case 1:
      mx.setColumn(col--, cBuf[curLen++]);
      if (curLen == showLen)
      {
        showLen = CHAR_SPACING;
        state = 2;
      }
      break;

    case 2:
      curLen = 0;
      state++;

    case 3:
      mx.setColumn(col--, 0);
      curLen++;
      if (curLen == showLen)
        state = 0;
      break;

    default:
      col = -1;
    }
  } while (col >= (modStart * COL_SIZE));

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void setup()
{
  mx.begin();
  Serial.begin(9600);
  pinMode(IRPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRPin), senso1, CHANGE);
  pinMode(butPin, INPUT_PULLUP);
   attachInterrupt(digitalPinToInterrupt(butPin), senso2, LOW);
  printText(0, MAX_DEVICES - 1, message);
} 

void loop()
{
}

void senso1()
{
  if(digitalRead(IRPin))start = millis()/1000;
  if(digitalRead(!IRPin))
  {
   if (isHit)
      isHit = false;
  }
}

void senso2()
{
  if(!isHit && digitalRead(IRPin))
  {
    
    Serial.println("HIT");
     end = float(millis())/1000;
     time = end - start;
     score = (time > 0) ? (minTime / time) * 1000 : 0;
      if (score > 1000 || score < 0 || time<0|| score == 0) score = 999;
      sprintf(message, "%d", score);
      printText(0, MAX_DEVICES - 1, message);
      isHit = true;
  }
}