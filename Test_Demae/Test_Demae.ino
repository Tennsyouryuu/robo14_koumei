//出前ロボット四輪駆動制御プログラムてすと版

#include "recv_udp.h" 

#include <math.h>

#define MOTOR_CNT 4  

int          m_pin[] = {
                       2,3
                      ,4,5
                      ,6,7
                      ,8,9
             };
                       

//走行モードを読み込む
extern int tire, cata, updown;

//ゲームパッドコマンド受信用
int          p_gamepad[] = {tire, 0, 0, 0, 0};

//trueで前進、falseで後退
boolean      mode_forward[MOTOR_CNT+1] = {true};

//PWMの格納用
int          pwm_val[MOTOR_CNT+1] = {0};


void print_cmd(){
    //コマンド表示
  for(int i=1; i<=(MOTOR_CNT-1); i++){
    Serial.print(p_gamepad[i]);
    Serial.print(":");
  }
  Serial.println(p_gamepad[MOTOR_CNT]);
}


//PWMの値を生成
int make_pwm(int cmd){
  int out_pwm = 200 - (abs(cmd) * 20);
  return(out_pwm);
}

void check_rotation(){
  for(int i=1; i<=MOTOR_CNT; i++){
    if( p_gamepad[i] > 0){
      mode_forward[i] = true;
    }else if(p_gamepad[i] < 0){
      mode_forward[i] = false;
    }else{
      mode_forward[i] = true;
    }
  }
}

void drive_demae(){
  for(int i=1; i<=MOTOR_CNT; i++){
  
    if(mode_forward[i]){
      for(int i=1; i<=MOTOR_CNT; i++){
        analogWrite(m_pin[i], pwm_val[i]);
        analogWrite(m_pin[i+1], 254);
        Serial.print(pwm_val[i]);
        Serial.print(":");
        Serial.print("Forward!");
      }Serial.println("");
    }else if(!mode_forward[i]){
        for(int i=1; i<=MOTOR_CNT; i++){
          analogWrite(m_pin[i], 254);
          analogWrite(m_pin[i+1], pwm_val[i]);
          Serial.print(pwm_val[i]);
          Serial.print(":");
          Serial.print("Back!");
        }Serial.println("");
    }
  }
}

void setup()
{ 
  //UDP通信を呼び出す
  UDP_setup();
   
  Serial.begin(9600); 
}

void loop(){

  
  //コントローラからコマンドを取得し、PWM値に変換する
  get_command( p_gamepad );
  check_rotation();
    
  //コマンド表示
  //print_cmd();  
  
  //PWMの値を計算
  for(int i=1; i<=MOTOR_CNT; i++){
    pwm_val[i] = make_pwm(p_gamepad[i]);
  }  

  
  drive_demae();
 
  delay(100);
  
}

