#include <IRremote.h>
#include <Servo.h>


#define U 0x3BE1205A //верх, влево
#define L 0xA62D1A55 //исходная позиция
#define D 0x3BCF9D24 //вниз, вправо
#define N1 0xB527D671 //переключение серво верх
#define N4 0x9B507FEB //переключение серво вниз
#define STOP 0xD7D42191 //команда стоп, подается после каждого сигнала

/*0-90
 * 180-135
 * 0-90-180
 * 75-90-105
 * 160-180
*/
int degree(int sNum, bool is);
void servoMove(int sNum, bool directionS);

const int servoKolvo = 5;
const int RECV_PIN = 2;
int servoNum = 0;
bool isStoped = false;

Servo servos[servoKolvo];
IRrecv irrecv(RECV_PIN);
decode_results results;




void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  servos[0].attach(3);
  servos[1].attach(4);
  servos[2].attach(5);
  servos[3].attach(6);
  servos[4].attach(7);
}



void loop(){
  if (irrecv.decode(&results)){
        switch(results.value){
          case 0x3BE1205A:
          isStoped = false;
         servoMove(servoNum, true);
         Serial.println("U");
          break;
          case 0x3BCF9D24:
          isStoped = false;
          servoMove(servoNum, false);
          Serial.println("D");
          break;
          case 0xB527D671:
          isStoped = false;
          servoNum +=1;
          if(servoNum > servoKolvo) {servoNum = 4;}
          Serial.println("N1");
          break;
          case 0x9B507FEB:
          isStoped = false;
          servoNum -=1;
          if(servoNum <= 0){servoNum = 0;}
          Serial.println("N4");
          break ;  
          case 0xD7D42191:
          isStoped = true;
          Serial.println("STOP");
          break ;   
        }
        irrecv.resume();
}
}



int degree(int sNum, bool is){
  if(sNum == 0 && is == true){
    return 90;
  }else if(sNum == 0 && is == false){
    return 0;
  }else if(sNum == 1 && is == true){
    return 180;
  }else if(sNum == 1 && is == false){
    return 135;
  }else if(sNum == 2 && is == true){
    return 180;
  }else if(sNum == 2 && is == false){
    return 0;
  }else if(sNum == 3 && is == true){
    return 105;
  }else if(sNum == 3 && is == false){
    return 75;
  }else if(sNum == 4 && is == true){
    return 180;
  }else if(sNum == 4 && is == false){
    return 160;
  }
}



void servoMove(int sNum, bool directionS){
  
  if(directionS && servos[sNum].read() < degree(sNum, true)){
    for(int i = servos[sNum].read();i<=degree(sNum, true);i++){
       servos[sNum].write(i);
       delay(5);
    }
  }
  else if(!directionS && servos[sNum].read() > degree(sNum, false)){
    for(int i = servos[sNum].read();i>=degree(sNum, false);i--){
       servos[sNum].write(i);
       delay(5);
    }
  }
}
