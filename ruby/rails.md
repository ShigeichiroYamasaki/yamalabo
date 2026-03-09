# rails8

2026/02/03

Shigeichiro Yamasaki

## Rubyのインストール

ここを参照してください　[Rubyのインストール](./ruby.md)

## nvmのインストール

ここを参照してください [JavaScript](../javascript/JavaScript.md)


### Railsインストール

```bash
gem update --system
gem update

gem install bundler
gem install sqlite3
gem install json-jwt
gem install jwt
gem install rails
```

## Railsアプリの例

家の場所ごとの温度データの時系列管理

* 室内温度:indoorC
* 床下温度:floorC
* 屋根温度:roofC

```
Temperature
  indoorC:float
  floorC:float
  roofC:float
```

## Railsプロジェクトの生成

```bash
rails new house --api
cd house
```


### scaffold で MVCを生成

```bash
rails g scaffold Temperature indoorC:float floorC:float roofC:float
```

### DB のマイグレーション

```bash
rails db:migrate
```

### ルーティングの確認

```bash
rails routes
=>
                                   Prefix Verb   URI Pattern                                                                                       Controller#Action
                            temperatures GET    /temperatures(.:format)                                                                           temperatures#index
                                         POST   /temperatures(.:format)                                                                           temperatures#create
                             temperature GET    /temperatures/:id(.:format)                                                                       temperatures#show
                                         PATCH  /temperatures/:id(.:format)                                                                       temperatures#update
                                         PUT    /temperatures/:id(.:format)                                                                       temperatures#update
                                         DELETE /temperatures/:id(.:format)                                                                       temperatures#destroy
                      rails_health_check GET    /up(.:format)                                                                                     rails/health#show
        
        ...
```

### サーバーの起動

```bash
rails s -b 0.0.0.0
```

### python からの http アクセス

* ライブラリのインストール

```bash
pip3 install requests       
```

* インタラクティブ操作

```bash
python3
```

```python
>>> import requests
>>> import json
```

* データの POST

```python
>>> data = {"indoorC" : 22.5, "floorC" : 22.4, "roofC" : 25.1}
>>> json_data = json.dumps(data)
>>> response = requests.post("http://192.168.0.44:3000/temperatures", data=json_data,  headers={"Content-Type": "application/json"})
```

* データの GET

```python
>>> response = requests.get("http://192.168.0.44:3000/temperatures",  headers={"Content-Type": "application/json"})

>>> response.text
'[{"id":1,"indoorC":22.5,"floorC":22.4,"roofC":25.1,"created_at":"2026-02-03T12:48:30.775Z","updated_at":"2026-02-03T12:48:30.775Z"},{"id":2,"indoorC":22.5,"floorC":22.5,"roofC":25.3,"created_at":"2026-02-03T12:48:30.821Z","updated_at":"2026-02-03T12:48:30.821Z"},{"id":3,"indoorC":22.0,"floorC":22.6,"roofC":25.4,"created_at":"2026-02-03T12:48:30.822Z","updated_at":"2026-02-03T12:48:30.822Z"},{"id":4,"indoorC":22.2,"floorC":22.7,"roofC":25.5,"created_at":"2026-02-03T12:48:30.825Z","updated_at":"2026-02-03T12:48:30.825Z"},{"id":5,"indoorC":22.5,"floorC":22.4,"roofC":25.1,"created_at":"2026-02-03T13:05:54.490Z","updated_at":"2026-02-03T13:05:54.490Z"}]'
```

* JavaScript でのデータの GET

