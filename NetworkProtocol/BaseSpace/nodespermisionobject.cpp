/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "nodespermisionobject.h"

#include <QDataStream>
#include <QSqlQuery>


namespace NP {

NodesPermisionObject::NodesPermisionObject():
    DBObject("NodesPermisions") {}

NodesPermisionObject::NodesPermisionObject(const Package &pkg):
    NodesPermisionObject() {
    fromBytes(pkg.toBytes());

}

NodesPermisionObject::NodesPermisionObject(const PermisionData &id):
    NodesPermisionObject()  {
    setKey(id);
}

bool NodesPermisionObject::isValid() const {
    return _key.isValid();
}

bool NodesPermisionObject::copyFrom(const AbstractData *other) {
    if (!DBObject::copyFrom(other))
        return false;

    auto otherObject = dynamic_cast<const NodesPermisionObject*>(other);
    if (!otherObject)
        return false;

    this->_key = otherObject->_key;
    this->_permisions = otherObject->_permisions;

    return true;
}

bool NodesPermisionObject::prepareSaveQuery(QSqlQuery &q) const {

    if (!isValid()) {
        return false;
    }

    QString queryString = "INSERT INTO %0(%1) VALUES (%2)";

    queryString = queryString.arg(tableName());

    queryString = queryString.arg(
                "nodeId, objectTable", "objectId", "lvl");

    QString values;

    values += "'" + _key.id().toBase64() + "', ";
    values += "'" + _key.address().table() + "', ";
    values += "'" + _key.address().id().toBase64() + "', ";
    values += QString::number(static_cast<int>(_permisions));

    queryString = queryString.arg(values);

    return q.prepare(queryString);
}

bool NodesPermisionObject::prepareRemoveQuery(QSqlQuery &q) const {
    if (!isValid()) {
        return false;
    }

    QString queryString = "DELETE FROM %0 where nodeId='%1' and objectTable='%2' and objectId='%3'";
    queryString = queryString.arg(tableName(), _key.id().toBase64(), _key.address().table(), _key.address().id().toBase64());

    return q.prepare(queryString);
}

bool NodesPermisionObject::prepareSelectQuery(QSqlQuery &q) const {
    if (_key.isValid()) {
        return false;
    }

    QString queryString = "SELECT * FROM %0 WHERE";
    bool fOptionAdded = false;

    if (_key.id().isValid()) {
        queryString += "nodeId='" + _key.id().toBase64() + "'";
        fOptionAdded = true;
    }

    if (_key.address().table().size()) {
        if (fOptionAdded)
            queryString += " and ";
        queryString += "objectTable='" + _key.address().table() + "'";
        fOptionAdded = true;
    }

    if (_key.address().id().isValid()) {
        if (fOptionAdded)
            queryString += " and ";
        queryString += "objectId='" + _key.address().id().toBase64() + "'";
        fOptionAdded = true;
    }

    queryString = queryString.arg(tableName());

    return q.prepare(queryString);
}

DBObject *NodesPermisionObject::factory() const {
    return new NodesPermisionObject();
}

DBCacheKey NodesPermisionObject::dbKey() const {
    return {&_key, true};
}

QDataStream &NodesPermisionObject::fromStream(QDataStream &stream) {

    stream >> _key;
    stream >> _permisions;

    return  stream;
}

QDataStream &NodesPermisionObject::toStream(QDataStream &stream) const {
    stream << _key;
    stream << _permisions;

    return  stream;
}

PermisionData NodesPermisionObject::key() const {
    return _key;
}

void NodesPermisionObject::setKey(const PermisionData &key) {
    _key = key;
}

Permission NodesPermisionObject::permisions() const {
    return _permisions;
}

void NodesPermisionObject::setPermisions(const Permission &permisions) {
    _permisions = permisions;
}
}