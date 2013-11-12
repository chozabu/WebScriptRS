#ifndef WEBBRIDGERS_H
#define WEBBRIDGERS_H

#include <QObject>
#include <QStringList>
#include<QVariantMap>
#include <QPixmap>

class p3JsonRS;

/*this class is passed to JS as "bridge"*/
class WebBridgeRS : public QObject
{
    Q_OBJECT
public:
    explicit WebBridgeRS(QObject *parent = 0);
    void pushMsgToJs(QVariantMap message);
    p3JsonRS *p3service;
    QMap<QString,QPixmap> avatars;

//the public slots are exposed to JS
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
    //QVariantList getPublicChatQueue();

    QVariantMap fileDetails(QString qhash);
    void newTabAt(QString url);
#ifdef BRIDGEGXS
    QVariantList getPostedList();
#endif
    QStringList getFriendList();
    QStringList getOnlineList();
    QVariantMap getPeerDetails(QString ssl_id);
    QString getPeerName(QString ssl_id);
    QPixmap getAvatarDetails(QString ssl_id);
private slots:
    void onUrlDownloaded();
    void onDownloadComplete(QString hash);
signals:
    void msgPush(QVariantMap message);
    void urlDownloaded(QString path, QString url);
    void rsDownloaded(QVariantMap);
    void newTabUrl(QString url);

};

#endif // WEBBRIDGERS_H
