#include "webviewrs.h"
#include <QDebug>
#include <QWebFrame>
#include "webbridgers.h"
#include "gui/RetroShareLink.h"

WebViewRS::WebViewRS(QWidget *parent) :
    QWebView(parent)
{
    connect( this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),    this,   SLOT(onJavaScriptWindowObjectCleared()) );
    this->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
    connect( this->page(), SIGNAL(linkClicked(const QUrl & )),    this,   SLOT(onLinkClicked(const QUrl & )) );
    settings()->enablePersistentStorage();


}
WebViewRS::~WebViewRS()
{
    //this->close();
}
void WebViewRS::onLinkClicked(const QUrl & url){
    std::cout << "processing webkut url: " << url.toString().toStdString().c_str() << std::endl;
    if(url.toString().startsWith("retroshare")){
        QStringList urls;
        urls.push_back(url.toString());
        RetroShareLink::process(urls, RetroShareLink::TYPE_UNKNOWN, RSLINK_PROCESS_NOTIFY_ALL);
    } else {
        this->setUrl(url);
    }
}

void WebViewRS::onJavaScriptWindowObjectCleared()
{
    QWebFrame *frame = this->page()->mainFrame();
    //frame->setZoomFactor(4);
    frame->addToJavaScriptWindowObject("webview", this);
    frame->addToJavaScriptWindowObject("bridge", p3service->bridge);
}

void WebViewRS::setP3service(p3JsonRS *p3servicein)
{
    p3service = p3servicein;
}


void WebViewRS::go2page(QString url)
{
    this->setUrl(QUrl(url));

}

void WebViewRS::onChange()
{
    this->setUrl(QUrl("canvas_interactive_particles.html"));// << "Browser::onChange()";
    qDebug() << "Browser::onChange()";
}
