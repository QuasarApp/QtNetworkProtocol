/*
 * Copyright (C) 2018-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#ifndef QSECRETRSA2048_H
#define QSECRETRSA2048_H

#include "icrypto.h"

#include <QHash>

class QRSAEncryption;

namespace QH {

/**
 * @brief The QSecretRSA2048 class Use a QtSecret library for implementation RSA alghoritms.
 */
class HEARTSHARED_EXPORT QSecretRSA2048: public ICrypto
{
    // ICrypto interface
public:
    QSecretRSA2048();
    ~QSecretRSA2048() override;
    bool crypt(QByteArray *data, const QByteArray &publicKey) override;
    bool decrypt(QByteArray *cryptedData, const QByteArray &privateKey) override;
    bool sign(QByteArray *data, const QByteArray &privateKey) override;
    bool check(const QByteArray &signedData, const QByteArray &publicKey) override;
    QByteArray extractSign(const QByteArray &data) override;
    QByteArray concatSign(const QByteArray &data, const QByteArray &sign) override;

    bool isValid() const override;

protected:
    CryptoPairKeys generate(const QByteArray& genesis = {}) const override;


private:
    QRSAEncryption *qtSecret = nullptr;


};
}
#endif // QSECRETRSA2048_H
