/*
 * Copyright (C) 2012-2013 Matt Broadstone
 * Contact: http://bitbucket.org/devonit/qjsonrpc
 *
 * This file is part of the QJsonRpc Library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#include <QVarLengthArray>
#include <QMetaMethod>
#include <QEventLoop>
#include <QDebug>

#include "qjsonrpcsocket.h"
#include "qjsonrpcservice_p.h"
#include "qjsonrpcservice.h"
#include "qjsonrpcconverter.h"

QJsonRpcService::QJsonRpcService(QObject *parent)
    : QObject(parent),
      d_ptr(new QJsonRpcServicePrivate(this))
{
}

QJsonRpcService::~QJsonRpcService()
{
}

QJsonRpcSocket *QJsonRpcService::senderSocket()
{
    Q_D(QJsonRpcService);
    if (d->socket)
        return d->socket.data();
    return 0;
}

static int convertVariantTypeToJSType(int type)
{
    switch (type) {
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::Double:
    case QMetaType::Long:
    case QMetaType::LongLong:
    case QMetaType::Short:
    case QMetaType::Char:
    case QMetaType::ULong:
    case QMetaType::ULongLong:
    case QMetaType::UShort:
    case QMetaType::UChar:
    case QMetaType::Float:
        return QJsonValue::Double;    // all numeric types in js are doubles
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        return QJsonValue::Array;
    case QMetaType::QVariantMap:
        return QJsonValue::Object;
    case QMetaType::QString:
        return QJsonValue::String;
    case QMetaType::Bool:
        return QJsonValue::Bool;
    default:
        break;
    }

    return QJsonValue::Undefined;
}

static int qJsonNameToTypeId(const char *name)
{
    int id = static_cast<int>(QVariant::nameToType(name));
    if (id == QVariant::UserType || id == QVariant::Invalid)
        id = QMetaType::type(name);
    return id;
}

int QJsonRpcServicePrivate::qjsonRpcMessageType = qRegisterMetaType<QJsonRpcMessage>("QJsonRpcMessage");
void QJsonRpcServicePrivate::cacheInvokableInfo()
{
    Q_Q(QJsonRpcService);
    const QMetaObject *obj = q->metaObject();
    int startIdx = q->staticMetaObject.methodCount(); // skip QObject slots
    for (int idx = startIdx; idx < obj->methodCount(); ++idx) {
        const QMetaMethod method = obj->method(idx);
        if ((method.methodType() == QMetaMethod::Slot &&
             method.access() == QMetaMethod::Public) ||
             method.methodType() == QMetaMethod::Signal) {
#if QT_VERSION >= 0x050000
            QByteArray methodName = method.name();
#else
            QByteArray signature = method.signature();
            QByteArray methodName = signature.left(signature.indexOf('('));
#endif

            QList<int> parameterTypes;
            QList<int> jsParameterTypes;
            int type;
#if QT_VERSION >= 0x050000
            if ((type = method.returnType()) == QMetaType::UnknownType)
            {
                qWarning() << "QJsonRpcService: can't bind method's return type"
                    << QString(methodName);
                continue;
            }
#else
            if (qstrlen(method.typeName()) == 0) {
                /* typeName returns an empty string for void returns */
                type = 0;
            } else if ((type = qJsonNameToTypeId(method.typeName())) == 0) {
                qWarning() << "QJsonRpcService: can't bind method's return type"
                    << QString(methodName);
                continue;
            }
#endif
            parameterTypes << type;

            foreach(QByteArray parameterType, method.parameterTypes()) {
                type = QMetaType::type(parameterType);

                if (type == 0)
                {
                    qWarning() << "QJsonRpcService: can't bind method's parameter"
                        << QString(parameterType);
                    methodName.clear();
                    break;
                }
                parameterTypes << type;
                jsParameterTypes << convertVariantTypeToJSType(type);
            }
            if (methodName.isEmpty())
                continue;

            invokableMethodHash.insert(methodName, idx);
            parameterTypeHash[idx] = parameterTypes;
            jsParameterTypeHash[idx] = jsParameterTypes;
        }
    }
}

