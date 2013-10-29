#include "webbridgers.h"

#include "p3JsonRS.h"

#include <QString>
WebBridgeRS::WebBridgeRS(QObject *parent) :
    QObject(parent)
{

    /*
    rsPeers->getOnlineList();
    rsPeers->getFriendList();
    rsFiles->FileDetails();
    rsFiles->SearchKeywords();
    */
    //rsFiles->FileRequest();
    //virtual bool FileRequest(const std::string& fname, const std::string& hash, uint64_t size, const std::string& dest, TransferRequestFlags flags, const std::list<std::string>& srcIds) = 0;

}

void WebBridgeRS::pushMsgToJs(QVariantMap message)
{
    emit msgPush(message);
}

QString WebBridgeRS::getOwnId()
{
    return QString(rsPeers->getOwnId().c_str());
}
QString WebBridgeRS::getDownloadDirectory()
{
    return QString(rsFiles->getDownloadDirectory().c_str());
}
