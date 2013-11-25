#Webkit Plugin for Retroshare

#####Testing python support ( http://www.henning-schroeder.de/embeddedpyqt )

This embeds a set of webkit tabs in a plugin page, along with the startings of a javascript bridge<br>
Still some way to go to be handy, So far it can:<br>
-broadcast to compatable peers<br>
-send targeted messages<br>
-search indexed files<br>
-download from hash+size+name
-handle retroshare:// links (if the link is not downloaded, it opens in a tab)<br>
-get Peer/online/channel/forumList<br>
-peer details/avatar<br>
-download any url<br>
-open new tabs<br>
<br>
implemented tests include<br>
-file search<br>
-packet inspection on index<br>
-broadcast chat<br>
-direct chat<br>
-three.js circle... shared drawing... thing ported from three.js demo<s br>
-simple p2p physics test with "averaging" of pos/vel over network.<br/>
-simple p2p physics test with "ownership" of objects.<br/>
-car driving test - ported from ivanks keyboard demo, players broadcast positions<br/>
-3d broadcast/wall chat - draggable text nodes. Shift+mouse moves camera<br/>
-local storage notepad - uses html5 persistant storage and editable tags (plus a source editor)<br/>
-Live MicroBlogging. only Friends see your messages, and friends of a friend whose message you comment on(buggy)<br/>



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
#sudo apt-get install python-dev #use this lines instead of above on older systems<br>
sudo apt-get install python-mpltoolkits.basemap python-qt4<br>

###Build
qmake-qt4; make<br>

###install/run
copy the .so files to your .retroshare/extensions/ directory
**run RetroShare from the plugins directory!**
install/run can be acomplished by running the script ctestit.sh

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
