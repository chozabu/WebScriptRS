#include "topjcdialog.h"
#include "ui_topjcdialog.h"
#include "webtabcontents.h"
#include "pytabcontents.h"

#include <QtGui>


#include <QLocalServer>
#include <QFile>
#include <QDir>

#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif //QT_VERSION >= 0x050000


/** Constructor
LinksDialog::LinksDialog(RsPeers *peers, RsFiles *files, QWidget *parent)
: MainPage(parent), mPeers(peers), mFiles(files)
{

}**/

WebScriptDialog::WebScriptDialog(QWidget *parent) :
    MainPage(parent),
    ui(new Ui::WebScriptDialog)
{
    ui->setupUi(this);

    embpyqt = new EmbeddedPyQt();

    bridge = new WebBridgeRS();
    bridge->embpyqt = embpyqt;


    embpyqt->registerObject(*bridge);
    embpyqt->registerMetaObject(WebBridgeRS::staticMetaObject);
    embpyqt->registerObject(*this);
    embpyqt->init("embpyqt/python/initembpyqt.py");
    embpyqt->execute("from embeddedpyqt import *", true);

    connect(ui->newTabBtn, SIGNAL(clicked()), this, SLOT(addTab()));
    connect(ui->closeTabBtn, SIGNAL(clicked()), this, SLOT(removeTab()));
    connect(ui->pythonBtn, SIGNAL(clicked()), this, SLOT(doPython()));
    connect(ui->rpcBtn, SIGNAL(clicked()), this, SLOT(startRPC()));
    connect( bridge, SIGNAL(newTabUrl(QString)),    this,   SLOT(onNewTabUrl(QString)) );
}

void WebScriptDialog::startRPC(){
#if QT_VERSION >= 0x050000
    QDir tempDirectory(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
#else
    QDir tempDirectory(QDesktopServices::storageLocation(QDesktopServices::TempLocation));
#endif //QT_VERSION >= 0x050000
    QString serviceName = tempDirectory.absoluteFilePath("testservice");
    //serviceName = "testservice";
    std::cerr << "starting RPC Server\n";
    if (QFile::exists(serviceName) && !QFile::remove(serviceName)) {
         std::cerr << "couldn't delete temporary service";
    }else{

        std::cerr << "temp service: "<< serviceName.toStdString().c_str() << "\n";
        rpcServer.addService(&ts);
        if (!rpcServer.listen(serviceName)) {
            std::cerr << "could not start server\n: " << rpcServer.errorString().toStdString().c_str() << "\n";
        } else {
            std::cerr << "Server Started\n";
        }
    }

}

WebScriptDialog::~WebScriptDialog()
{
    delete ui;
}

void WebScriptDialog::changeLocation()
{
    //QUrl url = QUrl(ui->lineEdit->text());
    //webview->load(url);
    //webview->setFocus();
}
void WebScriptDialog::adjustLocation()
{
    //ui->lineEdit->setText(webview->url().toString());
}

void WebScriptDialog::setP3service(p3JsonRS *p3servicein)
{
    //webview->setP3service(p3servicein);
    p3service = p3servicein;
    p3service->bridge = bridge;
    bridge->p3service = p3service;
    std::cerr << "bridge on set: " << p3service->bridge << std::endl;
    addTab();
}

void WebScriptDialog::doPython()
{
    //onNewPyTabUrl("");
    embpyqt->execute("embpyqt_console.Visible = True", true);

}
void WebScriptDialog::addTab(){
    onNewTabUrl("html/index.html");
}

void WebScriptDialog::onNewTabUrl(QString url)
{
    WebTabContents * wtc = new WebTabContents(this);
    wtc->getWebView()->setUrl(QUrl(url));
    wtc->setP3service(p3service);
    ui->webTabs->addTab(wtc,QString("hi"));
    connect( wtc->getWebView(), SIGNAL(titleChanged(QString)),    this,   SLOT(onTitleChanged(QString)) );
    connect( wtc->getWebView(), SIGNAL(requestTabForHash(QString)),    this,   SLOT(onNewRsTab(QString)) );
}
void WebScriptDialog::onNewPyTabUrl(QString url)
{
    PyTabContents * ptc = new PyTabContents(this,p3service);
    //wtc->getWebView()->setUrl(QUrl(url));
    //ptc->setP3service(p3service);
    ui->webTabs->addTab(ptc,QString("hi"));
    ptc->embpyqt->execute("embpyqt_console.Visible = True", true);
    //ptc->embpyqt->execute("execfile('myscript.py')", false);
    //connect( wtc->getWebView(), SIGNAL(titleChanged(QString)),    this,   SLOT(onTitleChanged(QString)) );
    //connect( wtc->getWebView(), SIGNAL(requestTabForHash(QString)),    this,   SLOT(onNewRsTab(QString)) );
    //ptc->embpyqt->execute("embpyqt_console.Visible = True", true);
}
void WebScriptDialog::onNewRsTab(QString hash){
    onNewTabUrl("html/loading.html?hash="+hash);
    WebTabContents * wtc = qobject_cast<WebTabContents *>(ui->webTabs->widget(ui->webTabs->count()-1));
    wtc->getWebView()->setHashQue(hash);
    //ui->webTabs->currentIndex();
    //ui->webTabs->currentWidget();
}
void WebScriptDialog::onTitleChanged(QString title){
    WebTabContents * wtc = qobject_cast<WebTabContents *>(QObject::sender()->parent());
    int index = ui->webTabs->indexOf(wtc);
    ui->webTabs->setTabText(index, title);
}

void WebScriptDialog::removeTab()
{
    WebTabContents * wtc = qobject_cast<WebTabContents *>(ui->webTabs->currentWidget());
    ui->webTabs->removeTab(ui->webTabs->currentIndex());
    delete(wtc);
}
