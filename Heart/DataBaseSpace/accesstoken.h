/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

#ifndef ACCSESTOKEN_H
#define ACCSESTOKEN_H

#include "streambase.h"

#include <QByteArray>
#include <QDateTime>
#include <heart_global.h>

namespace QH {

/**
 * @brief The AccessToken class is contains info about token and duration of this token.
 * Tiken is byte array with privileges. In usuallu cases toke add permisions for database objects.
 */
class HEARTSHARED_EXPORT AccessToken : public StreamBase
{
public:

    /**
     * @brief The Duration enum This enum contains a long time unit.
     */
    enum Duration: int {
        /// The one second
        Second = 1,
        /// The one minute
        Minute = 60 * Second,
        /// The one hour
        Hour = 60 * Minute,
        /// The one day
        Day = 24 * Hour,
        /// The one Week
        Week = 7 * Day,
        /// The one Month
        Month = 30 * Day,
        /// The one year
        Year = 365 * Day,
    };

    /**
     * @brief AccsesToken This constructor create token on duration (s)
     * @param duration This is value in secundes
     * @param entropy This is random bytes array for generate token. By Default is empty string
     */
    explicit AccessToken(int duration, const QByteArray& entropy = "");

    /**
     * @brief AccsesToken constructor by default create not valid token
     */
    explicit AccessToken();

    /**
     * @brief AccessToken copy constructor
     * @param other
     */
    AccessToken(const AccessToken& other);

    /**
     * @brief isValid This method check this toke to valid.
     * @return return true if token is valid;
     */
    bool isValid() const;

    /**
     * @brief clearThis method reset all data of token
     */
    void clear();

    /**
     * @brief operator == compare left and right values with type AccessToken
     * @param other This is another object of the AccessTokens
     * @return true if 2 object is equals
     */
    bool operator == (const AccessToken& other) const;

    /**
     * @brief operator != compare left and right values with type AccessToken
     * @param other This is another object of the AccessTokens
     * @return false if 2 object is equals
     */
    bool operator != (const AccessToken& other) const;

    /**
     * @brief operator = this is operator of copy objects
     * @param other
     * @return
     */
    AccessToken &operator =(const AccessToken& other);

    /**
     * @brief qHash This is hash function of the AccessToken type
     * @param token input value of AccessToken type
     * @return hash code of the AccessToken value.
     */
    friend uint qHash(const AccessToken& token);

protected:
    QDataStream &fromStream(QDataStream &stream);
    QDataStream &toStream(QDataStream &stream) const;

private:
    QByteArray _data;
    int _duration;

    // StreamBase interface


    QByteArray generate(const QByteArray &user);
};

}

#endif // ACCSESTOKEN_H