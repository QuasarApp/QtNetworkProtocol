/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "dbobject.h"
#include "dbtablebase.h"
#include <QDataStream>
#include <QDateTime>
#include <QSqlQuery>
#include <QHash>
#include <QSqlRecord>
#include <QVariantMap>

namespace NP {

DBObject::DBObject(const QString &tableName) {
    clear();
    _tableName = tableName;
}

DBObject::~DBObject() {

}

QString DBObject::tableName() const {
    return _tableName;
}

void DBObject::setTableName(const QString &tableName) {
    _tableName = tableName;
}

bool DBObject::isCached() const {
    return true;
}

DbAddress DBObject::dbAddress() const {
    return {tableName(), getId()};
}

bool DBObject::remove(QSqlQuery &q) const {
    if (_id.isValid()) {
        return false;
    }

    QString queryString = "DELETE FROM %0 where id=" + getId().toBase64();

    queryString = queryString.arg(tableName());

    if (!q.prepare(queryString)) {
        return false;
    }

    return q.exec();
}

QDataStream &DBObject::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);

    stream >> _tableName;
    stream >> _id;
    return stream;
}

QDataStream &DBObject::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << _tableName;
    stream << _id;
    return stream;
}

bool DBObject::isValid() const {
    return AbstractData::isValid() && _tableName.size();
}

bool DBObject::copyFrom(const AbstractData * other) {
    if (!AbstractData::copyFrom(other))
        return false;

    auto otherObject = dynamic_cast<const DBObject*>(other);
    if (!otherObject)
        return false;

    this->_tableName = otherObject->_tableName;
    this->_id = otherObject->_id;

    return true;
}

DbId DBObject::getId() const {
    return _id;
}

void DBObject::setId(const DbId& id) {
    _id = id;
}

void DBObject::clear() {
    _id.clear();
}

}
