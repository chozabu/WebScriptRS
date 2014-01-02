#!/usr/bin/python

import jsonrpc
import socket
import random
import json
import sys

print "dumping subscribed forums... please wait!"
rpc = jsonrpc.ServerProxy(jsonrpc.JsonRpc20(), jsonrpc.TransportSocket(addr="/tmp/testservice", sock_type=socket.AF_UNIX))
print "loading forums..."
forums = rpc.bridge.getForums()
print "loading keyring..."
gpgIdList = rpc.bridge.getGPGAllList()
print "gen name hash..."
gpgIds = {}
for i in gpgIdList:
  n = rpc.bridge.getPeerName(i)
  print n
  gpgIds[i]=n
print "writing forums list..."

with open('../jout/forums.json', 'w') as ffile:
    ffile.write(json.dumps(forums))
def dumpMsg(forumID,msgID):
            #print msgID
            msg = rpc.bridge.getForumMessage(forumID,msgID)
            sys.stdout.write('   '+msg['srcId']+': ')
            if msg['srcId'] != '':
              msg['srcId'] = gpgIds[msg['srcId']]
              sys.stdout.write(msg['srcId']+' ')
            with open("../jout/msg"+msgID+".json", 'w') as ffile:
                ffile.write(json.dumps(msg))
            msgheaders = rpc.bridge.getForumThreadMsgList(forumID,msgID)
            with open("../jout/thread"+msgID+".json", 'w') as ffile:
                ffile.write(json.dumps(msgheaders))
            for msgh in msgheaders:
                dumpMsg(forumID, msgh['msgId'])
print "writing forums..."
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
