#!/usr/bin/python

import jsonrpc
import socket

rpc = jsonrpc.ServerProxy(jsonrpc.JsonRpc20(), jsonrpc.TransportSocket(addr=("127.0.0.1",11111)))
print "Hi, "+rpc.bridge.getPeerName(rpc.bridge.getOwnId())
