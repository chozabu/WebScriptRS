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
** qjsonrpcconverter.cpp
**
**        Created on: Nov 06, 2013
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <QHash>
#include <QMetaType>
#include <QDebug>

#include "qjsonrpcconverter.h"

namespace QJsonRpcConverter {

struct Info
{
    ToJsonOperator toJson;
    FromJsonOperator fromJson;
};

typedef QHash<int, Info> InfoHash;

Q_GLOBAL_STATIC(InfoHash, convInfo)

void registerStreamOperators(
        const char *typeName,
        ToJsonOperator toJson,
        FromJsonOperator fromJson)
{
    int idx = QMetaType::type(typeName);
    if (!idx)
        return;

    Info &info = (*convInfo())[idx];

    info.toJson = toJson;
    info.fromJson = fromJson;
}

void toJson(QJsonValue &value, int type, const void *data)
{
    InfoHash::const_iterator it = convInfo()->find(type);

    if (it != convInfo()->end())
        it->toJson(value, data);
    else
        value = QJsonValue(QJsonValue::Undefined);
}

void fromJson(QJsonValue &value, int type, void *data)
{
    InfoHash::const_iterator it = convInfo()->find(type);

    if (it != convInfo()->end())
        it->fromJson(value, data);
    else
        value = QJsonValue(QJsonValue::Undefined);
}

}

