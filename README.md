#Webkit Plugin for Retroshare

This embeds webkit in a plugin page, along with the startings of a javascript bridge<br>
Still some way to go to be handy, So far it can broadcast, and perhaps send targeted messages<br>
implemented tests include<br>
-packet inspection on index
-broadcast chat<br>
-three.js circle... shared drawing... thing<br>
-simple p2p physics test with "averaging" of pos/vel over network.<br/>
-simple p2p physics test with "ownership" of objects.<br/>






###### Warning Watch your cpu and network usage when running the latter two tests.<br> Should be fine.

## Usage:

Build and install the plugin as usual.<br>
**run RetroShare from the directory with the html files**

###JS Bridge

currently you can send a message like this: <br>
<code>
var packetdata = {"type":"chatmessage","pos":"thiscouldbeanything"}; <br>
bridgeWV.broadcastMessage(JSON.stringify(packetdata)); <br>
</code>

And get one like this: <br>
<code>
bridge.msgPush.connect(incomingMsg); <br>
function incomingMsg(message){ <br>
  packetdata = JSON.parse(message.message); <br>
  id = message.peerID; <br>
  name = message.peerName;
}
</code>
