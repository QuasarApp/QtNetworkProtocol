#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include "abstractnodeinfo.h"
#include "clientprotocol_global.h"

#include <QByteArray>

class QAbstractSocket;
namespace ClientProtocol {

/**
 * @brief The BaseNodeInfo class with tocken support
 */
class CLIENTPROTOCOLSHARED_EXPORT BaseNodeInfo: public AbstractNodeInfo {

public:
    bool isValid() const override;

    explicit BaseNodeInfo(QAbstractSocket * tcp = nullptr,
                          NodeType type = NodeType::Client);
    ~BaseNodeInfo() override;

    /**
     * @brief token
     * @return token
     */
    QByteArray token() const;

    /**
     * @brief setToken
     * @param token set token
     */
    void setToken(const QByteArray &token);

protected:
    QByteArray _token;
};
}

#endif // CONNECTIONINFO_H