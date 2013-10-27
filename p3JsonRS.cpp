/*
    This file is part of the Zero Reserve Plugin for Retroshare.

    Zero Reserve is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Zero Reserve is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Zero Reserve.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "p3JsonRS.h"

#include "pqi/p3linkmgr.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>




// after getting data from 3 peers, we believe we're complete
static const int INIT_THRESHOLD = 3;

p3JsonRS::p3JsonRS(RsPluginHandler *pgHandler, RsPeers* peers, msgQue *msgin ) :
        RsPQIService( RS_SERVICE_TYPE_JSON_PLUGIN, CONFIG_TYPE_JSON_PLUGIN, 0, pgHandler ),

        m_peers(peers)
{
    addSerialType(new RsJsonSerialiser());
    pgHandler->getLinkMgr()->addMonitor( this );
    mMsgque = msgin;
    //tjd = tjdin;
    //tjd->p3service = this;
}

void p3JsonRS::msgPeer(std::string peerId, std::string msg){//, std::string message){
    RsJsonItem * item = new RsJsonItem();
    item->PeerId( peerId );
    item->setMessage(msg);
    sendItem( item );
}

void p3JsonRS::testit(){
    std::cout << "Would send data here" << std::endl;
}

void p3JsonRS::statusChange(const std::list< pqipeer > &plist)
{
    std::cerr << "Json: Status changed:" << std::endl;

    for (std::list< pqipeer >::const_iterator peerIt = plist.begin(); peerIt != plist.end(); peerIt++ ){
        if( RS_PEER_CONNECTED & (*peerIt).actions ){
            RsJsonItem * item = new RsJsonItem();
            //item->RS_PKT_SUBTYPE
            item->PeerId( (*peerIt).id );
            std::stringstream ss;
            ss << "JINT " << "nameperhaps";
            item->setMessage(ss.str());
            std::cout << "sending message: " << item->getMessage() << "\n";
            sendItem( item );
            //tjd->addPeerItem((*peerIt).id);//do this on message recept
        }else if( RS_PEER_DISCONNECTED & (*peerIt).actions ){
            //TODO remove peer from send list
            }
    }
}


int p3JsonRS::tick()
{
    RsItem *item = NULL;
    while(NULL != (item = recvItem())){
        switch( item->PacketSubType() )
        {
        case RsJsonItem::JSON_ITEM:
            handleJsonItem( dynamic_cast<RsJsonItem*>( item ) );
            break;
        default:
            std::cerr << "Json: Received Item unknown" << std::endl;
        }
        delete item;
    }
    return 0;
}


void p3JsonRS::handleJsonItem( RsJsonItem * item )
{
    std::cout << "GOT MSG\n";
    std::string msg = item->getMessage();
    std::cout << msg << "\n\n";
    if (msg.substr(0,4).compare("JINT") == 0){
        compatablePeers.insert(item->PeerId().c_str(),msg.c_str());
        return;
    }
    QStringList qMsg;
    qMsg.append(QString(item->getMessage().c_str()));
    qMsg.append(QString(item->PeerId().c_str()));
    qMsg.append(QString(rsPeers->getPeerName(item->PeerId()).c_str()));
    bridge->pushMsgToJs(qMsg);
    mMsgque->storeMsg(item);
}


