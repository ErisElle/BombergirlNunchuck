#include <Wire.h>
//#include <Mouse.h>
#include "nunchuck_funcs.h"

#include <XInput.h>
/*
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
 4, 0,                  // Button Count, Hat Switch Count
 true, true, false,     // X and Y, but no Z Axis
 false, false, false,   // No Rx, Ry, or Rz
 false, false,          // No rudder or throttle
 false, false, false);  // No accelerator, brake, or steering
 */

int KEY1_PIN = 4;
int KEY2_PIN = 5;
int KEY3_PIN = 6;
int KEY4_PIN = 7;

int INPUT_GATE_LOW = 123;
int INPUT_GATE_HIGH = 133;

byte p_accx, p_accy, p_accz, p_zbut, p_cbut, p_joyx, p_joyy;
byte accx, accy, accz, zbut, cbut, joyx, joyy;

int X_POS ;   // X軸方向の読み取り値の変数を整数型に
int Y_POS ;   // Y軸方向の読み取り値の変数を整数型に

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  nunchuck_init();
  XInput.begin();
  //XInput.setXAxisRange(0, 255);
  //XInput.setYAxisRange(255, 0);

  XInput.setRange(JOY_LEFT, 0, 255);
  XInput.setRange(JOY_RIGHT, 0, 1024);
  
  //Joystick.setXAxisRange(-1, 1);
  //Joystick.setYAxisRange(-1, 1);
  //XInput.setYAxis(0);
  //XInput.setXAxis(0);

  XInput.setButton(0, 0); // BOMB
  XInput.setButton(1, 0); // SKILL1
  XInput.setButton(2, 0); // SKILL2
  //Joystick.setButton(3, 0); // SKILL CHANGE

  // initialize buttons pin
  pinMode(KEY1_PIN, INPUT_PULLUP);
  pinMode(KEY2_PIN, INPUT_PULLUP);
  pinMode(KEY3_PIN, INPUT_PULLUP);
  //pinMode(7, INPUT_PULLUP);

  // stick initialize
  pinMode(A0, INPUT);          // UD
  pinMode(A1, INPUT);          // LR
}


void loop() {
  nunchuck_get_data();

  p_accx = accx;
  p_accy = accy;
  p_accz = accz;
  p_zbut = zbut;
  p_cbut = cbut;
  p_joyx = joyx;
  p_joyy = joyy;

  joyx = nunchuck_joyx();
  joyy = nunchuck_joyy();
  //accx  = nunchuck_accelx();
  //accy  = nunchuck_accely();
  zbut = nunchuck_zbutton();
  cbut = nunchuck_cbutton();

  XInput.setButton(BUTTON_X, !digitalRead(KEY1_PIN));
  XInput.setButton(BUTTON_A, !digitalRead(KEY2_PIN));
  XInput.setButton(BUTTON_B, !digitalRead(KEY3_PIN));

  /*
  if(!digitalRead(KEY4_PIN)){
    Joystick.setTrigger(TRIGGER_RIGHT, 255);
  } else {
    Joystick.setTrigger(TRIGGER_RIGHT, 0);
  }
  */

  XInput.setTrigger(TRIGGER_RIGHT, zbut * 256);
  

  // ニュートラルに近い値を丸める
  int xvalue = 127;
  int yvalue = 127;
  
  if(INPUT_GATE_HIGH < joyx || INPUT_GATE_LOW > joyx){
    xvalue = joyx;
  }

  if(INPUT_GATE_HIGH < joyy || INPUT_GATE_LOW > joyy){
    yvalue = joyy;
  } 

  XInput.setJoystick(JOY_LEFT, joyx, joyy);

  // right stick
  X_POS = analogRead(A1);     // X軸方向のアナログ値を読み取る
  Y_POS = analogRead(A0);     // Y軸方向のアナログ値を読み取る

  XInput.setJoystick(JOY_RIGHT, X_POS, Y_POS);
  
 delay(10);
}
