#include "SuplaDeviceV2.h"

#include <Client.h>

#include "srpc.h"

_supla_int_t SuplaClient::read(void *buf, _supla_int_t count) {
    mClient->available();
    return 0;
}

_supla_int_t SuplaClient::read(void *buf, _supla_int_t count, void *userData) {
    SuplaClient *cli = reinterpret_cast<SuplaClient*>(userData);
    if (cli)
        return cli->read(buf, count);

    return -1;
}


_supla_int_t SuplaClient::write(void *buf, _supla_int_t count) {
    return 0;
}

_supla_int_t SuplaClient::write(void *buf, _supla_int_t count, void *userData) {
   SuplaClient *cli = reinterpret_cast<SuplaClient*>(userData);
    if (cli)
        return cli->write(buf, count);

    return -1;
}

void SuplaClient::onRemoteCallReceived(void *_srpc, unsigned _supla_int_t rr_id, unsigned _supla_int_t call_type, void *userData, unsigned char proto_version) {
    SuplaClient *cli = reinterpret_cast<SuplaClient*>(userData);
    if (cli)
        return cli->onRemoteCallReceived(_srpc, rr_id, call_type, proto_version);

    return;
}

void SuplaClient::onRemoteCallReceived(void *_srpc, unsigned _supla_int_t rr_id, unsigned _supla_int_t call_type, unsigned char proto_version) {
    TsrpcReceivedData rd;
    char result;

    supla_log(LOG_DEBUG, "Data received. CallType:%i ProtoVer:%i", call_type, proto_version);

    result = srpc_getdata(_srpc, &rd, 0);
    if (result != SUPLA_RESULT_TRUE) {
        supla_log(LOG_ERR, "Failed to get data from server. Ret: %i", result);
        return;
    }

    switch (rd.call_type) {
        case SUPLA_SDC_CALL_VERSIONERROR:

            break;

        case SUPLA_SD_CALL_REGISTER_DEVICE_RESULT:

            break;

        case SUPLA_SD_CALL_CHANNEL_SET_VALUE:

            break;

        case SUPLA_SDC_CALL_SET_ACTIVITY_TIMEOUT_RESULT:

            break;

        default:
            supla_log(LOG_DEBUG, "Unsupported call type");
            break;
    }

    srpc_rd_free(&rd);
}

SuplaClient::SuplaClient(Client *client) : mClient(client), mSrpc(NULL) {

    memset(&mSuplaDevC, 0, sizeof(TDS_SuplaRegisterDevice_C));

		setState(State::UNINITIALIZED);
}

int SuplaClient::begin() {
    TsrpcParams params;

    srpc_params_init(&params);
    params.user_params = this;
    params.data_read = read;
    params.data_write = write;
    params.on_remote_call_received = onRemoteCallReceived;

    mSrpc = reinterpret_cast<Tsrpc*>(srpc_init(&params));
    if (mSrpc == NULL) {
				setState(State::UNINITIALIZED):
        return -1;
		}

		setState(State::INITIALIZED);

    return 0;
}

int SuplaClient::addChannel(SuplaChannel *channel) {

		if (getState() != State::UNINITIALIZED) {
			supla_log(LOG_INFO, "Device already initialized");
			return -1;
		}

		if (mSuplaDevC.channel_count == SUPLA_CHANNELMAXCOUNT) {
			supla_log(LOG_ERR, "MAXCHANNELCOUNT reached");
			return -1;
		}

		mSuplaDevC.channels[mSuplaDevC.channel_count].Number = mSuplaDevC.channel_count;

		channel->setClient(this);
		channel->setNumber(mSuplaDevC.channel_count);

		mSuplaDevC.channel.Number++;

    return -1;
}
