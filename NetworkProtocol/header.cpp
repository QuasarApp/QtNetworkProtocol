#include "header.h"

namespace NP {
Header::Header() {
    reset();
}

bool Header::isValid() const {

    if (sizeof (*this) != 6) {
        return false;
    }

    return command && size;
}

void Header::reset() {
    size = 0;
    command = 0;
    triggerCommnad = 0;
}
}
