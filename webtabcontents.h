#ifndef WEBTABCONTENTS_H
#define WEBTABCONTENTS_H

#include <QWidget>
#include "webviewrs.h"

namespace Ui {
class WebTabContents;
}

class WebTabContents : public QWidget
{
    Q_OBJECT

public:
    explicit WebTabContents(QWidget *parent = 0);
    ~WebTabContents();
    void setP3service(p3JsonRS *p3servicein);
    WebViewRS *getWebView();
//public signals:
//    void titleChanged(QString title, WebTabContents call);

protected slots:
    void adjustLocation();
    void changeLocation();

protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);
private:
    Ui::WebTabContents *ui;
    WebViewRS* webview;
};

#endif // WEBTABCONTENTS_H
