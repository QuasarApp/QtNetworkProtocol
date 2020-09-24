﻿/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include "abstractnodeinfo.h"
#include <openssl/evp.h>

#include <QAbstractSocket>
#include <QFutureWatcher>
#include <QMutex>
#include <QSslConfiguration>
#include <QTcpServer>
#include <QTimer>
#include "abstractdata.h"
#include "workstate.h"
#include "package.h"
#include "cryptopairkeys.h"
#include "icrypto.h"
#include "heart_global.h"
#include "packagemanager.h"

class QSslCertificate;
class QSslKey;
class QSslConfiguration;

namespace QH {

class DataSender;
class ReceiveData;

/**
 * @brief The ParserResult enum
 * Error - parser detect a errorob package
 * NotProcessed - the parser does not know what to do with the package or has not finished processing it.
 * Processed - the parser finished processing correctly
 */
enum class ParserResult {
    /// parser detect a errorob package
    Error = 0,
    /// the parser does not know what to do with the package or has not finished processing it.
    NotProcessed = 1,
    /// the parser finished processing correctly
    Processed = 2
};

/**
 * @brief The SslMode enum  This enum contatins options for set ssl mode of node (server)
 * For more information see AbstractNode::useSelfSignedSslConfiguration AbstractNode::useSystemSslConfiguration and AbstractNode::disableSSL methods.
 */
enum class SslMode {
    /// This is not secure connection without ssl encription. It is default value of new any node see AbstractNode(SslMode mode = SslMode::NoSSL, QObject * ptr = nullptr)
    NoSSL,
    /// This option try enable ssl connection from system configuration form fore information see Qt Documentation https://doc.qt.io/qt-5/qsslconfiguration.html
    InitFromSystem,
    /// This option force a current node geneerate self signed sertificat and work with it. For more information see a SslSrtData struct
    InitSelfSigned
};

/**
 * @brief The SslSrtData struct This structure contains base information for generate self signed ssl certefication.
 *  If yo want change selfSigned certificate then use method AbstractNode::useSelfSignedSslConfiguration
 */
struct SslSrtData {
    QString country = "BY";
    QString organization = "QuasarApp";
    QString commonName = "Dev";
    long long endTime = 31536000L; //1 year
};


#define CRITICAL_ERROOR -50
#define LOGICK_ERROOR   -20
#define REQUEST_ERROR   -5

class Abstract;

/**
 * @brief The AbstractNode class - Abstract implementation of node.
 *  this implementation have a methods for send and receive data messages,
 *  and work with crypto method for crease a security connections betwin nodes.
 *  AbstractNode - is thread save class
 */
class HEARTSHARED_EXPORT AbstractNode : public QTcpServer
{
    Q_OBJECT

public:

    /**
     * @brief AbstractNode - base constructor of node.
     * @param ptr - pointrt to parent Qt object, the AbstractNode class is Q_OBJECT
     */
    AbstractNode(QObject * ptr = nullptr);
    ~AbstractNode() override;

    /**
     * @brief run - this method implement deployment a network node (server) on selected address.
     * @param addres - If address is empty then server weel be listen all addreses of all interfaces else listen only selected address.
     * @param port - This is port of deployment node (server)
     * @return Result of deployment node (sever). (True if deploy finished successful else false).
     */
    virtual bool run(const QString& addres, unsigned short port);

    /**
     * @brief stop - stopped this node and close all network connections.
     */
    virtual void stop();

    /**
     * @brief getInfoPtr - This method return information class pointer about netwok connection.
     *  If Connection with id not found then return nullptr.
     * @param id - it is network address of requested node
     * @return The pointer of information about node. if address not found return nullptr
     */
    virtual AbstractNodeInfo* getInfoPtr(const HostAddress &id);

    /**
     * @brief getInfoPtr - this is some that getInfoPtr(const HostAddress &id) bod it is constant implementation.
     * @param id - it is network address of requested node
     * @return The pointer of information about node. if address not found return nullptr
     */
    virtual const AbstractNodeInfo* getInfoPtr(const HostAddress &id) const;

    /**
     * @brief ban - this method set for target connection a trust property to 0 and target connection will been aborted.
     * @param target - it is network address of target connection.
     */
    virtual void ban(const HostAddress& target);

    /**
     * @brief unBan - this method set for target connection a trust property to 100.
     * @param target - it is network address of target connection.
     */
    virtual void unBan(const HostAddress& target);

    /**
     * @brief connectToHost - connect to node (server) with address.
     * @param address - This is Network address of node (server)
     * @param mode - This is mode of connection see SslMode.  By default using SslMode::NoSSL connection mode, it is not secure.
     */
    virtual bool connectToHost(const HostAddress &address, SslMode mode = SslMode::NoSSL);

