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
// 3: リミッターオン
// 4: イベントなし

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
  digitalWrite(IN1, LOW);  // モーターの停止
  digitalWrite(IN2, LOW);
}

// モーター右回転
void rotateR() {
  digitalWrite(IN1, HIGH);  // HIGH LOW の組み合わせでモーター回転
  digitalWrite(IN2, LOW);   // 右回転
  delay(500);
}

// モーター左回転
void rotateL() {
  digitalWrite(IN1, LOW);   // HIGH LOW の組み合わせでモーター回転
  digitalWrite(IN2, HIGH);  // 左回転
  delay(500);
}

// イベント
int event() {

  Serial.println(digitalRead(LR));
  Serial.println(digitalRead(LL));

  // limitterイベントよりシリアルコマンドが優先される
  if (Serial.available()) {              // wait for data available
    String input = Serial.readString();  // read until timeout
    input.trim();
    return input.substring(0).toInt();  // 0, 1, 2
                                        // limitter　イベント
  } else if (digitalRead(LR) == 0 || digitalRead(LL) == 0) {
    return 3;
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
      if (s == 2) {
        rotateR();
        delay(2000);
      } else {
        rotateR();
      }
      s = 1;
      break;
    case 2:
      if (s == 1) {
        rotateL();
        delay(2000);
      } else {
        rotateL();
      }
      s = 2;
      break;
    case 3:  // limitterスイッチが押された状態
      stop();
      break;
    case 4:  // no new event
      break;
  }
  delay(500);
}
