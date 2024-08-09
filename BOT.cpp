#include "server.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>

void Server::parseQuestionsFile()
{
    if (botQuestions.size() == 0)
    {
        std::ifstream file;
        file.open("Q&A", std::istream::in);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line, '\n'))
            {
                std::istringstream ss(line);
                std::vector<std::string> elements;
                std::string s;
                while (std::getline(ss, s, '|'))
                    elements.push_back(s);
                botQuestions.push_back(std::make_pair(elements[0], elements[1]));
            }
        }
    }
}

int Server::isCorrectAnswer(std::string question, std::string answer)
{
    for (size_t i = 0; i < botQuestions.size(); i++)
    {
        if (botQuestions.at(i).first == question)
        {
            if (botQuestions.at(i).second == answer)
                return 1;
        }
    }
    return (0);
}

std::string Server::generateQuestion(std::vector<std::string> &answerdQuestions)
{
    std::string s = "";
    while (true)
    {
        int randomNum = rand() % (botQuestions.size());
        std::vector<std::string>::iterator it = \
        std::find(answerdQuestions.begin(), answerdQuestions.end(), botQuestions[randomNum].first);
        if (it ==  answerdQuestions.end())
            return botQuestions[randomNum].first;
        if (answerdQuestions.size() == botQuestions.size())
            return s;
    }
}

void Server::bot(Client *client)
{
    parseQuestionsFile();
    char buffer[1024];
    sendData(client->clientSocket, "QUIZZ GAME, Press ENTER to continue\n");
    bool started = false;
                std::string question;
    // bool answerd = false;
    bool questionGenerated = false;
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int r = recv(client->clientSocket, buffer, sizeof(buffer), 0);
        if (r > 0)
        {
            std::string input(buffer);
            input = input.substr(0, input.size() - 1);
            if (started == false)
            {
                sendData(client->clientSocket, "The game is about picking the right answer in each question, with every correct answer you earn 5xps\n");
                sendData(client->clientSocket, "Choose the right answer based on it's number\n");
                sendData(client->clientSocket, "Press ENTER to continue");
                started = true;
            }
            else if (started)
            {
                if (questionGenerated == false)
                {
                    question = generateQuestion(client->answerdQuestions);
                    if (question.empty())
                    {
                        sendData(client->clientSocket, "You answerd all the questions\n");
                        sendData(client->clientSocket, "Your result: ");
                        sendData(client->clientSocket, to_string(client->totalXps) + "Xps.");
                        return ;
                    }
                    questionGenerated = true;
                    sendData(client->clientSocket, "\n" + question + "\n");
                    sendData(client->clientSocket, "Your answer: ");
                }
                if (!input.empty())
                {
                    if (isCorrectAnswer(question, input))
                    {
                        sendData(client->clientSocket, "Correct! +5xp\nPress ENTER to move to next question");
                        client->answerdQuestions.push_back(question);
                        client->totalXps += 5;
                    }
                    else
                        sendData(client->clientSocket, "Wrong!\nPress ENTER to move to next question");
                    questionGenerated = false;
                }
            }
            else if (input == "finish")
            {
                sendData(client->clientSocket, "finished\n");
                return ;
            }
        }
    }
}