#include "client.hpp"
#include <cstdlib>
#include <sys/signal.h>
#include "server.hpp"



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
    if(std::string(av[2]).find(" ") != std::string::npos || std::string(av[2]).empty())
    {
        std::cerr << "Invalid password" << std::endl;
        exit(1);
    }
    Server server(atoi(av[1]), av[2]);
    server.run_server();
}