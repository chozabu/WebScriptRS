#include "webviewrs.h"
#include <QDebug>
#include <QWebFrame>
#include "webbridgers.h"
#include "gui/RetroShareLink.h"
#include "gui/notifyqt.h"

WebViewRS::WebViewRS(QWidget *parent) :
    QWebView(parent)
{
    connect( this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),    this,   SLOT(onJavaScriptWindowObjectCleared()) );
    this->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
    connect( this->page(), SIGNAL(linkClicked(const QUrl & )),    this,   SLOT(onLinkClicked(const QUrl & )) );
    settings()->enablePersistentStorage();

    connect(NotifyQt::getInstance(), SIGNAL(downloadComplete(QString)), this, SLOT(onDownloadComplete(QString)));


}
WebViewRS::~WebViewRS()
{
    //this->close();
}
void WebViewRS::onDownloadComplete(QString hash){
    if (hash == hashque){

        FileInfo fi;
        /* look up path */
        if (rsFiles->alreadyHaveFile(hash.toStdString(), fi)){
            QUrl newurl(QString(fi.path.c_str()));
            this->setUrl(newurl);
            return;
        }
    }
}

void WebViewRS::onLinkClicked(const QUrl & url){
    std::cout << "processing webkut url: " << url.toString().toStdString().c_str() << std::endl;
    if(url.toString().startsWith("retroshare")){
        //retroshare://file?name=Distro_guide.png&size=946336&hash=68282a3813fa284ab3c997c9702599b44c6db568
        RetroShareLink rsl(url);
        if (rsl.name().endsWith(".png") || rsl.name().endsWith(".gif") || rsl.name().endsWith(".jpg") || rsl.name().endsWith(".txt")  ||
                rsl.name().endsWith(".html") || rsl.name().endsWith(".htm") || rsl.name().endsWith(".mp3")){
                    loadRSFile(rsl.name(), rsl.hash(), rsl.size());
                    return;
        }
        QStringList urls;
        urls.push_back(url.toString());
        RetroShareLink::process(urls, RetroShareLink::TYPE_UNKNOWN, RSLINK_PROCESS_NOTIFY_ALL);
    } else {
        this->setUrl(url);
    }
}

void WebViewRS::loadRSFile(QString qname, QString qhash, int qsize){

    FileInfo fi;

    /* look up path */
    if (rsFiles->alreadyHaveFile(qhash.toStdString(), fi)){
        QUrl newurl(QString(fi.path.c_str()));
        this->setUrl(newurl);
        return;
    }
    hashque = qhash;
    // Get a list of available direct sources, in case the file is browsable only.
    //
    FileInfo finfo ;
    rsFiles->FileDetails(qhash.toStdString(), RS_FILE_HINTS_REMOTE, finfo) ;

    std::list<std::string> srcIds;
    for(std::list<TransferInfo>::const_iterator it(finfo.peers.begin());it!=finfo.peers.end();++it)
    {
    #ifdef DEBUG_RSLINK
        std::cerr << "  adding peerid " << (*it).peerId << std::endl ;
    #endif
        srcIds.push_back((*it).peerId) ;
    }

    /*QString cleanname = link.name() ;
    static const QString bad_chars_str = "/\\\"*:?<>|" ;

    for(int i=0;i<cleanname.length();++i)
        for(int j=0;j<bad_chars_str.length();++j)
            if(cleanname[i] == bad_chars_str[j])
            {
                cleanname[i] = '_';
                flag |= RSLINK_PROCESS_NOTIFY_BAD_CHARS ;
            }
    */
    if (rsFiles->FileRequest(qname.toStdString(), qhash.toStdString(), qsize, "", RS_FILE_REQ_ANONYMOUS_ROUTING, srcIds)) {
    } else {
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
