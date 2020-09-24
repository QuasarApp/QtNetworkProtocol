
/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef WEBSOCKETCONTROLLER_H
#define WEBSOCKETCONTROLLER_H

#include <QHash>
#include <QSharedPointer>
#include <dbobject.h>
#include <baseid.h>
#include <QMutex>

namespace QH {

class AbstractNodeInfo;
class DataBaseNode;

/**
 * @brief The WebSocketController class - manage subscribe
 */
class HEARTSHARED_EXPORT WebSocketController : public QObject
{
    Q_OBJECT

public:
    WebSocketController(DataBaseNode *node);
    bool subscribe(const BaseId &subscriber,
                   const DbAddress &item);

    void unsubscribe(const BaseId &subscriber,
                     const DbAddress &item);

    const QSet<DbAddress> &list(const BaseId& subscriber);

public slots:
    void handleItemChanged(const PKG::DBObject *item);

private:
    void foreachSubscribers(const PKG::DBObject *item,
                            const QSet<BaseId> &subscribersList);

    /// subscribers it is nodes or clients
    QHash<DbAddress, QSet<BaseId>> _subscribs;
    QHash<BaseId, QSet<DbAddress>> _items;

    QMutex _subscribsMutex;
    QMutex _itemsMutex;

    DataBaseNode *_node = nullptr;

};

}
#endif // WEBSOCKETCONTROLLER_H