#ifndef WEBBRIDGERS_H
#define WEBBRIDGERS_H

#include <QObject>
#include <QStringList>


class WebBridgeRS : public QObject
{
    Q_OBJECT
public:
    explicit WebBridgeRS(QObject *parent = 0);
    void pushMsgToJs(QStringList message);

signals:
    void msgPush(QStringList message);
public slots:

};

#endif // WEBBRIDGERS_H
