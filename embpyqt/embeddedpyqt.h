#ifndef EMBEDDEDPYQT_H
#define EMBEDDEDPYQT_H

#include <QObject>
#include <QMetaObject>
#include <QMap>
#include <QStringList>
#include "pythonize.h"

#define RAISE(type, msg) EmbeddedPyQt::instance()->raiseException(type, msg);
#define REGISTER_CLASS(cls) EmbeddedPyQt::instance()->registerMetaObject(cls::staticMetaObject);
#define REGISTER_INSTANCE(obj) EmbeddedPyQt::instance()->registerObject(obj);


class EmbeddedPyQt : public QObject
{
    Q_OBJECT

public:
    explicit EmbeddedPyQt(QObject *parent = 0);
    void init(const QString &filename);
    static EmbeddedPyQt *instance();
signals:
    void executionRequested(const QString &command, bool globalContext);

public slots:
    void deleteLater();
    void raiseException(const QString &type, const QString &message);
    void clearException();
    void* metaObjectByName(const QString &name);
    QObject* objectByName(const QString &name);
    QStringList availableClasses();
    QStringList availableObjects();
    void registerMetaObject(const QMetaObject &mo);
    void registerObject(const QObject &obj);
    void execute(const QString &command, bool globalContext);

private:
    static EmbeddedPyQt *_instance;
    Pythonize *python;
    QMap<QString,const QMetaObject*> classes;
    QMap<QString,const QObject*> objects;
};


#endif // EMBEDDEDPYQT_H
