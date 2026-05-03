#ifndef CONTROLEAERIEN_GROUPE_2_G_CORE_H
#define CONTROLEAERIEN_GROUPE_2_G_CORE_H

#include "Libraries.h"
#include "util.h"
#include "Network.h"

class Core {
private:
    Network m_network;
public:
    Core();
    ~Core();
    void initialisation();
};
#endif