static bool variantAwareCompare(const QJsonArray &params, const QList<int> &jsParameterTypes)
{
    if (params.size() != jsParameterTypes.size())
        return false;

    QJsonArray::const_iterator it = params.constBegin();
    for (int i = 0; it != params.end(); ++it, ++i) {
        int jsType = jsParameterTypes.at(i);

        if ((*it).type() == jsType)
            continue;
        else if (jsType == QJsonValue::Undefined)
            continue;
        else
            return false;
    }

    return true;
}

//QJsonRpcMessage QJsonRpcService::dispatch(const QJsonRpcMessage &request) const
bool QJsonRpcService::dispatch(const QJsonRpcMessage &request)
{
    Q_D(QJsonRpcService);
    if (request.type() != QJsonRpcMessage::Request &&
        request.type() != QJsonRpcMessage::Notification) {
        QJsonRpcMessage error =
            request.createErrorResponse(QJsonRpc::InvalidRequest, "invalid request");
        Q_EMIT result(error);
        return false;
    }

    QByteArray method = request.method().section(".", -1).toLatin1();
    if (!d->invokableMethodHash.contains(method)) {
        QJsonRpcMessage error =
            request.createErrorResponse(QJsonRpc::MethodNotFound, "invalid method called");
        Q_EMIT result(error);
        return false;
    }

    int idx = -1;
    QList<int> parameterTypes;
    QList<int> indexes = d->invokableMethodHash.values(method);
    QJsonArray arguments(request.rawParams());

    foreach (int methodIndex, indexes) {
        if (variantAwareCompare(arguments, d->jsParameterTypeHash.value(methodIndex))) {
            parameterTypes = d->parameterTypeHash.value(methodIndex);
            idx = methodIndex;
            break;
        }
    }

    if (idx == -1) {
        QJsonRpcMessage error =
            request.createErrorResponse(QJsonRpc::InvalidParams, "invalid parameters");
        Q_EMIT result(error);
        return false;
    }

    QVarLengthArray<void *, 10> parameters;
    parameters.reserve(parameterTypes.count());
    QVector<QVariant> vars;
    vars.reserve(parameterTypes.count());

    // first argument to metacall is the return value
    vars.append(QVariant(parameterTypes.value(0), (const void *) NULL));
    if (parameterTypes.value(0) == QMetaType::QVariant)
        parameters.append(&vars.last());
    else
        parameters.append(vars.last().data());

    // compile arguments
    for (int i = 0; i < parameterTypes.size() - 1; ++i) {
        int parameterType = parameterTypes.value(i + 1);

        if (parameterType >= QMetaType::User)
        {
            QJsonValue val = arguments.at(i);

            vars.append(QVariant(parameterType, (const void *) NULL));
            QJsonRpcConverter::fromJson(val, parameterType, vars.last().data());
        }
        else
        {
            vars.append(arguments.at(i).toVariant());
            QVariant &arg = vars.last();
            if (!arg.isValid()) {
                arg = QVariant(parameterType, (const void *) NULL);
            } else if (arg.userType() != parameterType &&
                    parameterType != QMetaType::QVariant &&
                    arg.canConvert(static_cast<QVariant::Type>(parameterType))) {
                arg.convert(static_cast<QVariant::Type>(parameterType));
            }
        }
        parameters.append(const_cast<void *>(vars.last().constData()));
    }

    bool success =
        const_cast<QJsonRpcService*>(this)->qt_metacall(QMetaObject::InvokeMetaMethod, idx, parameters.data()) < 0;
    if (!success) {
        QString message = QString("dispatch for method '%1' failed").arg(method.constData());
        QJsonRpcMessage error =
            request.createErrorResponse(QJsonRpc::InvalidRequest, message);
        Q_EMIT result(error);
        return false;
    }

    // cleanup and result
    QVariant returnCopy(vars[0]);
    Q_EMIT result(request.createResponse(returnCopy));
    return true;
}

