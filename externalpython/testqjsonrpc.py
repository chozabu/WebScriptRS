#!/usr/bin/python

import jsonrpc
import socket

rpc = jsonrpc.ServerProxy(jsonrpc.JsonRpc20(), jsonrpc.TransportSocket(addr="/tmp/testservice", sock_type=socket.AF_UNIX))
print "Hi, "+rpc.bridge.getPeerName(rpc.bridge.getOwnId())
