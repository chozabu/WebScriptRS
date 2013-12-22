Scripting/IPC Plugin for RetroShare
=========

###Overview
This plugin attempts to make it easy to write scripts or external programs than can interact with retroshare-gui by providing a bridge to Retroshare via the following transports:
- Embedded WebKit Tabs
- python console+file loading ( http://www.henning-schroeder.de/embeddedpyqt )
- JSON-RPC Server (no Push) ( https://bitbucket.org/devonit/qjsonrpc )

Some of the functions provided include:
- broadcast to compatable peers
- send targeted messages
- search indexed files
- turtle file search
- download from hash+size+name
- handle retroshare:// links (if the link is not downloaded, it opens in a tab)
- get Peer/online/channel/forumList
- peer details/avatar
- download any url
- open new tabs
- Lots more

Examples include:
- geographical peer map (pyqt)
- global peer map (python dump, d3.js view)
- car driving test - ported from ivanks keyboard demo, players broadcast positions<br/>
- 3d broadcast/wall chat - draggable text nodes. Shift+mouse moves camera<br/>
- external python via JSON-RPC
- file search
- packet inspection on index
- broadcast chat
- direct chat
- simple distributed p2p physics tests
- local storage notepad - uses html5 persistant storage and editable tags (plus a source editor)
- Live MicroBlogging. only Friends see your messages, and friends of a friend whose message you comment on(buggy)
- Lots more


#### Warning Included tests do not sanitise HTML. The JS has not been written with security in mind.
index12nov2013.png  microblog12nov2013.png  search12nov2013.png

![My image](http://chozabu.net/stuff/microblog12nov2013.png)
![My image](http://chozabu.net/stuff/search12nov2013.png)
![My image](http://chozabu.net/stuff/index12nov2013.png)
![My image](http://chozabu.net/stuff/WebScriptRS.png)
![My image](http://chozabu.net/stuff/WebScriptRS1.png)
![My image](http://chozabu.net/stuff/WebScriptRS2.png)

##### Warning Watch your cpu and network usage when running the physics/shared drawing tests.<br> Should be fine.

## Usage:

###PreReq:

sudo apt-get install libpython-dev libpython-all-dev<br>
sudo apt-get install python-dev #use this lines instead of above on older systems<br>
sudo apt-get install python-qt4<br>
<br>
if you want to run the Geo Friend Map Test:<br>
sudo apt-get install python-mpltoolkits.basemap<br>
sudo pip install pygeoip<br>

###Build
you probably want to clone this into the plugins directory of your retroshare source code.<br>
cd myretrosharesrc/plugins/ <br>
git clone git@github.com:chozabu/WebScriptRS.git<br>
cd WebScriptRS<br>
At this point you can run the initalbuild.sh script or:<br>
  git clone https://bitbucket.org/devonit/qjsonrpc.git<br>
  cd qjsonrpc/src/<br>
  qmake-qt4 QJSONRPC_LIBRARY_TYPE=staticlib; make<br>
  cd ..<br>
  qmake-qt4; make<br>

###install/run
copy the .so files to your .retroshare/extensions/ directory<br>
**run RetroShare from the plugins directory!**<br>
install/run can be acomplished by running the script ctestit.sh<br>

###JS Bridge

There is not much documentation yet - but there are examples, and you can look at webbridgers.h <br>
currently you can send a message like this: <br>
<code>
var packetdata = {"type":"chatmessage","pos":"thiscouldbeanything"}; <br>
bridge.broadcastMessage(JSON.stringify(packetdata)); <br>
</code>

or directly like this: <br>
<code>
var packetdata = {"x":3,"y":5,"name":"Eric"}; <br>
bridge.sendMessage(peer_id_as_string, JSON.stringify(packetdata)); <br>
</code>
And get one like this: <br>
<code>
bridge.msgPush.connect(incomingMsg); <br>
function incomingMsg(message){ <br>
  var packetdata = JSON.parse(message.message); <br>
  var peer_id_as_string = message.peerID; <br>
  var name = message.peerName;
}
</code>
