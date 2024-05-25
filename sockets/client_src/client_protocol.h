#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <arpa/inet.h>

#include "../common_src/socket.h"

class ClientProtocol {
private:
    Socket skt;
    // cppcheck-suppress unusedStructMember
    static const std::unordered_map<std::string, u_int8_t> translationMap;
    // cppcheck-suppress unusedStructMember
    bool was_close;
    // cppcheck-suppress unusedStructMember
    bool error_ocurred;
    // Swapea comandos por su respectivo codigo
    u_int8_t traslateMessengeToClientLanguage(std::string& command);

    // Lee la longitud del mensaje que envia el servidor
    u_int16_t recv2ByteNumber();

    std::vector<char> receiveString(uint16_t size);

    // Envia mensaje al servidor
    void sendCommandToServer(std::string& message);

    void sendFinalElement();

    void setRecivedErrorFlag(size_t len, size_t otherlen);

    bool setSendErrorFlag();

public:
    // Constructor de la clase ClientProtocol
    ClientProtocol(const std::string& hostname, const std::string& service);

    /// Envia la lista de comandos al servidor
    void sendCommandListToServer(std::list<std::string>& commmands);

    /// Recibe el string procesado por le servidor
    std::string receiveProcessedCommands();

    bool checkError();
    // Metodo destructor de la clase ClientProtocol
    ~ClientProtocol();
};
#endif
