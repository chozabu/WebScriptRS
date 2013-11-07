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
public slots:
    void addTab();
    void removeTab();
    void onTitleChanged(QString title);

protected slots:
    void adjustLocation();
    void changeLocation();

protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);
private:
    Ui::WebScriptDialog *ui;
    //WebViewRS* webview;
    //QTextBrowser* jslog;
};

#endif // TOPJCDIALOG_H
