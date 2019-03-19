#include "SuplaDeviceV2.h"


SuplaDev::SuplaDev() : mSrpc(NULL) {

    memset(&mSuplaDev, 0, sizeof(TDS_SuplaRegisterDevice_C));
}

int SuplaDev::begin() {
    TsrpcParams params;

    srpc_params_init(&params);
    params.user_params = this;
    params.data_read = NULL;
    params.data_write = NULL;
    params.on_remote_call_received = NULL;

    mSrpc = srpc_init(&params);
    if (mSrpc == NULL)
        return -1;

    return 0;
}
