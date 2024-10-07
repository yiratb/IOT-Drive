#include <iostream> //cout, cin

#include "logger.hpp" //logger

#define RESET "\033[0m"
#define GREEN   "\033[0;32m"

static void Test1();

int main()
{
    Test1();
    std::cout << GREEN << "PASS" << RESET <<std::endl;
    
    return 0;
}

static void Test1()
{
    std::string path("filename");
    Logger *logger = Singleton<Logger, const std::string&>::GetInstance(path);
    logger->Log("first", Logger::INFO);
    logger->Log("hello", Logger::INFO);
    logger->Log("my name is yirat", Logger::ERROR);
    logger->Log("check", Logger::DEBUG);
    logger->Log("i am sooo tired", Logger::DEBUG);
}