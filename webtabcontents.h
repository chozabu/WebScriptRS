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
    WebViewRS *getWebView();

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