    /**
     * @brief connectToHost - connect to node (server) with domain, bud this method find ip address of domain befor connecting
     * @param domain - This is domain address of node (server)
     * @param port - This is target port of node (server)
     * @param mode - This is mode of connection see SslMode. By default using SslMode::NoSSL connection mode, it is not secure.
     */
    virtual bool connectToHost(const QString &domain, unsigned short port, SslMode mode = SslMode::NoSSL);

    /**
     * @brief addNode - add new node (server) for this mode
     * @param nodeAdderess - This is network addres of a new node (server).
     * @note By Default This immplementation move called function into main Thread and invoke connectToHost method.
     */
    void addNode(const HostAddress& nodeAdderess);

    /**
     * @brief removeNode - remove node and disconnected forom node (server)
     * @param nodeAdderess - This is network adddress of removed node (server).
     */
    void removeNode(const HostAddress& nodeAdderess);

    /**
     * @brief address - Thim method return own network address of current node (server)
     * @return The current network adderss
     */
    HostAddress address() const;

    /**
     * @brief getSslConfig - This method return ssl configuration of current node (server).
     * @return current ssl configuration on this node (server)
     */
    QSslConfiguration getSslConfig() const;

    /**
     * @brief getMode - This method return SSL mode of corrent node (server).
     * @return current mode for more information see SslMode
     */
    SslMode getMode() const;

    /**
     * @brief getWorkState - This method collect general information about this server.
     *  For more information about returned data see getWorkState
     * @return state value for more information see WorkState class
     */
    virtual WorkState getWorkState() const;

    /**
     * @brief pareseResultToString This method convert ParserResult value to string.
     * @return The String value of pareseresult
     */
    QString pareseResultToString(const ParserResult& parseResult) const;

    /**
     * @brief connectionsCount - return count fo connections (connections with status connected)
     * @return count valid connections.
     */
    int connectionsCount() const;

    /**
     * @brief connectionsCount - return count of nodes with status confirmend
     * @return return confirmend connections of this node (server)
     */
    int confirmendCount() const;

    /**
     * @brief ping This method send ping package to address for testing connection
     * @param address This is address of target node (server)
     * @return true if ping sendet successful.
     */
    bool ping( const HostAddress& address);

signals:
    /**
     * @brief requestError This signal emited when client or node received from remoute server or node the BadRequest package.
     * @param msg - received text of remoute node (server).
     */
    void requestError(QString msg);

protected:

    /**
     * @brief generateRSAforSSL This method generate ssl rsa pair keys for using in selfsigned cetificate.
     *  By default generate RSA 2048, if you want change algorithm or keys size then override this method.
     * @param pkey This is openssl pointer to RSA pair key.
     * @return True if keys generated successful.
     */
    virtual bool generateRSAforSSL(EVP_PKEY* pkey) const;

    /**
     * @brief generateSslDataPrivate This method generate a ssl certificate and a ssl keys using The SslSrtData structure.
     * @param data The data for generate a selfSigned certificate.
     * @param r_srt This is return value of a certivicate
     * @param r_key - This is return value of private ssl key
     * @return True if generate the selfSigned certificate finished succesful.
     */
    virtual bool generateSslDataPrivate(const SslSrtData& data, QSslCertificate& r_srt, QSslKey& r_key);

    /**
     * @brief selfSignedSslConfiguration This method create a new ssl configuration with selfsigned certificates.
     * @param data This is data for generate selfsigned certification for more information see SslSrtData structure.
     * @return The new selfsigned ssl configuration
     */
    virtual QSslConfiguration selfSignedSslConfiguration( const SslSrtData& data = {});

    /**
     * @brief createNodeInfo This method create a nodeInfo object.
     *  override this method for create your own nodeInfo objects. for more in
     * @param socket This is socket of network address
     * @param clientAddress This parameter need to set when the socket du not contains a address or invalid.
     * @return return pointer to info object.
     */
    virtual AbstractNodeInfo* createNodeInfo(QAbstractSocket *socket,
                                             const HostAddress *clientAddress = nullptr) const;

    /**
     * @brief registerSocket This method registration new socket object.
     * @param socket This is incomming socket pointer
     * @param address This is host address of socket. By default is nullptr.
     *  Set this value for nodes created on this host
     * @return return true if the scoeket has been registered successful
     */
    virtual bool registerSocket(QAbstractSocket *socket, const HostAddress* address = nullptr);

