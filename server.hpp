#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <map>
#include "client.hpp"
#include <poll.h>
#include <string>
#include "client.hpp"
#include <fcntl.h>
#include <sstream>
#include "replies.hpp"
#include "channel.hpp"

#define RESET "\033[0m"
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */

#define BUFFER 1024
class Client;
class Channel;
class Server {
    public:
        Server(int port, const std::string& password);
        std::map<std::string, Channel > server_channels;
        std::map<int, struct sockaddr_in> adresses;
        struct sockaddr_in serverAddress;
        std::map<int , Client *> serverClients;
        // Server(const Server& other);
        // Server& operator=(const Server& other);
        ~Server();

        std::vector<struct pollfd> clientSockets;
        int serverSocket; 
        int socketId;
        int port; 
        std::string password;
        bool isRunning;
        bool isServerCommand;
        char buffer[BUFFER];
        
        bool isSetNick;
        bool isSetPass;
        bool isSetUser;
        bool isRegistred;
        static bool signal;


        bool initialize();
        bool sendMessage(int fd, const std::string& message);
        int acceptClient();
        static void signalHandler(int signum);
        bool isCommand(const std::string& data);
        void send_private_message(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void parse_commands(Client *client, const std::string& data);
        bool sendError(int clientSocket, const std::string& errorMessage);
        void notifyClients(int clientSocket, std::map<int, Client>& clients_Map, std::string oldNick);
        std::string toUpperCase(std::string& str);
        std::vector<std::string> split(const std::string &s, char delim);
        void check_Quit(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        // void sendWelcomeMessages(int clientSocket, std::map<int, Client>& clients_Map);
        void check_user(int clientSocket, const std::string&data , std::map<int, Client>& clients_Map, Client& client);
        bool check_Nick(int clientSocket, std::string value,  std::map<int, Client>& clients_Map);
    
        // void join(std::string value, int clientsocket, std::map<int, Client>& clients_Map);
        // void mod(std::string value, int clientsocket, std::map<int, Client>& clients_Map);
        int get_nick(std::string chName, std::string nickname);

        int check_properties(Channel channel, std::string mdp, Client *client);
        //
        void sendData(int fd, std::string data);
        void passCommand(Client *client, std::vector<std::string> &parameters);
        void nickCommand(Client *client, std::vector<std::string> &parameters);
        void userCommand(Client *client, std::vector<std::string> &parameters);
        // void privmsgCommand(Client *client, std::vector<std::string> &parameters);
        void kickCommand(int fd, std::vector <std::string> &parameters);
        void broadcastToChannels(int fd, std::string nickname, Client &c);
        void welcomeMessage(int fd, Client &client);
        std::string getIp(struct sockaddr_in addr);
        std::string getServerIp(void);
        int getClientFd(std::string target);
        void broadcastToChannel(Channel &channel, int operatorFd, std::string target, std::string comment);

        //
        void join(std::string value, Client *client);
        // void set_mode(std::vector<std::string> & args, char mode, std::map<int, Client >&  clients_Map, std::string ss);
        void mod(std::string value, Client * client);
        void send_info(Channel chName, std::string msg);
        // int get_nick(std::string chName, std::string nickname);
};

#endif
