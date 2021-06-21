# Ruby Language installation / update

## MacOSX Preparation

### Homebrew installation and environment maintenance

```
ruby -e "$ (curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

### Verification

```
brew doctor
```

* If you get an error or warning message, follow the instructions
* Repeat brew doctor until no warnings appear


```
Your system is ready to brew.
```

Is OK

* If the error or warning doesn't go away, uninstall homebrew and start over.

```
Uninstall # homebrew

ruby -e "$ (curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/uninstall)"
```

### Installing Ruby on MacOSX

Creating installation scripts for rbenv and related libraries

```
nano install_rbenv.sh
```

```
#! / bin / bash
brew update
brew upgrade

brew install gcc
brew install git
brew install leveldb
brew install direnv
brew install git
brew install nodejs
brew install ruby-dev
brew install curl
brew install imagemagick
brew install yarn
brew install npm
brew install sqlite3
brew install readline
brew install zlib
sudo npm install n -g
sudo n stable

rm -fr ~ / .rbenv

git clone https://github.com/rbenv/rbenv.git ~ / .rbenv
git clone https://github.com/rbenv/ruby-build.git ~ / .rbenv / plugins / ruby-build
curl -fsSL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-doctor | bash

echo'export PATH = "$ HOME / .rbenv / bin: $ PATH"' >> ~ / .zshrc
echo'eval "$ (rbenv init-)"' >> ~ / .zshrc
source ~ / .zshrc
source ~ / .zprofile
brew install rbenv
```

#### Change and execute permissions of rbenv installation script

```
chmod u + x install_rbenv.sh
./install_rbenv.sh
```


Rebooting the shell

```
exec $ SHELL -l
```


#### Installing Ruby with rbenv

```
rbenv install 3.0.1
rbenv global 3.0.1
```

Rebooting the shell

```
exec $ SHELL -l
```


## ubuntu


Installation script file

### Install rbenv

```
nano install_rbenv.sh
```

```
#! / bin / bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y build-essential
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y direnv
sudo apt install -y git
sudo apt install -y nodejs
sudo apt install -y ruby-dev
sudo apt install -y curl
sudo apt install -y imagemagick
curl -fsSL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-doctor | bash
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add-
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
sudo apt update

sudo apt install -y yarn
sudo apt install -y npm
sudo npm install n -g
yarn install
yarn upgrade
sudo n stable
sudo apt purge -y nodejs npm
exec $ SHELL -l
sudo apt install -y sqlite3 libsqlite3-dev
sudo apt install -y libssl-dev libreadline-dev zlib1g-dev

apt-get install -y libreadline-dev zlib1g-dev
rm -fr ~ / .rbenv

git clone https://github.com/rbenv/rbenv.git ~ / .rbenv
git clone https://github.com/rbenv/ruby-build.git ~ / .rbenv / plugins / ruby-build
echo'export PATH = "$ HOME / .rbenv / bin: $ PATH"' >> ~ / .bash_profile
echo'eval "$ (~ / .rbenv / bin / rbenv init-)"' >> ~ / .bash_profile
echo'export PATH = "$ HOME / .rbenv / bin: $ PATH"' >> ~ / .bashrc
echo'eval "$ (rbenv init-)"' >> ~ / .bashrc
source ~ / .bashrc
```

#### Change and execute permissions of rbenv installation script

```
chmod u + x install_rbenv.sh
./install_rbenv.sh
```

Rebooting the shell

```
exec $ SHELL -l
```


#### Installing Ruby with rbenv

```
rbenv install 3.0.1
rbenv global 3.0.1
```




## Check version

```
ruby -v
```