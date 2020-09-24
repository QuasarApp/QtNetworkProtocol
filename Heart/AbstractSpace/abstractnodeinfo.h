/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ABSTRACTNODEINFO_H
#define ABSTRACTNODEINFO_H
#include "hostaddress.h"
#include "heart_global.h"

#include <hostaddress.h>


class QAbstractSocket;
class QHostInfo;

namespace QH {

/**
 * @brief The TrustNode enum contains cases for trus of the client or nodes.
 */
enum class TrustNode: unsigned char {
    /// Undefined node
    Undefined = 0xFF,

    /// General trust of the node or client
    Default = 100,

    /// This trusted of unbaned nodes or clients. Recomendet trus of unbaned client.
    /// For more informations see method AbstrcatData::unBan
    Restore = 20,

    /// Node with this trusted is forbidden.
    Baned = 0
};

/**
 * @brief The AbstractNodeState enum - This is status of the known nodes or clients.
 */
enum class NodeCoonectionStatus: int {
    /// This node not sent data about its envirement and status of node socket is dissconnected.
    NotConnected,
    /// The node with this status has socket status is connected.
    Connected,
    ///  The node confirmend. Node with it status sent a information
    ///  requarement for confirm in to this node object.
    Confirmed,
};

/**
 * @brief qHash - simple hash function of NodeCoonectionStatus
 * @param status - input data.
 * @return crc32 hash code
 */
uint qHash(NodeCoonectionStatus status);

/**
 * @brief The AbstractNodeInfo class contains information about client or server connection and tcp socket of node.
 * all node Info classes must be initialized in the AbstractNode::createNodeInfo methods.
 * This implementation of nodeInf contains only trust of node and network socket.
 */
class HEARTSHARED_EXPORT AbstractNodeInfo
{

public:

    /**
     * @brief AbstractNodeInfo
     * @param sct socket of connection
     * @param address - address of socket
     */
    AbstractNodeInfo(QAbstractSocket *sct = nullptr,
                     const HostAddress* address = nullptr);

    /**
     * @brief ~AbstractNodeInfo
     */
    virtual ~AbstractNodeInfo();

    /**
     * @brief sct This method returtn socket of connection.
     * @return return socket of connection
     */
    QAbstractSocket *sct() const;

    /**
     * @brief disconnect This method disconnect device from host
     */
    virtual void disconnect();

    /**
     * @brief ban this node, set trust value to 0.
     */
    virtual void ban();

    /**
     * @brief isBanned - check node of banned.
     * @return true if the node is baned
     */
    virtual bool isBanned() const;

    /**
     * @brief unBan - set trust value of node to TrustNode::Restore.
     *  See TrustNode enum for more information.
     */
    virtual void unBan();

    /**
     * @brief trust - return current trust level of thge node or client
     * @return current trust value
     */
    virtual int trust() const;

    /**
     * @brief setTrust This method set manually value of a this node trust
     * @param trust - new value
     */
    virtual void setTrust(int trust);

    /**
     * @brief isValid - check node of valid. This implementation check connect status of socket.
     * @return true if node or client is valid.
     */
    virtual bool isValid() const;

    /**
     * @brief isConnected - check of node connect status.
     * @return true if the socket connected
     */
    virtual bool isConnected() const;

    /**
     * @brief fromStream
     * @param stream
     * @return stream
     */
    virtual QDataStream& fromStream(QDataStream& stream);

    /**
     * @brief toStream
     * @param stream
     * @return stream
     */
    virtual QDataStream& toStream(QDataStream& stream) const;

    /**
     * @brief info - this method return Host domain information.
     *  For more information see the Qt Documentation about QHostInfo.
     * @return Host info of this node.
     */
    QHostInfo *info() const;

    /**
     * @brief setInfo - set new host info for this node
     * @param info - host info
     */
    void setInfo(const QHostInfo &info);

    /**
     * @brief networkAddress This method return network address of current node or client.
     * @return network adderess of node
     */
    HostAddress networkAddress() const;

    /**
     * @brief setNetworkAddress This method update network address of the current node
     * @param networkAddress new address
     */
    void setNetworkAddress(const HostAddress &networkAddress);

    /**
     * @brief status This method return status of the node connection
     * @return connection status for more info see NodeCoonectionStatus
     */
    NodeCoonectionStatus status() const;

    /**
     * @brief setStatus - set new value of status node
     * @param status - new status
     */
    void setStatus(const NodeCoonectionStatus &status);

    /**
     * @brief confirmData This method check all data of node and return true
     * if node is confirmed.
     * @return true if node is confirmed
     */
    virtual bool confirmData() const;

    /**
     * @brief isLocal return true if connectuion opened on this node.
     * @return True if connection is opened on this host.
     */
    bool isLocal() const;

    /**
     * @brief setIsLocal - set local status for this Node.
     * @param isLocal
     */
    void setIsLocal(bool isLocal);

protected:
    /**
     * @brief setSct - set socket for this node or client
     * @param sct
     */
    void setSct(QAbstractSocket *sct);

private:

    QHostInfo *_info = nullptr;
    HostAddress _networkAddress;

    QAbstractSocket *_sct = nullptr;
    int _trust = static_cast<int>(TrustNode::Default);
    NodeCoonectionStatus _status = NodeCoonectionStatus::NotConnected;
    bool _isLocal = false;

};

}
#endif // ABSTRACTNODEINFO_H