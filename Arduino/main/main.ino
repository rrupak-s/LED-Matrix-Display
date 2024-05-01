#include "led_driver.h"
#include <TimerOne.h>

led_driver Snake_(6, 5, 7, 9, 10, 8);
#define JOY_X_PIN A5
#define JOY_Y_PIN A4
#define JOY_SW_PIN 2
enum direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN,
    STOP = 0
};
direction dir;

class snake
{
private:
    uint8_t headX, headY;
    uint32_t tailX[100], tailY[100];
    uint8_t fruitX, fruitY;

public:
    uint8_t score, nTail;
    uint8_t height, width;
    bool gameover;

    void initial()
    {
        gameover = false;
        dir = STOP;
        headX = 10;
        headY = 10;
        fruitX = 5;
        fruitY = 5;
        
    }
    

    void move()
    {
      int prevX = tailX[0];

        int prevY = tailY[0];

        int prev2X, prev2Y;

        tailX[0] = headX;

        tailY[0] = headY;

        for (int i = 1; i < nTail; i++)
       {

            prev2X = tailX[i];

            prev2Y = tailY[i];

            tailX[i] = prevX;

            tailY[i] = prevY;

            prevX = prev2X;

           prevY = prev2Y;
     }

        switch (dir)
        {
        case LEFT:
            headX--;
            break;

        case RIGHT:
            headX++;
            break;

        case UP:
            headY--;
            break;

        case DOWN:
            headY++;
            break;

        default:
            break;
        }

        if (headX > width)
            headX = 0;
        else if (headX <= 0)
            headX = width ;

        if (headY >height)
            headY = 0;
        else if (headY <= 0)
            headY = height;

        for (int i = 0; i < nTail; i++)

            if (tailX[i] == headX && tailY[i] == headY)
                gameover = true;

        if (headX == fruitX && headY == fruitY)
        {

            score += 10;

            fruitX = rand() % width;

            fruitY = rand() % height;

            nTail++;
        }
    }

   void Draw()
    {
       
       Snake_.make_coordinate(headX, headY);
       Snake_.make_coordinate(fruitX, fruitY);
       for(int i=0;i<nTail;i++){
        for(int j=0;j<nTail;j++){
        Snake_.make_coordinate(tailX[i],tailY[j]);
       }
       }

    }
};
snake Snake;

  



void setup(){

  pinMode(JOY_SW_PIN, INPUT);
  digitalWrite(JOY_SW_PIN, OUTPUT);
  pinMode(JOY_X_PIN, INPUT);
  pinMode(JOY_Y_PIN, INPUT);
  Timer1.initialize(5000); 
  Timer1.attachInterrupt(timerOneISR);
  Snake.initial();
}

void timerOneISR()
{
  
if (analogRead(JOY_Y_PIN) < 100)
{
  dir = RIGHT;
}
else if (analogRead(JOY_Y_PIN) > 1000)
{
  dir = LEFT;
}
else if (analogRead(JOY_X_PIN) < 100)
{
  dir = UP;
}
else if (analogRead(JOY_X_PIN) > 1000)
{
  dir = DOWN;
}
}
void loop()
{
    Snake.width = 21;
    Snake.height = 21;
    Snake.Draw();
    Snake.move();
    delay(50);
           
}
