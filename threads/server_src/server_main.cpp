#include <iostream>

#include "server.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Argument number Err" << std::endl;
            return -1;
        }
        std::string servername(argv[1]);
        Server server(servername);
        server.executeServer();
        return 0;
    } catch (const std::runtime_error&) {
        std::cerr << "Communication was unexpectedly closed " << std::endl;
    } catch (...) {
        std::cerr << "Unknowed Error ocurred during excecution " << std::endl;
    }
    return -1;
}