    /**
     * @brief parsePackage This is main method of all childs classes of an AbstractNode class.
     *  This method work on own thread.
     *  If you ovveride this method you need to create this than an example:
     * \code
        ParserResult DataBaseNode::parsePackage(const Package &pkg,
                                                const AbstractNodeInfo *sender) {
            auto parentResult = AbstractNode::parsePackage(pkg, sender);
            if (parentResult != ParserResult::NotProcessed) {
                return parentResult;
            }

            if (H_16<MyCommand>() == pkg.hdr.command) {
                MyCommand obj
                obj.fromPackage(pkg);

                BaseId requesterId = getSender(sender, &obj);

                ...

                if (FailCondition) {
                    return ParserResult::Error;
                }

                ...

                return ParserResult::Processed;

            }

            return ParserResult::NotProcessed;
        }
     * \endcode
     * @param pkg This is package with incomming data.
     * @param sender This is sender this pacakge
     * @return item of ParserResult. For more information see The ParserResult enum.
     */
    virtual ParserResult parsePackage(const Package &pkg, const AbstractNodeInfo* sender);

    /**
     * @brief sendPackage This method prepare and send to target address a package.
     * @param pkg This is sendet pakcage to target node.
     * @param target This is target node.
     * @return return true if The package is sendet succesfull
     */
    virtual bool sendPackage(const Package &pkg, QAbstractSocket *target) const;

    /**
     * @brief sendData This pakcage send data package to address and prepare object to sending.
     * @param resp This is pointer to sendet object
     * @param address This is target addres for sending
     * @param req This is header of request
     * @return true if data sendet succesful.
     */
    virtual bool sendData(PKG::AbstractData *resp,  const HostAddress& address,
                          const Header *req = nullptr);

    /**
     * @brief sendData this is some as a sendData(AbstractData *resp ...) exept this method not prepare object for sending.
     * @param resp This is pointer to sendet object
     * @param address This is target addres for sending
     * @param req This is header of request
     * @return true if data sendet succesful.
     */
    virtual bool sendData(const PKG::AbstractData *resp,  const HostAddress& address,
                          const Header *req = nullptr);

    /**
     * @brief badRequest This method is send data about error of request
     * @param address This is addrees of receiver
     * @param req This is header of incomming request
     * @param msg This is message of error
     */
    virtual void badRequest(const HostAddress &address, const Header &req,
                            const QString msg = "");

    /**
     * @brief getWorkStateString This method generate string about work state of server.
     * @return string of work state
     */
    virtual QString getWorkStateString() const;

    /**
     * @brief connectionState This method return string value about the cocction state
     * @return string with count users state
     */
    virtual QString connectionState() const;

    /**
     * @brief banedList This method retrun list of banned clients of nodes.
     * @return list of baned nodes
     */
    QList<HostAddress> banedList() const;

    /**
     * @brief isBanned This method checks if the node is banned.
     * @param socket This is node info object for validation
     * @return true if node is banned
     */
    bool isBanned(QAbstractSocket* socket) const;

    /**
     * @brief incomingConnection This is ovverided method of QTCPServer
     * @param handle This is socket handel
     */
    void incomingConnection(qintptr handle) override final;

    /**
     * @brief changeTrust This method change trust of connected node
     * @param id This is id of select node
     * @param diff This is difference of current trust (currenTrus += diff)
     * @return true if node Trust is changed successful
     */
    virtual bool changeTrust(const HostAddress& id, int diff);

    /**
    * @brief incomingConnection This methiod work with incomming  ssl sockets.
    * @param handle - handle of socket
    */
    virtual void incomingSsl(qintptr handle);

    /**
    * @brief incomingConnection This methiod work with incomming  tcp sockets.
    * @param handle - handle of socket
    */
    virtual void incomingTcp(qintptr handle);

    /**
     * @brief useSelfSignedSslConfiguration This method reconfigure current node to use selfSigned certificate.
     * @note Befor invoke this method stop this node (server) see AbstractNode::stop. if mode will be working then this method return false.
     *  The self signed certificate is temp value, this is will be changed after reboot node (server)
     * @param crtData - This is data for generation a new self signed certification.
     * @return result of change node ssl configuration.
     */
    bool useSelfSignedSslConfiguration(const SslSrtData& crtData);

    /**
     * @brief useSystemSslConfiguration This method reconfigure current node to use sslConfig.
     * @note Befor invoke this method stop this node (server) see AbstractNode::stop. if mode will be working then this method return false.
     * @param sslConfig This is ssl configuration ot a current node (server)
     * @return result of change node ssl configuration.
     */
    bool useSystemSslConfiguration(const QSslConfiguration& sslConfig);

    /**
     * @brief disableSSL This method disable ssl mode for this node
     * @note Befor invoke this method stop this node (server) see AbstractNode::stop. if mode will be working then this method return false.
     * @return true if changes is completed.
     */
    bool disableSSL();


