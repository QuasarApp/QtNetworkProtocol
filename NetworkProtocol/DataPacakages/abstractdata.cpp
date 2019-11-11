#include "abstractdata.h"
#include <QDataStream>
#include <QMap>
#include <typeinfo>
#include "networkprotocol.h"

namespace NetworkProtocol {

unsigned int AbstractData::cmd() const {
    return _cmd;
}

AbstractData::AbstractData() {
    _cmd = static_cast<unsigned int>(generateId());
}

AbstractData::AbstractData(const NetworkProtocol::Package &package):
    AbstractData() {

    fromBytes(package.data);
}

bool AbstractData::fromBytes(const QByteArray &data) {
    return StreamBase::fromBytes(data);
}

unsigned int AbstractData::generateId() {
    return qHash(typeid(*this).name());
}

QByteArray AbstractData::toBytes() const {
    return StreamBase::toBytes();
}

bool AbstractData::toPackage(Package &package,
                        unsigned int trigeredCommand) const {

    if (!isValid()) {
        return false;
    }

    package.data = toBytes();

    package.hdr.command = _cmd;
    package.hdr.triggerCommnad = trigeredCommand;
    package.hdr.size = package.data.size();

    return package.isValid();
}

QDataStream &AbstractData::fromStream(QDataStream &stream) {
    stream >> _cmd;
    return stream;
}

QDataStream &AbstractData::toStream(QDataStream &stream) const {
    stream << _cmd;
    return stream;
}

bool AbstractData::isValid() const {
    return _cmd;
}

AbstractData::~AbstractData() {

}


}