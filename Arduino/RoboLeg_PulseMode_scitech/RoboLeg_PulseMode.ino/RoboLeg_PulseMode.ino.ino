#include <TimerOne.h>
#define SIZE 8

// Declarations
int Sum(int* arr);

// variables
int Duration[SIZE] = {3,3,3,3,2,2,5,10};
int Amplitude[SIZE] = {10,0,10,0,50,-50,100,0};
int Type[SIZE] = {0,0,0,0,0,0,0,0};
int LoopRate = 0.001; // seconds
int TimeLimit = LoopRate*Sum(Duration);


void setup() {
  
  Serial.begin(115200);
  Timer1.initialize(LoopRate*1e6);
  Timer1.attachInterrupt(LoopAction);
}

void LoopAction()
{
  int input = 0;
  static short pos = 0;
  static short count_inside = -1; // SHUOLD BE ZERO ! STUPID
  static int totalCount = 0;

  // TODO:: Problem
  if(totalCount > Sum(Duration))
  {
    Timer1.detachInterrupt();
  }
  //Serial.print("Sdfsfsdf" + Sum(Duration));
  
  // TODO: it prints one more then what it should
  if(Type[pos] == 0) // Block
  {
      if(count_inside < Duration[pos]-1)  // SHUOLD BE ZERO ! STUPID
      {
        count_inside++;
      }
      else
      {
        count_inside = 0;
        pos++;
      }
      input = Amplitude[pos]; 
       //Serial.print(1, DEC);
  }
  else //  if(Type[pos] == 1)
  {
    input = -888;  
  }
  totalCount++;
  //Serial.print(2, DEC);
  Serial.println(input, DEC);

  
}

void loop() {
  // put your main code here, to run repeatedly:
//  int userInput = Serial.read();
//  if(userInput == '1')
//      Timer1.attachInterrupt(LoopAction);
//  else if(userInput == '0')
//      Timer1.detachInterrupt();
}

int Sum(int* arr)
{
  int total = 0;
  for(int i=0; i < SIZE; i++) // (sizeof(arr)/sizeof(*arr))  == SIZE  ?? CHECK THIS PROBELEM
   { 
    total += arr[i];
  } 
  return total;
}

