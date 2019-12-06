#include "userdata.h"

#include <QDataStream>
#include <QSharedPointer>
#include <QSqlQuery>

namespace NP {

UserData::UserData():
    DBObject("users") {
    clear();
    INIT_COMMAND

}

UserData::UserData(const Package &package): UserData() {
    fromBytes(package.data);
}

UserData::~UserData() {

}

QString UserData::name() const {
    return _name;
}

void UserData::setName(const QString &name) {
    _name = name;
}

QString UserData::passSHA256() const {
    return _passSHA256;
}

void UserData::setPassSHA256(const QString &passSHA256) {
    _passSHA256 = passSHA256;
}

QString UserData::mail() const {
    return _mail;
}

void UserData::setMail(const QString &mail) {
    _mail = mail;
}

int UserData::lastOnline() const {
    return _lastOnline;
}

void UserData::setLastOnline(int lastOnline) {
    _lastOnline = lastOnline;
}

int UserData::onlineTime() const {
    return _onlineTime;
}

void UserData::setOnlineTime(int onlineTime) {
    _onlineTime = onlineTime;
}

QVariantMap UserData::extraData() const {
    return _extraData;
}

QVariantMap &UserData::extraData() {
    return _extraData;
}

void UserData::setExtraData(const QVariantMap &extraData) {
    _extraData = extraData;
}

QDataStream &UserData::fromStream(QDataStream &stream) {
    DBObject::fromStream(stream);

    stream >> _name;
    stream >> _passSHA256;
    stream >> _mail;
    stream >> _lastOnline;
    stream >> _onlineTime;
    stream >> _extraData;
    stream >> _token;
    stream >> _points;

    return stream;
}

QDataStream &UserData::toStream(QDataStream &stream) const {
    DBObject::toStream(stream);

    stream << _name;
    stream << _passSHA256;
    stream << _mail;
    stream << _lastOnline;
    stream << _onlineTime;
    stream << _extraData;
    stream << _token;
    stream << _points;

    return stream;
}

bool UserData::select(QSqlQuery &q) {
    QString query;

    if (getId() > 0) {
        query = "SELECT * from '" + tableName() +
        "' where id='" + QString::number(getId()) + "'";
    } else {
        query = "SELECT * from '" + tableName() +
        "' where gmail='" + QString::number(getId()) + "'";
    }

    if (!q.prepare(query))
        return false;

    if (!q.exec())
        return false;

    if (!q.next())
        return false;

    setId(q.value("id").toInt());
    _name = q.value("name").toString();
    _passSHA256 = q.value("pass").toString();
    _mail = q.value("gmail").toString();
    _lastOnline = q.value("lastOnline").toInt();
    _onlineTime = q.value("onlinetime").toInt();
    _points = q.value("points").toInt();

    auto array = q.value("data").toByteArray();
    QDataStream s(&array, QIODevice::ReadWrite);
    s >> _extraData;

    return isValid();

}

bool UserData::save(QSqlQuery &q) {

    QString queryString = "INSERT INTO %0(%1) VALUES (%2)";

    queryString = queryString.arg(tableName());

    queryString = queryString.arg(
                "name, pass, gmail, lastOnline, onlinetime, points, data");

    QString values;

    values += "'" + _name + "', ";
    values += "'" + _passSHA256 + "', ";
    values += "'" + _mail + "', ";
    values += "'" + QString::number(_lastOnline) + "', ";
    values += "'" + QString::number(_onlineTime) + "', ";
    values += "'" + QString::number(_points) + "', ";
    values += ":bytes";

    if (!q.prepare(queryString)) {
        return false;
    }

    queryString = queryString.arg(values);

    QByteArray array;
    QDataStream s(&array, QIODevice::ReadWrite);
    s << _extraData;

    q.bindValue(":bytes", array);

    return q.exec();
}

bool UserData::remove(QSqlQuery &q) {
    return DBObject::remove(q);
}

void UserData::clear() {
    _name = "";
    _passSHA256 = "";
    _mail = "";
    _lastOnline = 0;
    _onlineTime = 0;
    _extraData = {};
    _points = 0;

    DBObject::clear();
}

bool UserData::isValid() const {
    return DBObject::isValid() && _mail.size();
}

SP<DBObject> UserData::factory() {
    return SP<UserData>::create();
}

const AccessToken &UserData::token() const {
    return _token;
}

void UserData::setToken(const AccessToken &token) {
    _token = token;
}

int UserData::points() const {
    return _points;
}

void UserData::setPoints(int points) {
    _points = points;
}

}
