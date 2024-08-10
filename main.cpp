#include "client.hpp"
#include <cstdlib>
#include <sys/signal.h>
#include "server.hpp"
// #include <poll.h>

int main(int ac , char** av)
{
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <port>" << " <password>"<< std::endl;
        exit(1);
    }
    if(!isdigit(*av[1]))
    {
        std::cerr << "Invalid port number" << std::endl;
        exit(1);
    }
    Server server(atoi(av[1]), av[2]);
    server.run_server();
}