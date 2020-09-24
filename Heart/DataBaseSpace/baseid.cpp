#include "baseid.h"
namespace QH {

BaseId::BaseId() = default;

BaseId::BaseId(unsigned int val) {
    fromRaw(reinterpret_cast<char*>(&val), sizeof (val));
}

BaseId::BaseId(const QByteArray &raw) {
    fromRaw(raw);
}

BaseId::BaseId(const QString &base64) {
    fromBase64(base64.toLatin1());
}

bool BaseId::fromBase64(const QByteArray &base64) {
    return fromRaw(QByteArray::fromBase64(base64, QByteArray::Base64UrlEncoding));
}

bool BaseId::fromRaw(const QByteArray &raw) {
    _data = raw;
    return isValid();
}

bool BaseId::fromRaw(const char *data, int len) {
    _data.clear();
    _data.insert(0, data, len);
    return isValid();
}

QByteArray BaseId::toBase64() const {
    return _data.toBase64(QByteArray::Base64UrlEncoding);
}

const QByteArray& BaseId::toRaw() const {
    return _data;
}

bool BaseId::isValid() const {
    return _data.size();
}

void BaseId::clear() {
    _data.clear();
}

unsigned char BaseId::prefix() const {
    if (_data.size())
        return _data[0];

    return 0;
}

QDataStream &BaseId::fromStream(QDataStream &stream) {
    stream >> _data;
    return stream;
}

QDataStream &BaseId::toStream(QDataStream &stream) const {
    stream << _data;
    return stream;
}

bool operator==(const BaseId &left, const BaseId &other) {
    return left._data == other._data;
}


uint qHash(const QH::BaseId &object) {
    return qHash(object.toRaw());
}
}
