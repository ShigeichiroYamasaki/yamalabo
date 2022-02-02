# Rails Google OAuth2

## rails テンプレート

```
rails new google_login --skip-test --skip-action-mailer --skip-action-text --skip-action-cable

cd google_login
```

### Gemfile

追加

nano Gemfile

```
gem 'omniauth-google-oauth2'
gem 'omniauth-rails_csrf_protection'
```

```
bundle install
```

## Google APIの設定

[https://console.cloud.google.com/apis/dashboard)](https://console.cloud.google.com/apis/dashboard)


### API projectを選択

### プロジェクト名をつけて作成

![](img/googleAPI01.png)

### 「APIとサービス」>「OAuth同意画面」へ

 User Type 外部
 
 作成
 
### アプリ名とメールアドレスを設定

### スコープ

保存して次

### テストユーザ

自分のアドレスを ADD USER

保存して次


### 認証情報　

＋認証情報を作成

#### APIキーを作成（OAuth 2.0 クライアントID

アプリケーションの種類「ウェブアプリケーション」

名前をつける


#### 承認済みのリダイレクト URI

http://localhost:3000/auth/google_oauth2/callback

#### クライアントID、クライアントシークレット

クライアントID
361941436732-9rv1r1d8kmfe0mc5bkj62278jmosuakl.apps.googleusercontent.com

クライアントシークレット
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


#### API とサービス 「＋APIサービスの有効化」

Google+ API を選択　→　「有効にする」

![](img/googleAPI02.png)


## Rails 側のアプリケーションの作成

### 設定ファイルを作成

nano config/initializers/omniauth.rb


```ruby
Rails.application.config.middleware.use OmniAuth::Builder do
  provider :google_oauth2,
           Rails.application.credentials.google[:client_id],
           Rails.application.credentials.google[:client_secret]
end
```

### シークレット情報を編集

```
EDITOR=nano bin/rails credentials:edit -e development
```

以下の場合（各自、自分のクライアントIDとクライアントシークレットに書き換える）

クライアントID
361941436732-9rv1r1d8kmfe0mc5bkj62278jmosuakl.apps.googleusercontent.com

クライアントシークレット
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


```
# aws:
#   access_key_id: 123
#   secret_access_key: 345

google:
  client_id: 361941436732-9rv1r1d8kmfe0mc5bkj62278jmosuakl.apps.googleusercontent.com
  client_secret: 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
```

## Userモデル作成

```
rails g model user name email image
rails db:migrateあ
```

## コントローラの作成

```
rails g controller home index
rails g controller sessions
```

## ルーティング作成

nano config/routes.rb

```ruby
Rails.application.routes.draw do
  get 'home/index'

  get 'auth/:provider/callback', to: 'sessions#create'
  get 'auth/failure', to: redirect('/')
  get 'log_out', to: 'sessions#destroy', as: 'log_out'

  resources :sessions, only: %i[create destroy]
end

```

get 'auth/failure', to: redirect('/')の部分に注意

## ApplicationControllerにログイン関連メソッドを追加

nano app/helpers/sessions_helper.rb


```ruby
module SessionsHelper
  def current_user
    return unless (user_id = session[:user_id])

    @current_user ||= User.find_by(id: user_id)
  end

  def log_in(user)
    session[:user_id] = user.id
  end

  def log_out
    session.delete(:user_id)
    @current_user = nil
  end
end
```

nano app/controllers/application_controller.rb

```ruby
class ApplicationController < ActionController::Base
  include SessionsHelper
  before_action :check_logged_in

  def check_logged_in
    return if current_user

    redirect_to root_path
  end
end
```

nano app/controllers/home_controller.rb

```ruby
class HomeController < ApplicationController
  skip_before_action :check_logged_in, only: :index
  def index
  end
end
```

nano app/controllers/sessions_controller.rb

```ruby
class SessionsController < ApplicationController
  skip_before_action :check_logged_in, only: :create
end
```


### SessionsControllerにログイン・ログアウト処理を追加

nano app/controllers/sessions_controller.rb

```ruby
class SessionsController < ApplicationController
  skip_before_action :check_logged_in, only: :create
  
  def create
    if (user = User.find_or_create_from_auth_hash(auth_hash))
      log_in user
    end
    redirect_to root_path
  end
   
  def destroy
    log_out
    redirect_to root_path
  end

  private

  def auth_hash
    request.env['omniauth.auth']
  end
end
```

### Userモデルにfind_or_create_from_auth_hashメソッド追加

nano app/models/user.rb

```ruby
class User < ApplicationRecord
  class << self
    def find_or_create_from_auth_hash(auth_hash)
      user_params = user_params_from_auth_hash(auth_hash)
      find_or_create_by(email: user_params[:email]) do |user|
        user.update(user_params)
      end
    end
    
    private

    def user_params_from_auth_hash(auth_hash)
      {
        name: auth_hash.info.name,
        email: auth_hash.info.email,
        image: auth_hash.info.image,
      }
    end
  end
end
```

### Viewを作成

nano app/views/layouts/application.html.erb

```ruby
<!DOCTYPE html>
<html>
  <head>
    <title>GoogleLogin</title>
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <%= csrf_meta_tags %>
    <%= csp_meta_tag %>

    <%= stylesheet_link_tag "application", "data-turbo-track": "reload" %>
    <%= javascript_importmap_tags %>
  </head>

  <body>
    <header>
      <% if current_user %>
        <%= image_tag current_user.image %>
        <%= current_user.name %>さん
        <%= link_to "ログアウト", log_out_path %>
      <% else  %>
        ゲストさん
        <%= link_to "Googleでログイン", "/auth/google_oauth2", method: :post %>
      <% end %>
    </header>
    <%= yield %>
  </body>
</html>
```

## サーバ起動

```
rails s -b 0.0.0.0
```

