make clean
qmake-qt4
make clean
make
kdesudo -u retrotester cp libEmptyPlugin.so* /home/retrotester/.retroshare/extensions/
kdesudo -u retrotester ../../retroshare-gui/src/RetroShare
