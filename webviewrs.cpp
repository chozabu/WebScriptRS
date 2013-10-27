#include "webviewrs.h"
#include <QDebug>
#include <QWebFrame>
#include "webbridgers.h"

WebViewRS::WebViewRS(QWidget *parent) :
    QWebView(parent)
{
    //this->
    //this->page()->mainFrame()->addToJavaScriptWindowObject( "bridgeWV", this );
    //this->acceptDrops()
    //connect(this->page()->mainFrame(), SIGNAL (javaS))
    connect( this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),    this,   SLOT(onJavaScriptWindowObjectCleared()) );
    bridge = new WebBridgeRS();



}
void WebViewRS::onJavaScriptWindowObjectCleared()
{
    QWebFrame *frame = this->page()->mainFrame();
    //frame->setZoomFactor(4);
    frame->addToJavaScriptWindowObject("bridgeWV", this);
    frame->addToJavaScriptWindowObject("bridge", bridge);
 //   QString script = "console.log('init!');";
    //   this->page()->mainFrame()->evaluateJavaScript( script );
}

void WebViewRS::setP3service(p3JsonRS *p3servicein)
{
    p3service = p3servicein;
    p3service->bridge = bridge;
}
void WebViewRS::broadcastMessage(QString msg)
{
    QMapIterator<QString, QString> mi(p3service->compatablePeers);
    //std::cout << "boardcasting\n";
    while ( mi.hasNext() ) {
      mi.next();
      //std::cout << "to: " << mi.key().toStdString() << std::endl;
      //qDebug() << mi.key() << ":" << mi.value().surname() << mi.value().forename();
      p3service->msgPeer(mi.key().toStdString(),msg.toStdString());
  }
}
void WebViewRS::sendMessage(QString peer, QString msg)
{
    p3service->msgPeer(peer.toStdString(),msg.toStdString());
}

QVariantList WebViewRS::getMessages()
{
    std::vector<RsJsonItem> *list = p3service->mMsgque->getMsgList();
    QVariantList qMsgList;
    //std::cout << "checking list" << std::endl;
    for(std::vector<RsJsonItem>::iterator it = list->begin(); it != list->end(); ++it) {
        RsJsonItem item = (*it);
        std::cout << "passin: " << item.getMessage() << std::endl;
        QStringList qMsg;
        qMsg.append(QString(item.getMessage().c_str()));
        qMsg.append(QString(item.PeerId().c_str()));
        qMsg.append(QString(rsPeers->getPeerName(item.PeerId()).c_str()));
        qMsgList.append(qMsg);
    }
    return qMsgList;
}

QMapIterator<QString, QString> WebViewRS::getPeers()
{
    return p3service->compatablePeers;
}

void WebViewRS::go2index2()
{
    this->setUrl(QUrl("index2.html"));// << "Browser::onChange()";

}

void WebViewRS::go2page(QString url)
{
    this->setUrl(QUrl(url));

}

QString WebViewRS::echothis(QString in)
{
    return in;
}

void WebViewRS::onChange()
{

    this->setUrl(QUrl("canvas_interactive_particles.html"));// << "Browser::onChange()";
    qDebug() << "Browser::onChange()";
}
//void WebViewRS::set
