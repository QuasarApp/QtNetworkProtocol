#include "dbobject.h"
#include "dbtablebase.h"
#include <QDataStream>
#include <QDateTime>
#include <QSqlQuery>
#include <QHash>
#include <QSqlRecord>
#include <QVariantMap>

namespace NetworkProtocol {

DBObject::DBObject(const QString &tableName) {
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

int DBObject::getId() const {
    return _id;
}

void DBObject::setId(int id) {
    _id = id;
}

void DBObject::clear() {
    _id = -1;
}


}
