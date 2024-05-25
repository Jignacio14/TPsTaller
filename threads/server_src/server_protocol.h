#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <netinet/in.h>

#include "../common_src/common_message.h"
#include "../common_src/liberror.h"
#include "../common_src/socket.h"
class ServerProtocol {
private:
    Socket skt;
    // cppcheck-suppress unusedStructMember
    bool was_close;

    void prepareServerMsj(const MessageFormat& msj);
    void sendGameStatus(uint16_t alive, uint16_t death, uint8_t event);
    std::array<uint16_t, 2> prepare2BytesNumbers(uint16_t alive, uint16_t death);
    void sendHeader();
    std::string _receiveClientAttack();
    void throwCommunicationError();

public:
    explicit ServerProtocol(Socket skt);

    /// Envia el estado del juego y el evento definido
    void sendGameStatusMsj(const MessageFormat& msj);

    std::string receiveClientAttack();
    bool isClosed();
    void endComunication();

    ~ServerProtocol();
};
#endif
