
#include "lcd_wrapper.h"
#include "mastermind.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

void setup()
{
  lcd_init();
  Serial.begin(115200);
  pinMode(BTN_1_PIN, INPUT);
  pinMode(BTN_2_PIN, INPUT);
  pinMode(BTN_3_PIN, INPUT);
  pinMode(BTN_4_PIN, INPUT);
  pinMode(BTN_ENTER_PIN, INPUT);

  for(int i = 6; i < 14; i++){
    pinMode(i, OUTPUT);
  }
}


void loop(){
  char* secret = generate_code(true,4);
  play_game(secret);
  free(secret);
}