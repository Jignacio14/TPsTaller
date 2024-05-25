#ifndef RESOLVER_H
#define RESOLVER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../common_src/socket.h"

class ServerProtocol {
private:
    Socket skt;
    // cppcheck-suppress unusedStructMember
    static const std::unordered_map<u_int8_t, std::string> translationMap;
    // cppcheck-suppress unusedStructMember
    bool was_close;
    // cppcheck-suppress unusedStructMember
    bool error_ocurred;
    // decodifica el mensaje recibido del cliente
    std::string decode(u_int8_t code);

    void sendString(const std::vector<char>& data, u_int16_t size);

    void send2ByteNumber(u_int16_t len);

    void sendStringData(const std::vector<char>& data, u_int16_t size);

    void setErrorFlag(bool condition);

public:
    // Constructor de la clase ServerProtocol
    explicit ServerProtocol(const std::string& port);

    // Deja el servirdor escuchando al cliente esperando su mensaje
    std::string receive();

    void accept();

    bool isRunning();

    void sendCommand(u_int16_t len, const std::vector<char>& data);

    bool checkError();

    // Metodo destructor de la clase ServerProtocol
    ~ServerProtocol();
};
#endif
