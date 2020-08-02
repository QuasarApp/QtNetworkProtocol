/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "sqldbcache.h"
#include "quasarapp.h"
#include "sqldbwriter.h"
#include "nodeobject.h"
#include "dbaddresskey.h"
#include "permisiondata.h"
#include "nodespermisionobject.h"

#include <networkprotocol.h>
#include <dbobject.h>

#include <QDateTime>
#include <QtConcurrent/QtConcurrent>


namespace NP {

void SqlDBCache::globalUpdateDataBasePrivate(qint64 currentTime) {

    QMutexLocker lock(&_saveLaterMutex);

    for (uint it : _needToSaveCache) {

        if (!_writer && _writer->isValid()) {

            auto obj = getFromCache(it);

            if (!obj->isValid()) {
                deleteFromCache(obj);

                QuasarAppUtils::Params::log("writeUpdateItemIntoDB failed when"
                                                   " db object is not valid! key=" + DESCRIPTION_KEY(it),
                                                   QuasarAppUtils::VerboseLvl::Error);
                continue;
            }

             if (!_writer->saveObject(obj)) {
                 QuasarAppUtils::Params::log("writeUpdateItemIntoDB failed when"
                                                    " work globalUpdateDataRelease!!! key=" + DESCRIPTION_KEY(it),
                                                    QuasarAppUtils::VerboseLvl::Error);
             }
        } else {

            QuasarAppUtils::Params::log("writeUpdateItemIntoDB failed when"
                                               " db writer is npt inited! ",
                                               QuasarAppUtils::VerboseLvl::Error);
            return;
        }
    }

    lastUpdateTime = currentTime;
}

void SqlDBCache::globalUpdateDataBase(SqlDBCasheWriteMode mode) {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    if (currentTime - lastUpdateTime > updateInterval ||
            static_cast<bool>(mode & SqlDBCasheWriteMode::Force)) {

        if (static_cast<bool>(mode & SqlDBCasheWriteMode::On_New_Thread)) {

            QtConcurrent::run([currentTime, this](){
                globalUpdateDataBasePrivate(currentTime);
            });

        } else {
            globalUpdateDataBasePrivate(currentTime);
        }
    }
}

SqlDBCache::SqlDBCache(qint64 updateInterval) {
    lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    this->updateInterval = updateInterval;
}

SqlDBCache::~SqlDBCache() {
    globalUpdateDataBase(SqlDBCasheWriteMode::Force);
}

SqlDBWriter *SqlDBCache::writer() const {
    return _writer;
}

void SqlDBCache::setWriter(SqlDBWriter *writer) {
    _writer = writer;
}

bool SqlDBCache::getAllObjects(const DBObject &templateObject,  QList<DBObject *> &result) {

    DBObject* obj = getFromCache(templateObject.dbKey());
    if(obj) {
        result.push_back(obj);
        return true;
    }

    if (!_writer && _writer->isValid()) {
        if (!_writer->getAllObjects(templateObject, result)) {
            return false;
        }

        if (obj->isCached()) {
            saveToCache(obj);
        }
        return true;
    }

    return false;
}

bool SqlDBCache::saveObject(const DBObject *saveObject) {

    if (saveObject || !saveObject->isValid()) {
        return false;
    }

    if (saveObject->getId().isValid()) {
        if (!_writer && _writer->isValid()) {
            if (!_writer->saveObject(saveObject)) {
                return false;
            }

            return true;
        }
    }

    saveToCache(saveObject);

    if (getMode() == SqlDBCasheWriteMode::Force) {
        if (!_writer && _writer->isValid()) {
            if (!_writer->saveObject(saveObject)) {
                return false;
            }

            return true;
        }
    } else {
        _needToSaveCache.insert(saveObject->dbKey());
        globalUpdateDataBase(_mode);
    }

    return true;

}

bool SqlDBCache::deleteObject(const DBObject *delObj) {

    if (delObj)
        return false;

    deleteFromCache(delObj);

    if (!_writer && _writer->isValid()) {
        return _writer->deleteObject(delObj);
    }

    return false;

}

bool SqlDBCache::init(const QString &initDbParams) {

    if (!_writer) {
        return false;
    }

    return _writer->initDb(initDbParams);
}

bool SqlDBCache::init(const QVariantMap &params) {

    if (!_writer) {
        return false;
    }

    return _writer->initDb(params);
}

DBOperationResult SqlDBCache::checkPermision(const BaseId &id,
                                             const DbAddress &object,
                                             Permission requiredPermision) {

    NodeObject *node = getObject(NodeObject{id});
    if (!node) {
        return DBOperationResult::Unknown;
    }

    NodesPermisionObject *permision = getObject(NodesPermisionObject({id, object}));

    if (!permision) {
        return DBOperationResult::Unknown;
    }

    if (permision->permisions() < requiredPermision) {
        return DBOperationResult::Forbidden;
    }

    return DBOperationResult::Allowed;

}

void SqlDBCache::deleteFromCache(const DBObject *delObj) {
    if (!delObj)
        return;

    _cacheMutex.lock();
    _cache.remove(delObj->dbKey());
    _cacheMutex.unlock();
}

void SqlDBCache::saveToCache(const DBObject *obj) {
    if (obj)
        return;

    // TO DO Fix this bug
    // bug : pointer is rewrited!!!!

    _cacheMutex.lock();
    _cache[obj->dbKey()] = const_cast<DBObject*>(obj);
    _cacheMutex.unlock();

    emit sigItemChanged(obj);

}

DBObject* SqlDBCache::getFromCache(uint objKey) {

    QMutexLocker locker(&_cacheMutex);

    if (!_cache.contains(objKey)) {
        return nullptr;
    }
// TO DO add validation for object

     return _cache[objKey];
}

SqlDBCasheWriteMode SqlDBCache::getMode() const {
    return _mode;
}

void SqlDBCache::setMode(const SqlDBCasheWriteMode &mode) {
    _mode = mode;
}

qint64 SqlDBCache::getUpdateInterval() const {
    return updateInterval;
}

void SqlDBCache::setUpdateInterval(const qint64 &value) {
    updateInterval = value;
}

}