    /**
     * @brief incomingData This method invoked when node get command or ansver. But in default implemmentation it using only for Ping command. Add
     * \code
     * incomingData(pkg, sender);
     * \endcode
     * Into the overrided AbstractNode::ParsePacakge method on your own server or client class.
     * @param pkg This is received package (in this implementation it is only the Ping command)
     * @param sender This is information of sender of the package
     * @note override this method for get a signals.
     */
    virtual void incomingData(PKG::AbstractData* pkg,
                      const HostAddress&  sender);

    /**
     * @brief connections - return hash map of all connections of this node.
     * @return return map of connections.
     */
    QHash<HostAddress, AbstractNodeInfo*> connections() const;

    /**
     * @brief connectionRegistered Override this method for get registered incoming connections.
     * @param info - connection information.
     */
    virtual void connectionRegistered(const AbstractNodeInfo *info);

    /**
     * @brief nodeStatusChanged This method invoked when status of node chganged.
     *  Base implementation do nothing. Override this method for add own functionality.
     * @param node This is address of changed node.
     * @param status This is new status of node.
     *
     */
    void nodeStatusChanged(const HostAddress& node, NodeCoonectionStatus status);

    /**
     * @brief nodeConfirmend This method invocked when the node status changed to "confirmend"
     *  default implementatio do nothing
     * @param node This is address of changed node
     */
    virtual void nodeConfirmend(const HostAddress& node);

    /**
     * @brief nodeConnected This method invocked when the node status changed to "connected"
     *  default implementatio do nothing
     * @param node This is address of changed node
     */
    virtual void nodeConnected(const HostAddress& node);

    /**
     * @brief nodeConnected This method invocked when the node status changed to "disconnected"
     *  default implementatio do nothing
     * @param node This is address of changed node
     */
    virtual void nodeDisconnected(const HostAddress& node);


    /**
     * @brief pushToQueue - This method add action to queue. When the node status will be equal 'triggerStatus' then node run a action method.
     * @param node This is address of node that changed status.
     * @param action This is lyamda function with action.
     * @param triggerStatus This is awaiting status of node.
     */
    void pushToQueue(const std::function<void ()> &action,
                     const HostAddress& node,
                     NodeCoonectionStatus triggerStatus);

    /**
     * @brief takeFromQueue This method take the list of actions of node.
     *  After invoke take elements will be removed.
     * @param node This is address of node that changed status.
     * @param triggerStatus This is awaiting status of node.
     * @return The list of an actions
     */
    QList<std::function<void ()>> takeFromQueue(const HostAddress& node,
                                                NodeCoonectionStatus triggerStatus);
private slots:

    void avelableBytes();
    void handleDisconnected();
    void handleConnected();
    void handleCheckConfirmendOfNode(HostAddress node);

    /**
     * @brief handleWorkerStoped
     */
    void handleWorkerStoped();

    /**
     * @brief handleForceRemoveNode - force remove connection.
     * @param node
     */
    void handleForceRemoveNode(HostAddress node);

    /**
     * @brief connectNodePrivate
     */
    void connectNodePrivate(QH::HostAddress);

private:

    /**
      @note just disaable listen method in the node objects.
     */
    bool listen(const HostAddress& address = HostAddress::Any);

    /**
     * @brief newWork - this method it is wraper of the parsePackage method.
     *  the newWork invoke a parsePackage in the new thread.
     * @param pkg
     * @param sender
     */
    void newWork(const Package &pkg, const AbstractNodeInfo* sender, const HostAddress &id);


    /**
     * @brief nodeConfirmet - this metthod invoked when node is confirment.
     * @param sender - node with new status;
    */
    void nodeConfirmet(const HostAddress &node);

    /**
     * @brief checkConfirmendOfNode - this method remove old not confirmed node.
     * @param node - node address
     */
    void checkConfirmendOfNode(const HostAddress& node);

    SslMode _mode = SslMode::NoSSL;
    QSslConfiguration _ssl;
    QHash<HostAddress, AbstractNodeInfo*> _connections;
    QHash<HostAddress, ReceiveData*> _receiveData;

    QHash<HostAddress, QHash<NodeCoonectionStatus, QList<std::function<void()>>>> _actionCache;

    DataSender * _dataSender = nullptr;

    QSet<QFutureWatcher <bool>*> _workers;

    PackageManager _packageManager;


    mutable QMutex _connectionsMutex;
    mutable QMutex _actionCacheMutex;
    mutable QMutex _confirmNodeMutex;

    friend class WebSocketController;


};

}
#endif // ABSTRACTNODE_H
