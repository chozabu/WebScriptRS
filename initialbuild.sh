
git clone https://bitbucket.org/devonit/qjsonrpc.git
cd qjsonrpc/src/
qmake-qt4 QJSONRPC_LIBRARY_TYPE=staticlib; make
cd ..
qmake-qt4; make
