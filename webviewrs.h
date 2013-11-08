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

public slots:
    void onJavaScriptWindowObjectCleared();
    void onLinkClicked(const QUrl & url);
    void onDownloadComplete(QString hash);
    void loadRSFile(QString qname, QString qhash, int qsize);



    void go2page(QString url);
    void onChange();
private:
    QString hashque;

};

#endif // WEBVIEWRS_H
