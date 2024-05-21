module.exports={
  // テストのディレクトリを test にする
  spec: 'test/**/*.spec.js',
  // テストスイートのタイムアウトは5秒
  timeout: 5000,
  // 使用するテストインターフェース TDD/BDD
  ui: 'tdd',
  // レポーターの指定
  reporter: 'spec'
}

