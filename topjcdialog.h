#ifndef TOPJCDIALOG_H
#define TOPJCDIALOG_H

#include <retroshare-gui/mainpage.h>
#include <retroshare-gui/RsAutoUpdatePage.h>
#include <retroshare/rsfiles.h>
#include <retroshare/rspeers.h>

#include <QWidget>
#include <QtWebKit>
#include <QTextBrowser>

#include "p3JsonRS.h"
#include "webviewrs.h"

#include "qjsonrpclocalserver.h"

#include "embpyqt/embeddedpyqt.h"

namespace Ui {
class WebScriptDialog;
}

class WebScriptDialog  : public MainPage
{
    Q_OBJECT
    
public:
    explicit WebScriptDialog(QWidget *parent = 0);
    ~WebScriptDialog();
    void setP3service(p3JsonRS *p3servicein);
    p3JsonRS *p3service;
    WebBridgeRS *bridge;
    EmbeddedPyQt *embpyqt;
public slots:
    void doPython();
    void addTab();
    void onNewTabUrl(QString url);
    void removeTab();
    void onTitleChanged(QString title);
    void onNewRsTab(QString hash);

    void onNewPyTabUrl(QString url);
    void startRPC();
protected slots:
    void adjustLocation();
    void changeLocation();

protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);
private:
    Ui::WebScriptDialog *ui;
    QJsonRpcLocalServer rpcServer;
};

#endif // TOPJCDIALOG_H
