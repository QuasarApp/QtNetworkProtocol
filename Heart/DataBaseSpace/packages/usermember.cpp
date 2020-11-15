#include "usermember.h"

#include <QDataStream>
namespace QH {
namespace PKG {

UserMember::UserMember() {
    setTrust(100);
}

UserMember::UserMember(const Package &pkg):
    UserMember() {
    fromBytes(pkg.data);
}

UserMember::UserMember(const QVariant &id):
    UserMember() {
    setId(id);
}

bool UserMember::copyFrom(const AbstractData *other) {
    if (!NetworkMember::copyFrom(other))
        return false;

    auto otherObject = dynamic_cast<const UserMember*>(other);
    if (!otherObject)
        return false;

    this->_token = otherObject->_token;
    this->_name = otherObject->_name;

    return true;
}

bool UserMember::fromSqlRecord(const QSqlRecord &q) {
    if (!NetworkMember::fromSqlRecord(q)) {
        return false;
    }

    setToken(AccessToken(q.value("token").toByteArray()));
    setName(q.value("userName").toString());

    return UserMember::isValid();
}

DBObject *UserMember::createDBObject() const {
    return create<UserMember>();
}

bool UserMember::isValid() const {
    return NetworkMember::isValid() && trust() <= 100 && _name.size();
}

QDataStream &UserMember::fromStream(QDataStream &stream) {
    NetworkMember::fromStream(stream);
    stream >> _token;
    stream >> _name;

    return stream;
}

QDataStream &UserMember::toStream(QDataStream &stream) const {
    NetworkMember::toStream(stream);
    stream << _token;
    stream << _name;

    return stream;
}

QString UserMember::name() const {
    return _name;
}

void UserMember::setName(const QString &name) {
    _name = name;
}

DBVariantMap UserMember::variantMap() const {
    auto map = NetworkMember::variantMap();
    map.insert("token",     {_token.toBytes(),  MemberType::InsertUpdate});
    map.insert("userName",  {_name,             MemberType::InsertUpdate | MemberType::Unique});

    return map;
}

AccessToken UserMember::token() const {
    return _token;
}

void UserMember::setToken(const AccessToken &token) {
    _token = token;
}
}
}