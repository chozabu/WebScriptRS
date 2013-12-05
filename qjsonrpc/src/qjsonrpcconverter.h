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
** qjsonrpcconv.hpp
**
**        Created on: Nov 06, 2013
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __QJSONRPCCONV_HPP__
#define __QJSONRPCCONV_HPP__

#include <QMetaType>

#if QT_VERSION >= 0x050000
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#else
#include "json/qjsonvalue.h"
#include "json/qjsonobject.h"
#include "json/qjsonarray.h"
#endif

#include "qjsonrpc_export.h"

namespace QJsonRpcConverter {

typedef void (*ToJsonOperator)(QJsonValue &, const void *ptr);
typedef void (*FromJsonOperator)(QJsonValue &, void *ptr);

void QJSONRPC_EXPORT registerStreamOperators(
        const char * typeName,
        ToJsonOperator toJson,
        FromJsonOperator fromJson);

void toJson(QJsonValue &value, int type, const void *data);
void fromJson(QJsonValue &value, int type, void *data);

}

template <typename T>
void qJsonRpcToJsonHelper(QJsonValue &value, const T *t)
{
    value << *t;
}

template <typename T>
void qJsonRpcFromJsonHelper(QJsonValue &value, T *t)
{
    value >> *t;
}

template <typename T>
void qRegisterJsonRpcOperators(
        const char *typeName,
        T * = 0)
{
    typedef void (*ToJsonPtr)(QJsonValue &, const T *);
    typedef void (*FromJsonPtr)(QJsonValue &, T *);
    ToJsonPtr sptr = qJsonRpcToJsonHelper<T>;
    FromJsonPtr lptr = qJsonRpcFromJsonHelper<T>;

    qRegisterMetaType<T>(typeName);
    QJsonRpcConverter::registerStreamOperators(typeName,
            reinterpret_cast<QJsonRpcConverter::ToJsonOperator>(sptr),
            reinterpret_cast<QJsonRpcConverter::FromJsonOperator>(lptr));
}

#endif

