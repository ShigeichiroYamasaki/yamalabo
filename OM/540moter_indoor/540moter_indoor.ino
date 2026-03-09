const int IN1 = 3;  // IN1ピンをD3に
const int IN2 = 4;  // IN2ピンをD4に
const int LR = 5;   // 右リミッター
const int LL = 6;   // 左リミッター

int e = 0;  // event
int s = 0;  // state

// event
// 0: stop
// 1: 右回り
// 2: 左回り
// 31: Rリミッターオン
// 32: Lリミッターオン
// 4: イベントなし
// 5: name

// state
// 0: モータ停止中
// 1: モーター右回り
// 2: モーター左回り

// action
// stop(): モーター停止
// rotateR(): モーター右回転
// rotateL(): モーター左回転



// モーター停止
void stop() {
  Serial.println('S');
  digitalWrite(IN1, LOW);  // モーターの停止
  digitalWrite(IN2, LOW);
}

// モーター右回転
void rotateR() {
  Serial.println('R');
  digitalWrite(IN1, HIGH);  // HIGH LOW の組み合わせでモーター回転
  digitalWrite(IN2, LOW);   // 右回転
  delay(500);
}

// モーター左回転
void rotateL() {
  Serial.println('L');
  digitalWrite(IN1, LOW);   // HIGH LOW の組み合わせでモーター回転
  digitalWrite(IN2, HIGH);  // 左回転
  delay(500);
}

// イベント
int event() {

  // Serial.println(digitalRead(LR));
  // Serial.println(digitalRead(LL));

  // limitterイベントよりシリアルコマンドが優先される
  if (Serial.available()) {              // wait for data available
    String input = Serial.readString();  // read until timeout
    input.trim();
    return input.substring(0).toInt();  // 0, 1, 2, 5
                                        // limitter　イベント
  } else if (digitalRead(LR) == 0) {
    return 31;
  } else if (digitalRead(LL) == 0) {
    return 32;
  } else {
    return 4;
  }
}
//==================================
void setup() {
  pinMode(IN1, OUTPUT);  // デジタルピンを出力に設定
  pinMode(IN2, OUTPUT);
  pinMode(LR, INPUT);
  pinMode(LL, INPUT);
  Serial.begin(9600);  // シリアル通信を9600bpsで開始
}

void loop() {

  e = event();
  //Serial.println(e);
  switch (e) {
    case 0:
      stop();
      s = 0;
      break;
    case 1:
      if (s == 31) {
        rotateR();
        delay(2000);
      } else if (s ==32){
      } else {
        rotateR();
      }
     // s = 1;
      break;
    case 2:
      if (s == 32) {
        rotateL();
        delay(2000);
      } else if (s ==31) {
      } else {
        rotateL();
      }
      //s = 2;
      break;
    case 31:  // R limitterスイッチが押された状態
      stop();
      s = 31;
      break;
    case 32:  // L limitterスイッチが押された状態
      stop();
      s = 32;
      break;
    case 4:  // no new event
      break;
    case 5:                 // name ?
      Serial.println('2');  // floor:1 indoor:2 exit:3
      break;
  }
  delay(500);
}
