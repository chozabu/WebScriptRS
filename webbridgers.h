#ifndef WEBBRIDGERS_H
#define WEBBRIDGERS_H

#include <QObject>
#include <QStringList>
#include<QVariantMap>

class p3JsonRS;

class WebBridgeRS : public QObject
{
    Q_OBJECT
public:
    explicit WebBridgeRS(QObject *parent = 0);
    void pushMsgToJs(QVariantMap message);
    p3JsonRS *p3service;

public slots:
    QVariantList getChannelList();
    QVariantList getForumInfo(QString flid);
    QVariantList getForums();
    QString getOwnId();
    QString getDownloadDirectory();
    QVariantList searchKeywords(const QString& keywords, QVariantMap searchOptions);
    //QString processLink(QString url);
    void processLinks(QStringList urls);

    void broadcastMessage(QString msg);
    void sendMessage(QString peer, QString msg);
    QVariantMap getPeers();
    QVariantMap downloadFile(QString qname, QString qhash, int qsize);
    void downloadURL(QString url);
    void broadcastToRS(QString msg);
    QVariantList getPublicChatQueue();
private slots:
    void onUrlDownloaded();
    void onDownloadComplete(QString hash);
signals:
    void msgPush(QVariantMap message);
    void urlDownloaded(QString path, QString url);
    void rsDownloaded(QVariantMap);

    /*
    rsPeers->getOwnId();
    rsPeers->getOnlineList();
    rsPeers->getFriendList();
    rsFiles->FileDetails();
    rsFiles->SearchKeywords();
    */

};

#endif // WEBBRIDGERS_H
