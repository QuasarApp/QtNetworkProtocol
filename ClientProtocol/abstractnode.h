#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include "abstractnodeinfo.h"
#include "clientprotocol.h"
#include <openssl/evp.h>

#include <QAbstractSocket>
#include <QSslConfiguration>
#include <QTcpServer>

class QSslCertificate;
class QSslKey;
class QSslConfiguration;

namespace ClientProtocol {

enum class SslMode {
    NoSSL,
    InitFromSystem,
    InitSelfSigned
};

/**
 * @brief The NodeInfoData struct
 */
struct NodeInfoData {
    AbstractNodeInfo info;
    BasePackage pkg;
};

/**
 * @brief The SslSrtData struct
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
 * @brief The AbstractNode class
 */
class AbstractNode : public QTcpServer
{
    Q_OBJECT

public:

    /**
     * @brief AbstractNode
     * @param ssl
     * @param ptr
     */
    AbstractNode(SslMode mode = SslMode::NoSSL, QObject * ptr = nullptr);

    /**
     * @brief run
     * @param addres
     * @param port
     * @return
     */
    virtual bool run(const QString& addres, unsigned short port);

    /**
     * @brief stop stop this node
     */
    virtual void stop();

    /**
     * @brief getInfo
     * @param id of selected node
     * @return pointer to information about node
     */
    virtual AbstractNodeInfo *getInfoPtr(quint32 id);

    /**
     * @brief getInfo
     * @param id
     * @return information about Node
     */
    virtual AbstractNodeInfo getInfo(quint32 id) const;

    /**
     * @brief getInfo
     * @param id of selected node
     * @return pointer to information about node
     */
    virtual AbstractNodeInfo *getInfoPtr(const QHostAddress &id);

    /**
     * @brief getInfo
     * @param id peer adders
     * @return information about Node
     */
    virtual AbstractNodeInfo getInfo(const QHostAddress &id) const;

    /**
     * @brief ban
     * @param target id of ban node
     */
    virtual void ban(quint32 target);

    /**
     * @brief unBan
     * @param target id of unban node
     */
    virtual void unBan(quint32 target);

    /**
     * @brief connectToHost
     */
    virtual void connectToHost(const QHostAddress &ip, unsigned short port, SslMode mode = SslMode::NoSSL);

    /**
     * @brief port
     * @return current node port
     */
    unsigned short port() const;

    /**
     * @brief address
     * @return return current adders
     */
    QHostAddress address() const;

    /**
     * @brief getSslConfig
     * @return current ssl configuration on this nod
     */
    QSslConfiguration getSslConfig() const;

    ~AbstractNode() override;

    SslMode getMode() const;

signals:
    void incomingReques(BasePackage pkg, quint32 sender);


protected:

    /**
     * @brief generateRSAforSSL
     * @param pkey -
     * @return
     */
    virtual bool generateRSAforSSL(EVP_PKEY* pkey) const;

    /**
     * @brief generateSslData - generate new ssl data
     * @param data - sign data
     * @param r_srt - result srt
     * @param r_key - result private key
     * @return true if all good
     */
    virtual bool generateSslDataPrivate(const SslSrtData& data, QSslCertificate& r_srt, QSslKey& r_key);

    /**
     * @brief selfSignedSslConfiguration
     * @return generate new keys and use it
     */
    virtual QSslConfiguration selfSignedSslConfiguration();

    /**
     * @brief registerSocket
     * @param socket
     * @return
     */
    virtual bool registerSocket(QAbstractSocket *socket);

    /**
     * @brief parsePackage
     * @param pkg
     * @param sender
     * @return
     */
    virtual bool parsePackage(const BasePackage &pkg, AbstractNodeInfo *sender);

    /**
     * @brief sendPackage
     * @param pkg
     * @param target
     * @return
     */
    virtual bool sendPackage(const BasePackage &pkg, QAbstractSocket *target);

    /**
     * @brief sendResponse
     * @param resp
     * @param address
     * @param req
     * @return
     */
    virtual bool sendResponse(const Abstract& resp,  quint32 address,
                              const BaseHeader *req = nullptr);

    /**
     * @brief badRequest
     * @param address
     * @param req
     */
    virtual void badRequest(quint32 address, const BaseHeader &req);

    /**
     * @brief getWorkState
     * @return string of work state
     */
    virtual QString getWorkState() const;

    /**
     * @brief connectionState
     * @return string with count users state
     */
    virtual QString connectionState() const;

    /**
     * @brief baned
     * @return
     */
    QStringList baned() const;

    /**
     * @brief connectionsCount
     * @return
     */
    int connectionsCount() const;

    /**
     * @brief isBaned
     * @param socket
     * @return
     */
    bool isBaned(QAbstractSocket* socket) const;

    /**
     * @brief incomingConnection
     * @param handle
     */
    void incomingConnection(qintptr handle) override;

    /**
     * @brief changeTrust change trust of connected node
     * @param id - id of select node
     * @param diff
     * @return true if all good
     */
    bool changeTrust(quint32 id, int diff);

    /**
    * @brief incomingConnection for ssl sockets
    * @param handle - handle of socket
    */
    virtual void incomingSsl(qintptr handle);

    /**
    * @brief incomingConnection for tcp sockets
    * @param handle -  handle of socket
    */
    virtual void incomingTcp(qintptr handle);


    /**
     * @brief setMode - invoke this method befor run method
     * @param mode
     */
    bool setMode(const SslMode &mode);


private slots:

    void avelableBytes();
    void handleDisconnected();

private:
    SslMode _mode;
    QSslConfiguration _ssl;
    QHash<unsigned int, NodeInfoData> _connections;


};
}
#endif // ABSTRACTNODE_H