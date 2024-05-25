#include <iostream>

#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    std::string puerto(argv[1]);
    Server server(puerto);
    server.accept();
    server.executeActions();
    return server.status();
}
