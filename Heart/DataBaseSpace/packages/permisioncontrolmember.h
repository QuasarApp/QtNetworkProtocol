/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef PERMISIONCONTROLMEMBER_H
#define PERMISIONCONTROLMEMBER_H

#include <networkmember.h>

namespace QH {
namespace PKG {

/**
 * @brief The PermisionControlMember class Using unly for change permision for Network node object.
 */
class PermisionControlMember: public NetworkMember
{
public:
    PermisionControlMember();
    PermisionControlMember(const BaseId& id);

    // DBObject interface
public:
    bool isCached() const override;

protected:
    BaseId generateId() const override;

    // DBObject interface
public:
    DBObject *createDBObject() const override;
};

}
}
#endif // PERMISIONCONTROLMEMBER_H
