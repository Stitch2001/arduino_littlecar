#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>

//定义PS2手柄管脚
#define PS2_DAT        A0   
#define PS2_CMD        A1
#define PS2_SEL        A2
#define PS2_CLK        A3

//定义PWM占空比（暂时用不上）
#define SPEED 255

//定义电机PWM管脚
int motor1 = 3; //左前轮
int motor2 = 5; //右前轮
int motor3 = 6; //右后轮
int motor4 = 9; //左后轮
int motor5 = 11;//前夹爪
int motor6 = 10;//升降架(无PWM功能)

//定义电机方向管脚
int dir1 = 2;
int dir2 = 4;
int dir3 = 7;
int dir4 = 8;
int dir5 = 12;
int dir6 = 13;

//定义舵机
int SERVO = A4;

//定义电机正方向
int good1 = HIGH;
int good2 = LOW;
int good3 = HIGH;
int good4 = LOW;
int good5 = LOW;
int good6 = HIGH;

#define pressures   true
#define rumble      true

PS2X ps2x;//PS2手柄
Servo servo;//舵机

int error = 0;
byte type = 0;
byte vibrate = 0;
int pos = 0;

// Reset func 
void (* resetFunc) (void) = 0;

//init func
void my_init(){
  Serial.begin(9600);
  delay(500);  //added delay to give wireless ps2 module some time to startup, before configuring it
  servo.attach(SERVO);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      Serial.println("GuitarHero Controller found ");
      break;
  case 3:
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
  }
}

void setup(){
  my_init();
  pinMode(motor1,OUTPUT);
  pinMode(motor2,OUTPUT);
  pinMode(motor3,OUTPUT); 
  pinMode(motor4,OUTPUT);
  pinMode(motor5,OUTPUT);
  pinMode(motor6,OUTPUT);
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(dir3,OUTPUT);
  pinMode(dir4,OUTPUT);
  pinMode(dir5,OUTPUT);
  pinMode(dir6,OUTPUT);
}


void Stop(){
 digitalWrite(motor1,LOW);
 digitalWrite(motor2,LOW);  
 digitalWrite(motor3,LOW);
 digitalWrite(motor4,LOW);  
}//停止函数

void Run(){
  digitalWrite(motor1,HIGH); 
  digitalWrite(motor2,HIGH); 
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,HIGH); 
  digitalWrite(dir1,good1); 
  digitalWrite(dir2,good2); 
  digitalWrite(dir3,good3); 
  digitalWrite(dir4,good4); 
}//前进函数

void Back(){
  digitalWrite(motor1,HIGH); 
  digitalWrite(motor2,HIGH); 
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,HIGH); 
  digitalWrite(dir1,!good1); 
  digitalWrite(dir2,!good2); 
  digitalWrite(dir3,!good3); 
  digitalWrite(dir4,!good4); 
}//后退函数

void Left(){
  digitalWrite(motor1,HIGH); 
  digitalWrite(motor2,HIGH); 
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,HIGH); 
  digitalWrite(dir1,good1); 
  digitalWrite(dir2,!good2); 
  digitalWrite(dir3,good3); 
  digitalWrite(dir4,!good4); 
}//左转函数

void Right(){
  digitalWrite(motor1,HIGH); 
  digitalWrite(motor2,HIGH); 
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,HIGH); 
  digitalWrite(dir1,!good1); 
  digitalWrite(dir2,good2); 
  digitalWrite(dir3,!good3); 
  digitalWrite(dir4,good4); 
}//右转函数

void Rotate_left(){
  digitalWrite(motor1,HIGH); 
  digitalWrite(motor2,HIGH); 
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,HIGH); 
  digitalWrite(dir1,!good1); 
  digitalWrite(dir2,good2); 
  digitalWrite(dir3,good3); 
  digitalWrite(dir4,!good4); 
}//向左自转函数

void Rotate_right(){
  digitalWrite(motor1,HIGH); 
  digitalWrite(motor2,HIGH); 
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,HIGH); 
  digitalWrite(dir1,good1); 
  digitalWrite(dir2,!good2); 
  digitalWrite(dir3,!good3); 
  digitalWrite(dir4,good4); 
}//向右自转函数

void Motor5_up(){
  digitalWrite(motor5,HIGH); 
  digitalWrite(dir5,good5); 
}//夹爪升起

void Motor5_stop(){
  digitalWrite(motor5,LOW); 
}//夹爪停下

void Motor5_down(){
  digitalWrite(motor5,HIGH); 
  digitalWrite(dir5,!good5);
}//夹爪落下

void Motor6_up(){
  digitalWrite(motor6,HIGH); 
  digitalWrite(dir6,good6); 
}//升降架升起

void Motor6_stop(){
  digitalWrite(motor6,LOW); 
}//升降架停下

void Motor6_down(){
  digitalWrite(motor6,HIGH); 
  digitalWrite(dir6,!good6);
}//升降架落下

void Stop_all(){
  Stop();
  Motor5_stop();
  Motor6_stop();
}

void loop() {
  if(error == 1){ //skip loop if no controller found
    resetFunc();
  }
  if(type == 1){ //DualShock Controller
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    if(ps2x.Button(PSB_START)){//按下START，舵机打开
      servo.write(-150);
    }
    if(ps2x.Button(PSB_SELECT)){//按下SELECT，舵机关闭
      servo.write(150);
    }
    if(ps2x.Button(PSB_PAD_UP)){//按下前，往前走
      Run();
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){//按下右，往右走
      Right();
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      Left();
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      Back();
    }   
    if (ps2x.Button(PSB_SQUARE)){//按下□，向左自转
      Serial.println("Pressed PSB_SQUARE");
      Rotate_left();
    }
    if (ps2x.Button(PSB_CIRCLE)){//按下○，向右自转
      Serial.println("Pressed PSB_CIRCLE");
      Rotate_right();
    }
    if (ps2x.Button(PSB_TRIANGLE)){//按下△，夹爪升起
      Motor5_up();
    }
    if (ps2x.Button(PSB_CROSS)){//按下×，夹爪落下
      Motor5_down();
    }
    if(ps2x.Button(PSB_L2)){
      Motor6_up();
    }
    if(ps2x.Button(PSB_R2)){
      Motor6_down();
    }

    
    if (ps2x.ButtonReleased(PSB_CROSS)){
      Motor5_stop();
    }
    if (ps2x.ButtonReleased(PSB_TRIANGLE)){
      Motor5_stop();
    }
    if(ps2x.ButtonReleased(PSB_L2)){
      Motor6_stop();
    }
    if(ps2x.ButtonReleased(PSB_R2)){
      Motor6_stop();
    }
    if (ps2x.ButtonReleased(PSB_SQUARE)){
      Serial.println("Released PSB_SQUARE");
      Stop();
    }
    if (ps2x.ButtonReleased(PSB_CIRCLE)){
      Serial.println("Released PSB_CIRCLE");
      Stop();
    }
    if (ps2x.ButtonReleased(PSB_PAD_UP)){
      Stop();
    }
    if (ps2x.ButtonReleased(PSB_PAD_DOWN)){
      Stop();
    }
    if (ps2x.ButtonReleased(PSB_PAD_LEFT)){
      Stop();
    }
    if (ps2x.ButtonReleased(PSB_PAD_RIGHT)){
      Stop();
    }
  }
  delay(50);
}
