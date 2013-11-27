from embeddedpyqt import *
import random
import json
#import PyQt4
print ""
#print 'names in current namespace', dir()
print "dumping subscribed forums... please wait!"
forums = bridge.getForums()
with open('jout/forums.json', 'w') as ffile:
    ffile.write(json.dumps(forums))
def dumpMsg(forumID,msgID):
            #print msgID
            msg = bridge.getForumMessage(forumID,msgID)
            with open("jout/msg"+msgID+".json", 'w') as ffile:
                ffile.write(json.dumps(msg))
            msgheaders = bridge.getForumThreadMsgList(forumID,msgID)
            with open("jout/thread"+msgID+".json", 'w') as ffile:
                ffile.write(json.dumps(msgheaders))
            for msgh in msgheaders:
                dumpMsg(forumID, msgh['msgId'])
for f in forums:
    if f['forumRelationship'] == "SUBSCRIBED":
        print "dumping " + f['forumId']
        threadlist = bridge.getForumThreadList(f['forumId'])
        with open("jout/forum"+f['forumId']+".json", 'w') as ffile:
            ffile.write(json.dumps(threadlist))
        for tm in threadlist:
            #print tm
            dumpMsg(f['forumId'], tm['msgId'])

print "done dumping forum messages"
