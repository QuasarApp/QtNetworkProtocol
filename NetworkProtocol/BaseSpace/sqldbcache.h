/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef SQLDBCASHE_H
#define SQLDBCASHE_H

#include "dbcachekey.h"
#include "iobjectprovider.h"

#include <QMap>
#include <QHash>
#include <QSet>
#include <QVariantMap>
#include <networkprotocol.h>
#include <QMutex>
#include "config.h"
#include <permisions.h>

namespace NP {

class SqlDBWriter;
class DBObject;

enum class SqlDBCasheWriteMode: int {
    Default = 0x0,
    On_New_Thread = 0x1,
    Force = 0x2,
} ;

/**
 * @brief The DBOperationResult enum
 */
enum class DBOperationResult {
    /// Node do not know about this operaio
    Unknown,
    /// Node allow this operation and exec it
    Allowed,
    /// Node forbid this operation.
    Forbidden,
};


/**
 * @brief The SqlDBCache class it is db cache and bridge for DbWriters
 */
class NETWORKPROTOCOLSHARED_EXPORT SqlDBCache: public QObject, public iObjectProvider
{
    Q_OBJECT


public:
    SqlDBCache(qint64 updateInterval = DEFAULT_UPDATE_INTERVAL);
    ~SqlDBCache() override;

    /**
     * @brief writer
     * @return weak pointer to writer
     */
    SqlDBWriter* writer() const;

    /**
     * @brief setWriter
     * @param writer
     */
    void setWriter(SqlDBWriter* writer);

    bool getObject(DBObject *obj) override;
    DBObject* getObjectFromCache(const DBCacheKey &key);

    bool saveObject(const DBObject* saveObject) override;
    bool deleteObject(const DBObject* delObj) override;

    /**
     * @brief getUpdateInterval of SqlDBCasheWriteMode::Default mode
     * @return time in msecs
     */
    qint64 getUpdateInterval() const;

    /**
     * @brief setUpdateInterval set update time of SqlDBCasheWriteMode::Default mode
     * @param value time in msecs
     */
    void setUpdateInterval(const qint64 &value);

    /**
     * @brief init
     * @return
     */\
    virtual bool init(const QString &initDbParams = "");

    /**
     * @brief init
     * @return
     */\
    virtual bool init(const QVariantMap &params);

    /**
     * @brief checkPermision - check permision of clientId for object.
     * @param id - id of node or client
     * @param object - target object
     * @param requiredPermision - requirement permision of object
     * @return operation result see DBOperationResult
     */
    virtual DBOperationResult checkPermision(const QByteArray& id,
                                             const DBObject &object,
                                             Permission requiredPermision);

protected:

    /**
     * @brief deleteFromCache - delete object  from db cache, bat not from database.
     * @param table - table of object
     * @param id in table of object
     * @return true if all good
     */
    virtual void deleteFromCache(const DBObject *delObj);

    /**
     * @brief saveToCache
     * @param obj
     */
    virtual void saveToCache(const DBObject *obj);

    /**
     * @brief getFromCache -  get database objcet from cache.
     * @param obj
     * @return
     */
    virtual bool getFromCache(DBObject* obj);

    /**
     * @brief getMode
     * @return
     */
    SqlDBCasheWriteMode getMode() const;

    /**
     * @brief setMode
     * @param mode
     */
    void setMode(const SqlDBCasheWriteMode &mode);


    /**
     * @brief globalUpdateDataBasePrivate
     * @param currentTime
     */
    virtual void globalUpdateDataBasePrivate(qint64 currentTime);

    /**
     * @brief globalUpdateDataBase
     * @param mode
     */
    virtual void globalUpdateDataBase(SqlDBCasheWriteMode mode = SqlDBCasheWriteMode::Default);

    SqlDBWriter* _writer = nullptr;
private:
    qint64 lastUpdateTime = 0;
    qint64 updateInterval = DEFAULT_UPDATE_INTERVAL;
    QMutex _saveLaterMutex;

    SqlDBCasheWriteMode _mode;

    QHash<DBCacheKey, DBObject*>  _cache;
    QSet<DBCacheKey>  _needToSaveCache;



signals:
    void sigItemChanged(const DBObject *obj);

};

}
#endif // SQLDBCASHE_H
