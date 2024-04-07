#include <MD_MAX72xx.h>
#include <SPI.h>

#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


#define CHAR_SPACING  1 

#define BUF_SIZE  75
char *message = "HIT IT";
int IRPin = 2;
int butPin = 4;
float minTime = 2;
float time = 0;
int score;
void printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
{
  uint8_t   state = 0;
  uint8_t   curLen;
  uint16_t  showLen;
  uint8_t   cBuf[8];
  int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

  mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

  do  
  {
    switch(state)
    {
      case 0:
      if (*pMsg == '\0')
        {
          showLen = col - (modEnd * COL_SIZE); 
          state = 2;
          break;
        }

        showLen = mx.getChar(*pMsg++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
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
  pinMode(IRPin, INPUT);
  pinMode(butPin, INPUT);
  printText(0, MAX_DEVICES-1, message);
}

void loop()
{
   delay(1000);
  if(digitalRead(IRPin)==0)
  {
    Serial.println("Punch the reflex back");
  }else{
     Serial.println("The reflex back is puncked");
     time = 0;
    while(digitalRead(butPin))
    {
      delay(1);
      time+=0.001;
    }
  score = (minTime/time)*1000;
  if (score> 1000) score = 999;
    sprintf(message, "%d", score);
    Serial.println(message);
    Serial.print("thats your score");
    int len = strlen(message);
   printText(0, MAX_DEVICES-1, message);
  }
}
