// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

interface IERC20 {
    function totalSupply() external view returns (uint256);
    function balanceOf(address account) external view returns (uint256);
    function transfer(address recipient, uint256 amount) external returns (bool);
    function allowance(address owner, address spender) external view returns (uint256);
    function approve(address spender, uint256 amount) external returns (bool);
    function transferFrom(address sender, address recipient, uint256 amount) external returns (bool);

    event Transfer(address indexed from, address indexed to, uint256 value);
    event Approval(address indexed owner, address indexed spender, uint256 value);
}

contract MusicToken is IERC20 {
    // トークン名
    string public name = "MusicToken";
    // シンボル
    string public symbol = "MT";
    // 18桁
    uint8 public decimals = 18;
    // 総発行量
    uint256 private _totalSupply;
    
    // アドレスごとのトークンの保有量
    mapping(address => uint256) private _balances;
    // アドレスに対するトークンの承認量
    mapping(address => mapping(address => uint256)) private _allowances;

    // コントラクトの初期化
    constructor(uint256 initialSupply) {
        // トークンの総発行量の決定
        _totalSupply = initialSupply * 10 ** uint256(decimals);
        // オーナーが全トークンを保有する
        _balances[msg.sender] = _totalSupply;
        // トークンの総発行量をイベントログに発出
        emit Transfer(address(0), msg.sender, _totalSupply);
    }

    // 総発行量を確認する関数
    function totalSupply() public view override returns (uint256) {
        return _totalSupply;
    }

    // アドレスのトークン保有量を確認する関数
    function balanceOf(address account) public view override returns (uint256) {
        return _balances[account];
    }

    // トークンをアドレスからアドレスに送付する
    function transfer(address recipient, uint256 amount) public override returns (bool) {
        // 送金者が必要な量のトークンを保有しているか
        require(_balances[msg.sender] >= amount, "Insufficient balance");
        _balances[msg.sender] -= amount;
        _balances[recipient] += amount;
        emit Transfer(msg.sender, recipient, amount);
        return true;
    }

    // オーナーが使用者に許容しているトークン量
    function allowance(address owner, address spender) public view override returns (uint256) {
        return _allowances[owner][spender];
    }

    // オーナーが使用者に指定金額のトークン量を承認する
    function approve(address spender, uint256 amount) public override returns (bool) {
        _allowances[msg.sender][spender] = amount;
        emit Approval(msg.sender, spender, amount);
        return true;
    }

    // 使用者が別のアドレスに承認されている範囲のトークンを送付する
    function transferFrom(address sender, address recipient, uint256 amount) public override returns (bool) {
        require(_balances[sender] >= amount, "Insufficient balance");
        require(_allowances[sender][msg.sender] >= amount, "Allowance exceeded");

        _balances[sender] -= amount;
        _balances[recipient] += amount;
        _allowances[sender][msg.sender] -= amount;

        emit Transfer(sender, recipient, amount);
        return true;
    }
}
