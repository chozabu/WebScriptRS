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
    void setP3service(p3JsonRS *p3servicein);
    p3JsonRS *p3service;

signals:

public slots:
    void onJavaScriptWindowObjectCleared();
    //QString getMessage(QString in);
    void broadcastMessage(QString msg);
    void sendMessage(QString peer, QString msg);
    QVariantList getMessages();
    QMapIterator<QString, QString> getPeers();


    void go2index2();
    void go2page(QString url);
    QString echothis(QString in);
    void onChange();

};

#endif // WEBVIEWRS_H
