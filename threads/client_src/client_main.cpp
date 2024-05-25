#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include <sys/types.h>

#include "client.h"

int main(int argc, const char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Argument number Err" << std::endl;
            return -1;
        }
        const std::string hostname = argv[1];
        const std::string port = argv[2];
        Client client(hostname, port);
        client.run();
        return 0;
    } catch (const std::runtime_error&) {
        std::cerr << "Communication was unexpectedly closed " << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknowed Error ocurred during excecution " << std::endl;
    }
}
