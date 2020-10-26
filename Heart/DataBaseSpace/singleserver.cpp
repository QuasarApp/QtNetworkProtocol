#include "singleserver.h"

#include <user.h>
#include <sqldbcache.h>
#include <basenodeinfo.h>

namespace QH {

SingleServer::SingleServer()
{

}

RegisteruserResult SingleServer::registerNewUser(const PKG::User &user,
                                                 const AbstractNodeInfo *info) {
    if (!db()) {
        return RegisteruserResult::InternalError;
    }

    auto localObject = db()->getObject(user);

    if (localObject) {
        return RegisteruserResult::UserExits;
    }

    if (!db()->saveObject(&user)) {
        return RegisteruserResult::InternalError;
    };

    return loginUser(user, info);
}

RegisteruserResult SingleServer::loginUser(PKG::User user,
                                           const AbstractNodeInfo *info) {

    if (!db()) {
        return RegisteruserResult::InternalError;
    }

    auto localObject = db()->getObject(user);

    if (!localObject) {
        return RegisteruserResult::UserNotExits;
    }

    if (localObject->authenticationData() != user.authenticationData()) {
        return RegisteruserResult::UserInvalidPasswoed;
    }

    auto nodeinfo = dynamic_cast<const BaseNodeInfo*>(info);
    if (!nodeinfo)
        return RegisteruserResult::InternalError;

    AccessToken token((nodeinfo->selfId().toBytes()));

    if (token.isValid()) {
        return RegisteruserResult::UserAlreadyLogged;
    }

    token = localObject->token();

    if (!token.isValid()) {
        token = generateToken(AccessToken::Year);
    }

    auto editableNodeInfo = static_cast<BaseNodeInfo*>(getInfoPtr(info->networkAddress()));
    if (!editableNodeInfo)
        return RegisteruserResult::InternalError;

    editableNodeInfo->setSelfId(token.toBytes());
    user.setToken(token);

    if (!sendData(&user, info->networkAddress())) {
        return RegisteruserResult::InternalError;
    };

    return RegisteruserResult::Success;
}

AccessToken SingleServer::generateToken(int length) {
    return AccessToken(length);
}

ParserResult SingleServer::parsePackage(const Package &pkg, const AbstractNodeInfo *sender) {
    auto parentResult = DataBaseNode::parsePackage(pkg, sender);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }


    if (H_16<QH::PKG::User>() == pkg.hdr.command) {
            QH::PKG::User obj(pkg);

            if (!obj.isValid()) {
                badRequest(sender->networkAddress(), pkg.hdr);
                return ParserResult::Error;
            }

            if (!workWithUserRequest(obj, pkg, sender)) {
                return QH::ParserResult::Error;
            }

            return QH::ParserResult::Processed;

        }

    return QH::ParserResult::NotProcessed;

}

bool SingleServer::workWithUserRequest(const PKG::User& obj,
                                       const Package &pkg,
                                       const AbstractNodeInfo *sender) {
    RegisteruserResult result = RegisteruserResult::InternalError;

    if (obj.type() == PKG::UserRequestType::Login) {
        result = loginUser(obj, sender);
    } else if (obj.type() == PKG::UserRequestType::SignIn) {
        result = registerNewUser(obj, sender);
    }

    switch (result) {

    case RegisteruserResult::InternalError: {
        QuasarAppUtils::Params::log("Internal error ocured in the loginUser or registerNewUser method.",
                                    QuasarAppUtils::Error);
        badRequest(sender->networkAddress(), pkg.hdr, "Internal server error."
                                                      " Please create issue about this problem in the support page "
                                                      " https://github.com/QuasarApp/Heart/issues/new",
                   REQUEST_INTERNAL_ERROR);

        return false;
    }

    case RegisteruserResult::UserExits: {
        badRequest(sender->networkAddress(), pkg.hdr, "Such user already exists ", REQUEST_LOGIN_ERROR);
        return true;

    }

    case RegisteruserResult::UserNotExits: {
        badRequest(sender->networkAddress(), pkg.hdr, "Such user not exists ", REQUEST_LOGIN_ERROR);
        return true;

    }

    case RegisteruserResult::UserInvalidPasswoed: {
        badRequest(sender->networkAddress(), pkg.hdr, "Invalid password ", REQUEST_LOGIN_ERROR);
        return true;

    }

    case RegisteruserResult::UserAlreadyLogged: {
        badRequest(sender->networkAddress(), pkg.hdr, "User Already Logged", REQUEST_LOGIN_ERROR);
        return true;

    }

    case RegisteruserResult::Success: {
        return true;
    }
    }

    badRequest(sender->networkAddress(), pkg.hdr, "Internal server error. RegisteruserResult return invalid value!"
                                                  " Please create issue about this problem in the support page "
                                                  " https://github.com/QuasarApp/Heart/issues/new",
               REQUEST_INTERNAL_ERROR);
    return false;
}

}
