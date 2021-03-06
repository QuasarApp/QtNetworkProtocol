/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "basenodeinfo.h"
#include "dbaddress.h"
#include <QTcpSocket>
#include <hostaddress.h>

namespace QH {


BaseNodeInfo::BaseNodeInfo(QAbstractSocket *tcp, const HostAddress* address):
    AbstractNodeInfo(tcp, address){}

BaseNodeInfo::~BaseNodeInfo() = default;

bool BaseNodeInfo::isValid() const {
    return AbstractNodeInfo::isValid();
}

const AccessToken &BaseNodeInfo::token() const {
    return _token;
}

void BaseNodeInfo::setToken(const AccessToken &token) {
    _token = token;
}

void BaseNodeInfo::reset() {
    AbstractNodeInfo::reset();
    setToken(AccessToken{});
    setId({});
}

bool BaseNodeInfo::confirmData() const {
    return AbstractNodeInfo::confirmData();
}

const QVariant& BaseNodeInfo::id() const {
    return _id;
}

void BaseNodeInfo::setId(const QVariant &id)
{
    _id = id;
}

}
