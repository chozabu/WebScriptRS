#!/usr/bin/python

import jsonrpc
import socket


rpc = jsonrpc.ServerProxy(jsonrpc.JsonRpc20(), jsonrpc.TransportSocket(addr="/tmp/testservice", sock_type=socket.AF_UNIX))

import random
import json
#import PyQt4
print ""
#print 'names in current namespace', dir()
print "dumping subscribed forums... please wait!"
forums = rpc.bridge.getForums()
with open('../jout/forums.json', 'w') as ffile:
    ffile.write(json.dumps(forums))
def dumpMsg(forumID,msgID):
            #print msgID
            msg = rpc.bridge.getForumMessage(forumID,msgID)
            with open("../jout/msg"+msgID+".json", 'w') as ffile:
                ffile.write(json.dumps(msg))
            msgheaders = rpc.bridge.getForumThreadMsgList(forumID,msgID)
            with open("../jout/thread"+msgID+".json", 'w') as ffile:
                ffile.write(json.dumps(msgheaders))
            for msgh in msgheaders:
                dumpMsg(forumID, msgh['msgId'])
for f in forums:
    if f['forumRelationship'] == "SUBSCRIBED":
        print "dumping " + f['forumId']
        threadlist = rpc.bridge.getForumThreadList(f['forumId'])
        with open("../jout/forum"+f['forumId']+".json", 'w') as ffile:
            ffile.write(json.dumps(threadlist))
        for tm in threadlist:
            #print tm
            dumpMsg(f['forumId'], tm['msgId'])

print "done dumping forum messages"
