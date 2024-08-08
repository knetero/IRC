#ifndef SERVER_HPP
#define SERVER_HPP

#include <iterator>
#include <string>
#include <algorithm>
#include <cstdio>
#include <errno.h>  
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
        struct sockaddr_in serverAddress;
        struct sockaddr_in clientAdress;
        int                clientSocket;
        std::map<int , Client *>            serverClients;
        std::map<std::string, Channel *>    server_channels;
        char    *startdate;
        // Server(const Server& other);
        // Server& operator=(const Server& other);
        ~Server();

        std::vector<struct pollfd> clientSockets;
        int serverSocket; 
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

        std::vector<std::pair<std::string, std::string> > botQuestions;

        void run_server(void);
        bool initialize();
        void acceptClient();
        void ReceiveData(size_t i);
        static void signalHandler(int signum);
        void send_private_message(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void parse_commands(Client *client, std::string& data);
        void notifyClients(int clientSocket, std::map<int, Client>& clients_Map, std::string oldNick);
        std::string toUpperCase(std::string& str);
        void check_Quit(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void check_user(int clientSocket, const std::string&data , std::map<int, Client>& clients_Map, Client& client);
        bool check_Nick(int clientSocket, std::string value,  std::map<int, Client>& clients_Map);
    
        void                        welcomeMessage(int fd, Client *client);
        int                         get_nick(std::string chName, std::string nickname);
        int                         check_properties(Channel *channel, std::string mdp, Client *client);
        void                        sendData(int fd, std::string data);
        void                        broadcastToChannels(std::string nickname, Client *c);
        std::string                 getIp(struct sockaddr_in addr);
        std::string                 getServerIp(void);
        int                         getClientFd(std::string target);
        void                        broadcastToChannel(Channel &channel, Client *kicker, std::string target, std::string comment);
        void                        getDate(char **buf);
        std::vector<std::string>    splitParameters(std::string parametersString);
        void send_info(Channel &chName, std::string msg);
        int channelExist(std::string channelName);
        void broadcastTopic(Channel *channel, Client *client, std::string topic);
        void send_info(Channel *chName, std::string msg);


        void bot(Client *client);
        void parseQuestionsFile();
        int isCorrectAnswer(std::string question, std::string answer);
        std::string generateQuestion(std::vector<std::string> &answerdQuestions);
        // Commands
        void                        passCommand(Client *client, std::vector<std::string> &parameters);
        void                        nickCommand(Client *client, std::vector<std::string> &parameters);
        void                        userCommand(Client *client, std::vector<std::string> &parameters);
        void                        privmsgCommand(Client *client, std::vector<std::string> &parameters);
        void                        kickCommand(Client *client, std::vector <std::string> &parameters);
        void                        inviteCommand(Client *client, std::vector<std::string> &parameters);
        void                        topicCommand(Client *client, std::vector<std::string> &parameters);
        void                        join(std::string value, Client *client);
        void                        mode(std::string value, Client *client);
        //
};

#endif
