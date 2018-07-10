#include <Wire.h>
#include <S11059.h>
#include <MsTimer2.h>

//音関係
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define RX_PIN 10
#define TX_PIN 11
SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
int numfile;   //楽曲ファイル数

S11059 colorSensor;

const int kubimotorA = 7; // 首モーター
const int kubimotorB = 8; // 首モーター
const int PWM_mot = 9;

void setup() {
  Serial.begin(9600);
  pinMode( kubimotorA, OUTPUT); //信号用ピン
  pinMode( kubimotorB, OUTPUT); //信号用ピン

  MsTimer2::set(500,flash);
  MsTimer2::start();

   mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true);
  }
  myDFPlayer.volume(3);  // 音量の設定(0～30)
  numfile = myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD); //音楽ファイル数取得
  
  Wire.begin();

  // 積分モードを固定時間モードに設定
  colorSensor.setMode(S11059_MODE_FIXED);

  // ゲイン選択
  // * S11059_GAIN_HIGH: Highゲイン
  // * S11059_GAIN_LOW: Lowゲイン
  colorSensor.setGain(S11059_GAIN_HIGH);

  // 1色あたりの積分時間設定(下記は指定可能な定数ごとの固定時間モードの場合の積分時間)
  // * S11059_TINT0: 87.5 us
  // * S11059_TINT1: 1.4 ms
  // * S11059_TINT2: 22.4 ms
  // * S11059_TINT3: 179.2 ms
  colorSensor.setTint(S11059_TINT0);

  // ADCリセット、スリープ解除
  if (!colorSensor.reset()) {
    Serial.println("reset failed");
  }

  // ADCリセット解除、バスリリース
  if (!colorSensor.start()) {
    Serial.println("start failed");
  }
}

void loop() {
  // 積分時間よりも長く待機
  //
  // 固定時間モード時のS11059.delay()で実行される待機処理の時間は
  // S11059.setTint(tint)で設定した値に応じて異なります
  colorSensor.delay();

  // センサのデータ用レジスタ(赤、緑、青、赤外)をまとめてリード
  if (colorSensor.update()) {
    Serial.println(colorSensor.getRed());
    if(colorSensor.getRed()>100){
      myDFPlayer.play(random(numfile)+1); //ランダム再生
      delay(7000);
    } 
  }else {
    Serial.println("update failed");
  }
  delay(500);
}

void flash(){
      if(colorSensor.getRed()>100){
      digitalWrite( kubimotorA, HIGH);
      digitalWrite( kubimotorB, LOW);
      analogWrite( PWM_mot, 255);
      }else{
      digitalWrite( kubimotorA, LOW);
      digitalWrite( kubimotorB, LOW);
      analogWrite( PWM_mot, 0);
      }
}

