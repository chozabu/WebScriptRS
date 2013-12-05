/*
** Copyright (C) 2013 Fargier Sylvain <fargier.sylvain@free.fr>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
** tst_qjsonrpc_custom_types.cpp
**
**        Created on: Nov 06, 2013
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <QtCore/QVariant>
#include <QtTest/QtTest>

#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#else
#include "json/qjsondocument.h"
#endif

#include "qjsonrpcabstractserver.h"
#include "qjsonrpcmessage.h"
#include "qjsonrpcservice.h"
#include "qjsonrpcconverter.h"

class TestQJsonRpcCustomTypes: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testCustomParams();
    void testCustomRet();
    void testInvalidParams();
};

class CustomClass : public QObject
{
public:
    CustomClass(int data = 0, QObject *parent = 0) :
        QObject(parent),
        data(data)
    {}

    CustomClass(const CustomClass &other) :
        QObject(),
        data(other.data)
    {}

    CustomClass &operator = (const CustomClass &other) {
        if (&other != this)
        {
            data = other.data;
        }
        return *this;
    }

    int data;
};

QJsonValue &operator << (QJsonValue &json, const CustomClass &value)
{
    QJsonObject obj;
    obj.insert("data", value.data);
    json = obj;

    return json;
}

QJsonValue &operator >> (QJsonValue &json, CustomClass &value)
{
    if (!json.isObject())
        json = QJsonValue(QJsonValue::Undefined);
    else
    {
        QJsonObject obj(json.toObject());

        QJsonObject::const_iterator it = obj.find("data");
        if (it == obj.end())
            json = QJsonValue(QJsonValue::Undefined);
        else
            value.data = (int) it.value().toDouble();
    }
    return json;
}

Q_DECLARE_METATYPE(CustomClass);

class UnboundClass : public QObject
{
public:
    UnboundClass(QObject *parent = 0) :
        QObject(parent)
    {}
};

class TestService : public QJsonRpcService
{
    Q_OBJECT
    Q_CLASSINFO("serviceName", "service")
public:
    TestService(QObject *parent = 0)
        : QJsonRpcService(parent)
    {}

    bool testDispatch(const QJsonRpcMessage &message) {
        return QJsonRpcService::dispatch(message);
    }

public Q_SLOTS:
    void testCustomParams(const CustomClass &param) const {
        QCOMPARE(param.data, 42);
    }

    CustomClass testCustomRet(const CustomClass &param) const {
        CustomClass ret(param);
        ++ret.data;

        return ret;
    }

    /*
     * QJsonRpc should be unable to bind this method.
     */
    void testInvalidParams(const UnboundClass &) const {
    }
};

class TestServiceProvider : public QObject, public QJsonRpcServiceProvider
{
    Q_OBJECT
public:
    TestServiceProvider() {}

    bool addService(QJsonRpcService *srv) {
        if (QJsonRpcServiceProvider::addService(srv)) {
            connect(srv, SIGNAL(result(QJsonRpcMessage)),
                    this, SLOT(reply(QJsonRpcMessage)), Qt::DirectConnection);
            return true;
        }
        else
            return false;

    }

    QJsonRpcMessage last;

protected Q_SLOTS:
    void reply(const QJsonRpcMessage &msg) {
        last = msg;
    }
};

void TestQJsonRpcCustomTypes::initTestCase()
{
    qRegisterJsonRpcOperators<CustomClass>("CustomClass");
}

/*
 * Custom class parameter
 */
void TestQJsonRpcCustomTypes::testCustomParams()
{
    TestServiceProvider provider;
    TestService service;
    provider.addService(&service);
    CustomClass custom(42);

    QJsonRpcMessage request =
        QJsonRpcMessage::createRequest("service.testCustomParams",
                QVariant::fromValue(custom));
    QVERIFY(service.testDispatch(request));
}

/*
 * Custom return type class
 */
void TestQJsonRpcCustomTypes::testCustomRet()
{
    TestServiceProvider provider;
    TestService service;
    provider.addService(&service);

    QJsonRpcMessage request =
        QJsonRpcMessage::createRequest("service.testCustomRet",
                QVariant::fromValue(CustomClass()));
    QVERIFY(service.testDispatch(request));

    QCOMPARE(provider.last.type(), QJsonRpcMessage::Response);
    QVariant result = provider.last.result(QMetaType::type("CustomClass"));

    QCOMPARE(QString(QMetaType::typeName(result.userType())),
            QString("CustomClass"));
    QCOMPARE(result.value<CustomClass>().data, 1);
}

/*
 * Methods with unconvertible parameters shouldn't be bound.
 */
void TestQJsonRpcCustomTypes::testInvalidParams()
{
    TestServiceProvider provider;
    TestService service;
    provider.addService(&service);
    UnboundClass custom;

    QJsonRpcMessage request =
        QJsonRpcMessage::createRequest("service.testInvalidParams",
                QVariant::fromValue(CustomClass()));
    QVERIFY(!service.testDispatch(request));

    QCOMPARE(provider.last.type(), QJsonRpcMessage::Error);
    QCOMPARE(provider.last.errorCode(), (int) QJsonRpc::MethodNotFound);
}

QTEST_MAIN(TestQJsonRpcCustomTypes)
#include "tst_qjsonrpc_custom_types.moc"

