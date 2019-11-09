#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <clientprotocol.h>

namespace ClientProtocol {
    class Client;
}
class TestUtils
{
public:
    TestUtils();
    static bool wait(const bool &forWait, int msec);
    static bool loginFunc(ClientProtocol::Client &cli,
                              const QString &login,
                              const QByteArray &pass,
                              bool sendResult,
                              bool loginResult);

    static bool connectFunc(ClientProtocol::Client &cli,
                     const QString &address,
                     unsigned short port);
//    static bool getState(ServerProtocol::Client &cli, QVariantMap &state);
//    static bool unBanFunc(ServerProtocol::Client &cli, const QHostAddress &address);
//    static bool banFunc(ServerProtocol::Client &cli, const QHostAddress &address);
//    static bool reconnectFunc(ClientProtocol::Client &cli);
//    static bool registerFunc(ClientProtocol::Client &cli, const QString &login,
    //                             const QByteArray &pass, bool sendResult, bool loginResult);

};

#endif // TESTUTILS_H
