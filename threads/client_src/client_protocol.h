#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_
#include <atomic>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>

#include "../common_src/common_message.h"
#include "../common_src/liberror.h"
#include "../common_src/socket.h"

class ClientProtocol {
private:
    Socket skt;
    // cppcheck-suppress unusedStructMember
    bool was_close;

    bool checkSocketCloseSatus();

    u_int8_t receiveByteCode();

    std::pair<uint16_t, uint16_t> receive2BytesArray();

    void throwCommunicationError();

    bool _sendAttack();

    MessageFormat _listenSocketData();

public:
    //
    explicit ClientProtocol(const std::string& hostname, const std::string& servername);

    bool sendAttack();

    // devuelve el string del mensaje recibido
    MessageFormat listenSocketData();
    bool wasClosed();

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(ClientProtocol&&) = default;

    ~ClientProtocol();
};
#endif
