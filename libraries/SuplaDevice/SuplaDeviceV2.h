#ifndef __SUPLA_DEV_V2__
#define __SUPLA_DEV_V2__

#include "proto.h"

class SuplaDev {

public:
    SuplaDev();
    virtual SuplaDev() { }

    int begin();

private:
    TDS_SuplaRegisterDevice_C mSuplaDev;
    Tsrpc *mSrpc;
};

#endif //__SUPLA_DEV_V2__
