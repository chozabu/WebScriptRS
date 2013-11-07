#ifndef WEBVIEWRS_H
#define WEBVIEWRS_H

#include <QWebView>
#include <QVariantList>
#include <QStringList>
#include "p3JsonRS.h"

class WebViewRS : public QWebView
{
    Q_OBJECT
public:
    explicit WebViewRS(QWidget *parent = 0);
    ~WebViewRS();
    void setP3service(p3JsonRS *p3servicein);
    p3JsonRS *p3service;

signals:

public slots:
    void onJavaScriptWindowObjectCleared();
    void onLinkClicked(const QUrl & url);



    void go2page(QString url);
    void onChange();

};

#endif // WEBVIEWRS_H
