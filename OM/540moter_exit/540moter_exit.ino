const int IN1 = 3;  // IN1ピンをD3に
const int IN2 = 4;  // IN2ピンをD4に
const int LR = 5;   // 右ホールセンサー
const int LL = 6;   // 左ホールセンサー

int e = 0;  // event
int s = 0;  // state

// event
// 0: stop
// 1: 右回り
// 2: 左回り
// 31: Rホールセンサー 磁気検知
// 32: Lホールセンサー 磁気検知
// 4: イベントなし
// 5: 名前問い合わせ
// 6: 状態問い合わせ

// state
// 0: モータ停止中
// 31: 右リミット
// 32: 左リミット

// action
// stop(): モーター停止
// rotateR(): モーター右回転
// rotateL(): モーター左回転

// モーター停止
void stop() {
  //  Serial.println('S');
  digitalWrite(IN1, LOW);  // モーターの停止
  digitalWrite(IN2, LOW);
}

// モーター右回転
void rotateR() {
  if (s == 32) {              // 右リミット
  } else if (s == 31) {       // 左リミット
    digitalWrite(IN1, HIGH);  // HIGH LOW の組み合わせでモーター回転
    digitalWrite(IN2, LOW);   // 右回転
    delay(10000);
  } else {
    digitalWrite(IN1, HIGH);  // HIGH LOW の組み合わせでモーター回転
    digitalWrite(IN2, LOW);   // 右回転
  }
}

// モーター左回転
void rotateL() {
  if (s == 31) {              // 左リミット
  } else if (s == 32) {       // 左リミット
    digitalWrite(IN1, LOW);   // HIGH LOW の組み合わせでモーター回転
    digitalWrite(IN2, HIGH);  // 右回転
    delay(10000);
  } else {
    digitalWrite(IN1, LOW);   // HIGH LOW の組み合わせでモーター回転
    digitalWrite(IN2, HIGH);  // 左回転
  }
}

// イベント検知 -> イベント発生
int event() {
  // ホールセンサーによるイベントよりシリアルコマンドが優先される
  if (Serial.available()) {              // wait for data available
    String input = Serial.readString();  // read until timeout
    input.trim();
    return input.substring(0).toInt();  // イベントの発出 ： 0, 1, 2, 5
  } else if (digitalRead(LR) == 0) {    // 磁気を検知
    return 31;
  } else if (digitalRead(LL) == 0) {  // 磁気を検知
    return 32;
  } else {
    return 4;
  }
}
//==================================
// セットアップ
void setup() {
  pinMode(IN1, OUTPUT);  // デジタルピンを出力に設定
  pinMode(IN2, OUTPUT);
  pinMode(LR, INPUT);
  pinMode(LL, INPUT);
  Serial.begin(9600);  // シリアル通信を9600bpsで開始
}

void loop() {
  // イベントを検知
  e = event();
  //Serial.println(e);
  switch (e) {
    case 0:  // 停止
      stop();
      s = 0;
      break;
    case 1:  // 右回り
      rotateR();
      break;
      break;
    case 2:
      rotateL();
      break;
    case 31:  // R ホールセンサーが磁気を感知した状態
      stop();
      s = 31;
      break;
    case 32:  // L ホールセンサーが磁気を感知した状態
      stop();
      s = 32;
      break;
    case 4:  // no new event
      break;
    case 5:                 // 名前のと言わせ
      Serial.println('3');  // 排出 exit と応える
      break;
    case 6:                 // 名前のと言わせ
      Serial.println(s);  // 
      break;
  }
  delay(500);
}
