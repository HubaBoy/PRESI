#include <MD_MAX72xx.h>
#include <SPI.h>

#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  
#define DATA_PIN  11  
#define CS_PIN    10  

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
#define CHAR_SPACING  1 

#define BUF_SIZE  75
char message[BUF_SIZE] = "";
bool newMessageAvailable = true;
int IRPin = 2;
int butPin = 4;
float minTime = 2;
float time;
int score;
void readSerial(void)
{
  static uint8_t	putIndex = 0;

  while (Serial.available())
  {
    message[putIndex] = (char)Serial.read();
    if ((message[putIndex] == '\n') || (putIndex >= BUF_SIZE-3))  
    {
      message[putIndex] = '\0';
      putIndex = 0;
      newMessageAvailable = true;
    }
    else
     message[putIndex++];
  }
}

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
  pinMode(IRPin, INPUT);
  pinMode(butPin, INPUT);
  Serial.begin(9600);
  Serial.print("\n[MD_MAX72XX Message Display]\nType a message for the display\nEnd message line with a newline");
}

void loop() {
  delay(2000); 

  if (digitalRead(IRPin)) {
    Serial.println("Punch the reflex back");
  } else {
111112113114115116117118119120121122123124125126127128129130131132133134135
#include <MD_MAX72xx.h>
#include <SPI.h>

#define PRINT(s, v) { Serial.print(F(s)); Serial.print(v); }


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13  


    time = 0;
    Serial.println("The reflex back is punched");

    while (digitalRead(butPin)) {
      delay(1);
      time += 0.001;
    }

    score = (minTime / time) * 1000;
    sprintf(message, "%d", score);
    strcat(message, "ko");
    Serial.println("Your Score");
    Serial.print(message);

   
    printText(0, MAX_DEVICES-1, message);
  }
}
