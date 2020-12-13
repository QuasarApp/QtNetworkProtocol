/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#include "isqldbcache.h"
#include "quasarapp.h"
#include "sqldbwriter.h"
#include "dbaddresskey.h"
#include "permisiondata.h"

#include <dbobject.h>
#include <asyncsqldbwriter.h>

#include <QDateTime>
#include <QtConcurrent/QtConcurrent>


namespace QH {

using namespace PKG;

void ISqlDBCache::globalUpdateDataBase(SqlDBCasheWriteMode mode) {
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

qint64 ISqlDBCache::getLastUpdateTime() const
{
    return lastUpdateTime;
}

void ISqlDBCache::setLastUpdateTime(const qint64 &value)
{
    lastUpdateTime = value;
}

ISqlDBCache::ISqlDBCache(qint64 updateInterval, SqlDBCasheWriteMode mode) {
    lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    this->updateInterval = updateInterval;
    setMode(mode);
}

ISqlDBCache::~ISqlDBCache() {
    globalUpdateDataBase(SqlDBCasheWriteMode::Force);
}

SqlDBWriter *ISqlDBCache::writer() const {
    return _writer;
}

void ISqlDBCache::setWriter(SqlDBWriter *writer) {

    if ((_mode == SqlDBCasheWriteMode::Force) && dynamic_cast<AsyncSqlDbWriter*>(writer)) {
        throw std::runtime_error("You user the SqlDBCasheWriteMode::Force mode of ISqlDBCache and AsyncSqlDbWriter. "
                                  "This is this can cause your client or server to crash. Since the asynchronous save will be "
                                  "able to be performed for temporary objects. Use the SqlDBCasheWriteMode::Default or"
                                  " not async SqlDbWriter");

    }

    _writer = writer;
}

bool ISqlDBCache::getAllObjects(const DBObject &templateObject,  QList<const DBObject *> &result) {

    if (templateObject.isCached()) {
        DBObject* obj = getFromCache(&templateObject);
        if(obj) {
            result.push_back(obj);
            return true;
        }
    }

    if (_writer && _writer->isValid()) {
        if (!_writer->getAllObjects(templateObject, result)) {
            return false;
        }

        for (auto object: result) {
            if (object->isCached() && !insertToCache(object)) {
                QuasarAppUtils::Params::log("Selected object from database can not be saved into database cache. " +
                                            object->toString(),
                                            QuasarAppUtils::Warning);
            }
        }

        return true;
    }

    return false;
}

bool ISqlDBCache::deleteObject(const DBObject *delObj, bool wait) {

    if (!delObj)
        return false;

    deleteFromCache(delObj);

    if (_writer && _writer->isValid()) {
        return _writer->deleteObject(delObj, wait);
    }

    return false;

}

bool ISqlDBCache::updateObject(const DBObject *saveObject, bool wait) {

    if (!saveObject || !saveObject->isValid()) {
        return false;
    }

    if (saveObject->isCached() && updateCache(saveObject)) {

        if (getMode() == SqlDBCasheWriteMode::Force) {
            return _writer && _writer->isValid() &&
                   _writer->updateObject(saveObject, wait);
        }

        return true;
    }

    return  _writer && _writer->isValid() &&
            _writer->updateObject(saveObject);

}

bool ISqlDBCache::insertObject(const DBObject *saveObject, bool wait) {
    if (!saveObject || !saveObject->isValid()) {
        return false;
    }

    if (saveObject->isCached() && insertToCache(saveObject)) {

        if (getMode() == SqlDBCasheWriteMode::Force) {

            return _writer && _writer->isValid() &&
                    _writer->insertObject(saveObject, wait);
        }

        return true;
    }

    return _writer && _writer->isValid() &&
            _writer->insertObject(saveObject);
}

bool ISqlDBCache::init(const QString &initDbParams) {

    if (!_writer) {
        return false;
    }

    return _writer->initDb(initDbParams);
}

bool ISqlDBCache::init(const QVariantMap &params) {

    if (!_writer) {
        return false;
    }

    return _writer->initDb(params);
}

void ISqlDBCache::setSQLSources(const QStringList &list) {
    auto db = writer();
    if (db)
        db->setSQLSources(list);
}

bool ISqlDBCache::changeObjects(const DBObject &templateObject,
                               const std::function<bool (DBObject *)> &changeAction) {

    QList<const DBObject *> list;
    if (!getAllObjects(templateObject, list)) {
        return false;
    }

    if (!list.size())
        return false;

    for (const DBObject * obj :list) {
        auto cachedObject = getFromCache(obj);

        if (!cachedObject && obj->isCached())
            return false;

        DBObject *ptr = (cachedObject)? cachedObject: const_cast<DBObject*>(obj);

        if (!changeAction(ptr)) {
            return false;
        };

        if (!updateObject(ptr)) {
            return false;
        };
    }

    return true;
}

SqlDBCasheWriteMode ISqlDBCache::getMode() const {
    return _mode;
}

void ISqlDBCache::setMode(const SqlDBCasheWriteMode &mode) {
    _mode = mode;
}

void ISqlDBCache::globalUpdateDataBasePrivate(qint64 currentTime) {
    Q_UNUSED(currentTime)

    throw std::runtime_error("ISqlDBCache::globalUpdateDataBasePrivate is not implemented!");
}

qint64 ISqlDBCache::getUpdateInterval() const {
    return updateInterval;
}

void ISqlDBCache::setUpdateInterval(const qint64 &value) {
    updateInterval = value;
}

}