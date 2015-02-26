/* Simple Maze Challenge
 
This sofware is provide to control a three wheeled robot with the 
front wheel connected to Dynamixle #3 to control the angle of movement
and the Dynamixle #1 and Dynamixle #2 are for moving forward and backword.
a limit switch is used and mounted in the front right and front left of the robot to give a signal
that the robot hit the wall and then a dicision is made to correct the path to avoid the wall.
The goal is to have the robot move in an arc shape pattern and follow a wall in a maze 
from start point to end, the shape of the path is not important .
the angle is controlled so sthe radius of the motion is smaller that the width of the 
corridor of the maze.

created 19 02 2015
by Mahmoud Al Dabbas
 */
 /* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
/* Dynamixel ID defines */
#define ID_NUM_1 1
#define ID_NUM_2 2
#define ID_NUM_3 3
#define GOAL_SPEED 32

#define CCW_Angle_Limit 8
#define CONTROL_MODE 11

Dynamixel Dxl(DXL_BUS_SERIAL1);

void setup() {
  // Dynamixel 2.0 Protocol -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);
  
    Dxl.wheelMode(ID_NUM_1); //wheelMode() is to use wheel mode
    Dxl.wheelMode(ID_NUM_2); //wheelMode() is to use wheel mode
    Dxl.jointMode(ID_NUM_3); //jointMode() is to use position mode
    pinMode(BOARD_LED_PIN, OUTPUT); //to indicate when the movement of the robot
    pinMode(D10, INPUT_PULLDOWN); //provide a signal from the limit switch.

 }


void loop() 
{
  int buttonState = digitalRead(D10); //read the limit switch
  Arc(); // move in an arc fashion.
  
  
      while (1) // do for ever.
      {
         int buttonState = digitalRead(D10); //read the limit switch
         if(buttonState==LOW) // didnt hit the wall
             {
             forward(); // keep moving
             }
             else
             {
             Back(); // the robot hit the wall, go back.
             forward(); // go forward in arc fashion.
             }

      }
}


void Arc()
{
  /*Turn dynamixel ID 3 to position tilted*/
  Dxl.goalPosition(ID_NUM_3, 590); 
  delay(500); 
}
void forward()
{
  //forward
  Dxl.writeWord(ID_NUM_1, GOAL_SPEED, 400| 0x400); 
  Dxl.writeWord(ID_NUM_2, GOAL_SPEED, 400); // opposit to the first dynamixle so to move in same direction
  digitalWrite(BOARD_LED_PIN, LOW); // set to as HIGH LED is turn-off to indicate movement

}

void Back() //back up a little bit so it doesnt trap it self
{
  //stop
  Dxl.writeWord(ID_NUM_1, GOAL_SPEED, 0); 
  Dxl.writeWord(ID_NUM_2, GOAL_SPEED, 0); 
  digitalWrite(BOARD_LED_PIN, HIGH); // set to as HIGH LED is turn-off to indicate not moving  
  delay(500);
  
  //backward
  Dxl.writeWord(ID_NUM_1, GOAL_SPEED, 200); 
  Dxl.writeWord(ID_NUM_2, GOAL_SPEED, 200| 0x400); 
  delay(250); 


  Dxl.goalPosition(ID_NUM_3, 230); // tilt 90 degrees away from wall.
  delay(500); 
  Dxl.writeWord(ID_NUM_1, GOAL_SPEED, 500|0x400); 
  Dxl.writeWord(ID_NUM_2, GOAL_SPEED, 500|0x400); 
  delay(250);
  //stop
  Dxl.writeWord(ID_NUM_1, GOAL_SPEED, 0); 
  Dxl.writeWord(ID_NUM_2, GOAL_SPEED, 0); 
  delay(500);
  
 /*Turn dynamixel ID 3 to position 90 deg right*/
  Dxl.goalPosition(ID_NUM_3, 512+22/.29);  /* controls the radius of the angle here, 512 corresponds 
  to straight, 22 is the angle and .29 to convert from degrees to 1024 scale corresponding to 300 degree total. */
  delay(500); 
 
}


