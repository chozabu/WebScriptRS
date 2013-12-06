from embeddedpyqt import *
import json
import hashlib

def dsHash(data):
    return hashlib.sha512(data).hexdigest()[0:20]
def dhHashId(Id):
    fi=bridge.getPeerDetails(Id)
    return dsHash(dsHash(fi['id'])+dsHash(fi['name']))

print ""
print "starting anon peer connections dump for global peermap"
todump = {"alias":"anon","ownHash":dhHashId(bridge.getOwnId())}
dumplist=[]
for f in bridge.getFriendList():
    dumplist.append(dhHashId(f))
todump['contactHashes']=dumplist

todump=json.dumps(todump)
print "Writing data: "+todump
outfile = bridge.getDownloadDirectory()+"/global-peer-map-from-"+dhHashId(bridge.getOwnId())+".json"
print "to: "+outfile
with open(outfile, 'w') as ffile:
    ffile.write(todump)

print "done dumping anon peer connections for global peermap"
