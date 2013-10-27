#include "webbridgers.h"

#include "p3JsonRS.h"
WebBridgeRS::WebBridgeRS(QObject *parent) :
    QObject(parent)
{

}

void WebBridgeRS::pushMsgToJs(QVariantMap message)
{
 emit msgPush(message);
}
