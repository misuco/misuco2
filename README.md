# MISUCO2
Microtonal surface controller - a multitouch music performance application with OSC (Open Sound Control) support and built in Synthesizer
## Build under Debian 13 (trixie)
```
# install dependencies
apt-get install qtcreator qtbase5-dev qt5-qmake qtbase5-dev-tools qtquickcontrols2-5-dev qml-module-qtquick-controls qml-module-qtquick-dialogs qml-module-qtquick-extras qml-module-qtquick-layouts qml-module-qtquick2 
 qtdeclarative5-dev libasound2-dev

# clone code repo
git clone https://github.com/misuco/misuco2.git

# initialize out of source build directory
mkdir misuco2-build

# build
cd misuco2-build
qmake ../misuco2/misuco2.pro
make
```

## Run under Debian 13 (trixie)
After build, in misuco2-build directory:
```
./misuco2
```
