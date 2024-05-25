#include <iostream>

#include "client.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    std::string hostname(argv[1]);
    std::string service(argv[2]);
    Client client(hostname, service);
    client.executeActions();
    return client.status();
}
