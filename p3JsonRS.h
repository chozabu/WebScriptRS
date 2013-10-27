

#ifndef P3ZERORESERVERRS_H
#define P3ZERORESERVERRS_H


#include "RSJsonItems.h"

#include "retroshare/rspeers.h"
#include "plugins/rspqiservice.h"
#include "pqi/pqimonitor.h"
//#include "topjcdialog.h"

#include "msgQue.h"
#include "webbridgers.h"
#include <QMap>
#include <QString>


//class WebScriptDialog;
class RsPluginHandler;
class RsPeers;

class p3JsonRS : public RsPQIService,   // the service interface calls tick()
                    public pqiMonitor      // the monitor tells us when friends changed their status
{
public:
    p3JsonRS(RsPluginHandler *pgHandler, RsPeers* peers, msgQue *msgin);

    virtual int tick();
    virtual void statusChange(const std::list<pqipeer> &plist);
    virtual void testit();
    virtual void msgPeer(std::string peerId, std::string msg);//, std::string message){
    msgQue * mMsgque;
    WebBridgeRS * bridge;
    QMap<QString,QString> compatablePeers;

private:
    void handleMessage( RsJsonItem *item );
    void handleJsonItem( RsJsonItem * item );

private:

    RsPeers * m_peers;
};

#endif // P3ZERORESERVERRS_H
