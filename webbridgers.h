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

signals:
    void msgPush(QVariantMap message);
public slots:

};

#endif // WEBBRIDGERS_H
