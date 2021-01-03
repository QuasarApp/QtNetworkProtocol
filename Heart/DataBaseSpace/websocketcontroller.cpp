/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "databasenode.h"
#include "abstractnodeinfo.h"
#include "websocketcontroller.h"
#include <quasarapp.h>
#include <permission.h>

namespace QH {

using namespace PKG;

WebSocketController::WebSocketController(DataBaseNode *node) {
    _node = node;
    assert(_node);
}

bool WebSocketController::subscribe(const QVariant &subscriber,
                                    const DbAddress &item) {

    _subscribsMutex.lock();
    _subscribs[item].insert(subscriber);
    _subscribsMutex.unlock();

    _itemsMutex.lock();
    _items[subscriber].insert(item);
    _itemsMutex.unlock();

    return true;
}

void WebSocketController::unsubscribe(const QVariant &subscriber,
                                      const DbAddress& item) {
    _subscribsMutex.lock();
    _subscribs[item].remove(subscriber);
    _subscribsMutex.unlock();

    _itemsMutex.lock();
    _items[subscriber].remove(item);
    _itemsMutex.unlock();

}

const QSet<DbAddress> &WebSocketController::list(const QVariant &subscriber) {
    QMutexLocker locker(&_itemsMutex);
    return _items[subscriber];
}

void WebSocketController::handleItemChanged(const QSharedPointer<DBObject> &item) {
    auto obj = item.dynamicCast<DBObject>();
    if (!obj)
        return;

    QMutexLocker locker(&_subscribsMutex);

    foreachSubscribers(item, _subscribs.value(obj->dbAddress()));
}

void WebSocketController::foreachSubscribers(const QSharedPointer<DBObject> &item,
                                             const QSet<QVariant> &subscribersList) {

    for (const auto &subscriber : subscribersList) {
        bool fAllowed = _node->checkPermission(subscriber, item->dbAddress(), Permission::Read) ==
                DBOperationResult::Allowed;

        if (fAllowed &&  !_node->sendData(item.data(), subscriber)) {
            QuasarAppUtils::Params::log("Send update failed for " + subscriber.toString(),
                                               QuasarAppUtils::Warning);

            unsubscribe(subscriber, item->dbAddress());
        }

        if (!fAllowed) {
            QuasarAppUtils::Params::log(QString("Internal Error. Member:%0  not have permission to object %1").
                                        arg(subscriber.toString(), item->toString()),
                                            QuasarAppUtils::Error);
            unsubscribe(subscriber, item->dbAddress());
        }
    }
}
}

uint qHash(const QVariant &variant) {
    return qHash(variant.toByteArray());
}
