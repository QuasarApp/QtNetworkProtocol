/*
 * Copyright (C) 2018-2020 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


#include "packadata.h"
#include "package.h"

namespace QH {

PackaData::~PackaData() {
    delete _data;
}
}
