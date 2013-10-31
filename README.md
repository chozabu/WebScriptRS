#Webkit Plugin for Retroshare

This embeds webkit in a plugin page, along with the startings of a javascript bridge<br>
Still some way to go to be handy, So far it can broadcast to compatable peers, and send targeted messages<br>
implemented tests include<br>
-packet inspection on index<br>
-broadcast chat<br>
-direct chat<br>
-three.js circle... shared drawing... thing ported from three.js demo<s br>
-simple p2p physics test with "averaging" of pos/vel over network.<br/>
-simple p2p physics test with "ownership" of objects.<br/>
-car driving test - ported from ivanks keyboard demo, players broadcast positions<br/>
-3d broadcast/wall chat - ported from ivanks keyboard demo, players broadcast positions - draggable text nodes. Shift+mouse moves camera<br/>



#### Warning Included tests do not sanitise HTML. The JS has not been written with security in mind.
![My image](http://chozabu.net/stuff/WebScriptRS.png)



##### Warning Watch your cpu and network usage when running the physics/shared drawing tests.<br> Should be fine.

## Usage:

Build and install the plugin as usual.<br>
**run RetroShare from the directory with the html files**

###JS Bridge

currently you can send a message like this: <br>
<code>
var packetdata = {"type":"chatmessage","pos":"thiscouldbeanything"}; <br>
bridgeWV.broadcastMessage(JSON.stringify(packetdata)); <br>
</code>

or directly like this: <br>
<code>
bridgeWV.sendMessage(peer_id_as_string, JSON.stringify(packetdata)); <br>
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
