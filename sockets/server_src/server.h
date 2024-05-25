#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>

#include "server_protocol.h"

class Server {
private:
    // cppcheck-suppress unusedStructMember
    unsigned int counter;
    // cppcheck-suppress unusedStructMember
    bool is_running;
    ServerProtocol servprot;
    // cppcheck-suppress unusedStructMember
    int finalStatus;

    std::string matchCombos(const std::string& commands);

    std::string replaceString(const std::string& original, const std::string& target,
                              const std::string& replacement);

    void countCombos(const std::string& combos);

    // Revisa que la conexion se mantenga
    bool isRunning();

    // Procesa un mensaje enviado por el cliente y lo transforma al formato servidor
    std::string asyncGet();

    // Envia el string previamente procesado devuelta al cliente
    void returnProccesedCommand(std::string& command);

    // Imprime la cantidad de acciones enviadas por el cliente
    void printActions();


public:
    explicit Server(std::string& puerto);

    // Acepta la primera conexion que le solicita unirse
    void accept();

    // Devuelve el estado final del servidor
    int status();

    // Ejecuta todas las acciones del servidor
    void executeActions();

    // Destructor de server
    ~Server();
};
#endif
