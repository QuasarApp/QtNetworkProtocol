/*
 * Copyright (C) 2018-2021 QuasarApp.
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
 * @brief The TrustNode enum contains cases for trust of the client or nodes.
 */
enum class TrustNode: unsigned char {
    /// Undefined node
    Undefined = 0xFF,

    /// General trust of the node or client
    Default = 100,

    /// This trusted of unbanned nodes or clients. Recomended trust of unbanned client.
    /// For more informations see method AbstrcatData::unBan
    Restore = 20,

    /// Node with this trust value is forbidden.
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
    ///  The node confirmed. Node with it status sent a information
    ///  requirement for confirm in to this node object.
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
class HEARTSHARED_EXPORT AbstractNodeInfo: public QObject
{
    Q_OBJECT
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
     * @brief sct This method return socket of connection.
     * @return return socket of connection
     */
    QAbstractSocket *sct() const;

    /**
     * @brief disconnect This method disconnect device from host
     * @param disableEvents This argument force nodeInfo object invoke a QObject::disconect method of socket object before close connection. This using on the poweroff signals. By Default this arguments = false.
     */
    virtual void disconnect();

    /**
     * @brief ban this node, set trust value to 0.
     */
    virtual void ban();

    /**
     * @brief isBanned - check node which banned.
     * @return true if the node is banned
     */
    virtual bool isBanned() const;

    /**
     * @brief unBan - set trust value of node to TrustNode::Restore.
     *  See TrustNode enum for more information.
     */
    virtual void unBan();

    /**
     * @brief trust - show current value of trust level node or client
     * @return current trust value
     */
    virtual int trust() const;

    /**
     * @brief setTrust This method set manually value of this node trust
     * @param trust - new value
     */
    virtual void setTrust(int trust);

    /**
     * @brief isValid - check node of valid. This method check connect status of socket.
     * @return true if node or client is valid.
     */
    virtual bool isValid() const;

    /**
     * @brief isConnected - check of node connect status.
     * @return true if the socket connected
     */
    virtual bool isConnected() const;

    /**
     * @brief reset This method reset all fields of the nodeInfo.
     * @note override this method if your node info has new fields.
     */
    virtual void reset();

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
     * @return network address of node
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
     * @brief setStatus This method Sets new value of status node
     * @param status This is a new status of the node.
     */
    void setStatus(const NodeCoonectionStatus &status);

    /**
     * @brief updateConfirmStatus This method invoked after process a received package and update confirm status of the node.
     */
    void updateConfirmStatus();

    /**
     * @brief isLocal return true if connection opened on this node.
     * @return True if connection is opened on this host.
     */
    bool isLocal() const;

    /**
     * @brief setIsLocal This method sets local status for this Node.
     * @param isLocal This is a new value.
     */
    void setIsLocal(bool isLocal);

    /**
     * @brief setSct This method sets a new socket for this node or client
     * @param sct This is a new valuse of the socket.
     */
    void setSct(QAbstractSocket *sct);

signals:

    /**
     * @brief sigConnected This is wrapper signal for the QAbstractSocket::connetced signal.
     * @param thisNode This is pointer to current object.
     */
    void sigConnected(AbstractNodeInfo* thisNode);

    /**
     * @brief sigDisconnected This is wrapper signal for the QAbstractSocket::disconnected signal.
     * @param thisNode This is pointer to current object.
     */
    void sigDisconnected(AbstractNodeInfo* thisNode);

    /**
     * @brief sigReadyRead This is wrapper signal for the QAbstractSocket::readyRead signal.
     * @param thisNode This is pointer to current object.
     */
    void sigReadyRead(AbstractNodeInfo* thisNode);

    /**
     * @brief sigErrorOccurred This is wrapper signal for the QAbstractSocket::errorOccurred signal.
     * @param thisNode This is pointer to current object.
     * @param socketError This is socket error code.
     * For more information see the QAbstractSocket::SocketError enum class.
     */
    void sigErrorOccurred(AbstractNodeInfo* thisNode, QAbstractSocket::SocketError socketError);

    /**
     * @brief sigConfirmed This signal emitted when node is confirmnd. The confirm status sets in the confirmData method.
     * @param thisNode This is pointer to current object.
     */
    void sigConfirmed(AbstractNodeInfo* thisNode);

    /**
     * @brief statusChaned This signal emitted when nodes status is changed.
     * @param thisNode This is pointer to current object.
     * @param status This is status of node. For more information see the NodeCoonectionStatus enum.
     */
    void statusChaned(AbstractNodeInfo* thisNode, NodeCoonectionStatus status);

protected:

    /**
     * @brief confirmData This method check all data of node and return true
     * if node is confirmed.
     * @return true if node is confirmed
     */
    virtual bool confirmData() const;

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

Q_DECLARE_METATYPE(QH::NodeCoonectionStatus)

#endif // ABSTRACTNODEINFO_H
