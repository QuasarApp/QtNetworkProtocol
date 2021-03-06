/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef BADNODEREQUEST_H
#define BADNODEREQUEST_H

#include "senderdata.h"

#include <badrequest.h>

namespace QH {
namespace PKG {

/**
 * @brief The BadNodeRequest class - bad request command for bae node level
 */
class HEARTSHARED_EXPORT BadNodeRequest: public BadRequest, public SenderData
{
public:
    explicit BadNodeRequest();
    explicit BadNodeRequest(const ErrorData &err);
    explicit BadNodeRequest(const Package& package);

    // AbstractData interface
    bool isValid() const;
    bool copyFrom(const AbstractData *);

    // StreamBase interface
protected:
    QDataStream &fromStream(QDataStream &stream);
    QDataStream &toStream(QDataStream &stream) const;
};
}
}
#endif // BADNODEREQUEST_H
