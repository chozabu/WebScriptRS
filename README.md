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

![My image](http://chozabu.net/stuff/microblog12nov2013.png)
![My image](http://chozabu.net/stuff/search12nov2013.png)
![My image](http://chozabu.net/stuff/index12nov2013.png)
![My image](http://chozabu.net/stuff/WebScriptRS.png)
![My image](http://chozabu.net/stuff/WebScriptRS1.png)
![My image](http://chozabu.net/stuff/WebScriptRS2.png)

##### Warning Watch your cpu and network usage when running the physics/shared drawing tests.<br> Should be fine.

## Usage:

###PreReq:

sudo apt-get install libpython-dev libpython-all-dev 
sudo apt-get install python-dev #use this lines instead of above on older systems 
sudo apt-get install python-qt4 
 
if you want to run the Geo Friend Map Test: 
sudo apt-get install python-mpltoolkits.basemap 
sudo pip install pygeoip 

###Build/Install
##PreReq
You will need a linux machine 
<pre>
  apt-get install python-dev python-qt4
</pre>
is a good start,  
<pre>
  apt-get install python-mpltoolkits.basemap 
</pre>
Will let you run the geoip peermap demo   
please let me know if you need to install anything else to make it run.
##compile
you probably want to clone this into the plugins directory of your retroshare source code. 
<pre>
  cd myretrosharesrc/plugins/  
  git clone https://github.com/chozabu/WebScriptRS.git  
cd WebScriptRS 
</pre>
At this point you can run the initalbuild.sh script or: 
<pre>
  git clone https://bitbucket.org/devonit/qjsonrpc.git  
  cd qjsonrpc/src/  
  qmake-qt4 QJSONRPC_LIBRARY_TYPE=staticlib; make  
  cd ..  
  qmake-qt4; make  
</pre>
###install/run
copy the .so files to your .retroshare/extensions/ directory  
**run RetroShare from the plugins directory!**  
install/run can be acomplished by running the script ctestit.sh 

###JS Bridge

There is not much documentation yet - but there are examples, and you can look at webbridgers.h  
currently you can send a message like this: 
<pre>
var packetdata = {"type":"chatmessage","pos":"thiscouldbeanything"};  
bridge.broadcastMessage(JSON.stringify(packetdata)); 
</pre>

or directly like this: 
<pre>
var packetdata = {"x":3,"y":5,"name":"Eric"};  
bridge.sendMessage(peer_id_as_string, JSON.stringify(packetdata)); 
</pre>
And get one like this: 
<pre>
bridge.msgPush.connect(incomingMsg);  
function incomingMsg(message){  
  var packetdata = JSON.parse(message.message);  
  var peer_id_as_string = message.peerID;  
  var name = message.peerName;  
}
</pre>
