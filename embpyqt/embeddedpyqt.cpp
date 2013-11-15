#include "embeddedpyqt.h"
#include "pythonize.h"
#include <QApplication>
#include <QVariant>
#include <QPushButton>
#include <qdebug.h>


EmbeddedPyQt *EmbeddedPyQt::_instance = 0;


EmbeddedPyQt::EmbeddedPyQt(QObject *parent) :
    QObject(parent)
{
    EmbeddedPyQt::_instance = this;
    setObjectName("embeddedPyQt");
    QApplication::instance()->setProperty("embedded_pyqt",
                                          qVariantFromValue((QObject*)this));
    python = new Pythonize();
    Q_ASSERT(python);
}


void EmbeddedPyQt::init(const QString &filename) {
    char *cfn = (char*)filename.toLocal8Bit().constData();
    python->runScript(cfn);
    //python->appendToSysPath("");
    //python->runString("import embeddedpyqt");
}


void EmbeddedPyQt::deleteLater() {
    Q_ASSERT(python);
    QApplication::instance()->setProperty("embedded_pyqt",
                                          qVariantFromValue(NULL));
    delete(python);
}


void EmbeddedPyQt::raiseException(const QString &type, const QString &message) {
    QApplication::instance()->setProperty("embedded_pyqt_exception_type",
                                         qVariantFromValue(type));
    QApplication::instance()->setProperty("embedded_pyqt_exception_msg",
                                         qVariantFromValue(message));
    const char *cmsg = message.toLocal8Bit().constData();
    python->raiseException(cmsg);
}


void EmbeddedPyQt::clearException() {
    raiseException(NULL, NULL);
}

void* EmbeddedPyQt::metaObjectByName(const QString &name) {
    return (void*)classes[name];
}

QObject* EmbeddedPyQt::objectByName(const QString &name) {
    return (QObject*)objects[name];
}


QStringList EmbeddedPyQt::availableClasses() {
    return classes.keys();
}


QStringList EmbeddedPyQt::availableObjects() {
    return objects.keys();
}


void EmbeddedPyQt::registerMetaObject(const QMetaObject &mo) {
    classes[mo.className()] = &mo;
}


void EmbeddedPyQt::registerObject(const QObject &obj) {
    //connect(obj, SIGNAL(destroyed()), unregister_objec);
    objects[obj.objectName()] = &obj;
}


void EmbeddedPyQt::execute(const QString &command, bool globalContext) {
    emit executionRequested(command, globalContext);
}
