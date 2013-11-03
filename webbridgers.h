#ifndef WEBBRIDGERS_H
#define WEBBRIDGERS_H

#include <QObject>
#include <QStringList>
#include<QVariantMap>


class WebBridgeRS : public QObject
{
    Q_OBJECT
public:
    explicit WebBridgeRS(QObject *parent = 0);
    void pushMsgToJs(QVariantMap message);

public slots:
    QString getOwnId();
    QString getDownloadDirectory();
    QVariantList searchKeywords(const QString& keywords, QVariantMap searchOptions);
signals:
    void msgPush(QVariantMap message);

    /*
    rsPeers->getOwnId();
    rsPeers->getOnlineList();
    rsPeers->getFriendList();
    rsFiles->FileDetails();
    rsFiles->SearchKeywords();
    */
public slots:

};

#endif // WEBBRIDGERS_H
