/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef TRANSPORTDATA_H
#define TRANSPORTDATA_H

#include "abstractdata.h"
#include "nodeid.h"
#include "senderdata.h"

namespace NP {

class NETWORKPROTOCOLSHARED_EXPORT TransportData: public AbstractData, public SenderData
{

public:
    explicit TransportData();
    explicit TransportData(const Package& package);

    const Package& data() const;
    void setData(const Package &data);

    bool isValid() const;

    NodeId targetAddress() const;
    void setTargetAddress(const NodeId &targetAddress);

protected:
    QDataStream &fromStream(QDataStream &stream);
    QDataStream &toStream(QDataStream &stream) const;

private:
    Package _data;
    NodeId _targetAddress;


};

}
#endif // TRANSPORTDATA_H
