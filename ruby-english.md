# How to install Ruby

2022/03/24

## Installation on MacOSX

#### Homebrew installation and environment maintenance

```bash
ruby -e "$ (curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

#### Confirmation

```bash
brew doctor
```

* If you get an error or warning message, follow the instructions
* Repeat brew doctor until no warnings appear

OK if this message appears

```
Your system is ready to brew.
```

* If the error or warning doesn't go away, uninstall homebrew and then reinstall homebrew to avoid any extra trouble.

```bash
How to uninstall #homebrew
ruby -e "$ (curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/uninstall)"
```

#### Installation of necessary libraries

```bash
brew update
brew upgrade

brew install llvm autoconf gcc make curl direnv readline sqlite3 sqlite-utils libsigsegv leveldb zlib zlib-ng git nodejs node npm m4 wget curl
```

#### Installation of rbenv

```bash
cd ~
rm -fr ~ / .rbenv
git clone https://github.com/rbenv/rbenv.git ~ / .rbenv
```

#### Editing the login script (.zshrc)

`nano .zshrc`

Add the following two lines to the end of the file and save it

```bash
export PATH = "~ / .rbenv / bin: $ PATH"
eval "$ (rbenv init-)"
```

#### Evaluate the login script

```bash
source ~ / .zshrc
```

#### Check the rbenv command

There will be an installable version of Ruby

```bash
git clone https://github.com/rbenv/ruby-build.git "$ (rbenv root)" / plugins / ruby-build
rbenv install -l



2.6.9
2.7.5
3.0.3
3.1.1
jruby-9.3.3.0
mruby-3.0.0
rbx-5.0
truffleruby-22.0.0.2
truffleruby + graalvm-22.0.0.2

Only latest stable releases for each Ruby implementation are shown.
Use'rbenv install --list-all / -L' to show all local versions.
```

If you want to install a new version of Ruby, update the rbenv environment as follows

```bash
git clone https://github.com/rbenv/ruby-build.git "$ (rbenv root)" / plugins / ruby-build
```

### Ruby installation

#### Install by specifying the version

The latest 3.X.X versions are fast, but may not be stable.
Here, we will use 2.7.5.

It takes a lot of time because it compiles etc.

```bash
rbenv install 2.7.5
```

#### Specify the version of Ruby to use

You can install multiple versions of Ruby using rbenv.
Specify the version to be used in it as follows

```bash
rbenv global 2.7.5
source .zshrc
```

Checking the actual Ruby version

```bash
ruby -v
ruby 2.7.5p203 (2021-11-24 revision f69aeb8314) [arm64-darwin21]
```


## Installation on Ubuntu

#### Installation of necessary libraries

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y llvm autoconf gcc clang cmake curl direnv sqlite3 libsqlite3-dev git nodejs m4 libssl-dev zlib1g-dev cmdtest build-essential wget imagemagick libreadline-dev
sudo apt autoremove -y
```

#### Installation of rbenv

```bash
cd ~
rm -fr ~ / .rbenv
git clone https://github.com/rbenv/rbenv.git ~ / .rbenv
```

#### Editing the login script (.bashrc)

`nano .bashrc`

Add the following two lines to the end of the file and save it

```bash
export PATH = "~ / .rbenv / bin: $ PATH"
eval "$ (rbenv init-)"
```

#### Login script rating

```bash
source ~ / .bashrc
```

#### Check the rbenv command

There will be an installable version of Ruby

```bash
git clone https://github.com/rbenv/ruby-build.git "$ (rbenv root)" / plugins / ruby-build
rbenv install -l


2.6.9
2.7.5
3.0.3
3.1.1
jruby-9.3.3.0
mruby-3.0.0
rbx-5.0
truffleruby-22.0.0.2
truffleruby + graalvm-22.0.0.2

Only latest stable releases for each Ruby implementation are shown.
Use'rbenv install --list-all / -L' to show all local versions.
```

If you want to install a new version of Ruby, update the rbenv environment as follows

```bash
git clone https://github.com/rbenv/ruby-build.git "$ (rbenv root)" / plugins / ruby-build
```

### Ruby installation

#### Install by specifying the version

The latest 3.X.X versions are fast, but may not be stable.
Here, we will use 2.7.5.

It takes a lot of time because it compiles etc.

```bash
rbenv install 2.7.5
```

#### Specify the version of Ruby to use

You can install multiple versions of Ruby using rbenv.
Specify the version to be used in it as follows

```bash
rbenv global 2.7.5
source .bashrc
```

Checking the Ruby version

```bash
ruby -v
ruby 2.7.5p203 (2021-11-24 revision f69aeb8314) [arm64-darwin21]
``` 
