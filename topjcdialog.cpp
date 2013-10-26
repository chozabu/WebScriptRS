#include "topjcdialog.h"
#include "ui_topjcdialog.h"

#include <QtGui>
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

    webview = new WebViewRS();
    this->ui->verticalLayout->addWidget(webview,1);
    webview->show();
    QString loadfirst = "";//QDir::homePath();

    loadfirst.append("tindex.html");
    webview->setUrl(QUrl(loadfirst));


    jslog = new QTextBrowser();
    this->ui->verticalLayout->addWidget(jslog,1);
    jslog->show();
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

}

WebScriptDialog::~WebScriptDialog()
{
    delete ui;
}

void WebScriptDialog::setP3service(p3JsonRS *p3servicein)
{
    webview->setP3service(p3servicein);
    p3service = p3servicein;
}
