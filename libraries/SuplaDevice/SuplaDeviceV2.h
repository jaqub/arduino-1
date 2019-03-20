#ifndef __SUPLA_DEV_V2__
#define __SUPLA_DEV_V2__

#include <list>
#include "proto.h"

class SuplaClient;
class Client;
struct Tsrpc;

class SuplaChannel {

private:
    unsigned char mChannelNum;

    _supla_int_t mType;

    SuplaClient *mSuplaClient;

    void setClient(SuplaClient *client) { mSuplaClient = client; }

    void setNumber(unsigned char num) { mChannelNum = num; }

public:
    SuplaChannel(_supla_int_t type) { mType = type; }

    virtual ~SuplaChannel() { }

    void setType(_supla_int_t type) { mType = type; }

    _supla_int_t getType() { return mType; }

    unsigned char getChannelNum() { return mChannelNum; }

    virtual int setValue(char value);

    virtual int setValue(double value);

    virtual int onRemoteValueChange(TSD_SuplaChannelNewValue *value) = 0;
};


class SuplaClient {
public:
    enum State {
        UNINITIALIZED = 0,
        INITIALIZED,
        REGISTERING,
        REGISTERED,
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
    };


private:
    Client *mClient;

    TDS_SuplaRegisterDevice_C mSuplaDevC;

    Tsrpc *mSrpc;

    State mState;

    std::list<SuplaChannel*> mChannels;

    void setState(SuplaClient::State state) { mState = state; }

    _supla_int_t read(void *buf, _supla_int_t count);

    static _supla_int_t read(void *buf, _supla_int_t count, void *userData);

    _supla_int_t write(void *buf, _supla_int_t count);

    static _supla_int_t write(void *buf, _supla_int_t count, void *userData);

    void onRemoteCallReceived(void *_srpc, unsigned _supla_int_t rr_id, unsigned _supla_int_t call_type, unsigned char proto_version);

    static void onRemoteCallReceived(void *_srpc, unsigned _supla_int_t rr_id, unsigned _supla_int_t call_type, void *userData, unsigned char proto_version);

public:
    SuplaClient(Client *client);

    virtual ~SuplaClient() { }

    SuplaClient::State getState() { return mState; }

    int begin();

    int addChannel(SuplaChannel *channel);

    int iterate();
};

#endif //__SUPLA_DEV_V2__
