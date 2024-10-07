#include <unistd.h> //open
#include <fcntl.h> //open
#include <functional>
#include <iostream>
#include <thread>		// thread
#include <sys/socket.h> // socketpair

#include "reactor.hpp"

int success_count = 0;

void WriteFiveMoreLines(int fd, Reactor *reactor)
{
    static int count = 0;

    char buffer[1024] = "bye!";

    write(fd, buffer, 1024);

    ++count;
    ++success_count;
    ++success_count;

    if (count == 5)
    {
        reactor->Remove(fd, Reactor::READ);
        close(fd);
    }
}

void WriteFiveLines(int fd, Reactor *reactor)
{
    static int count = 0;

    char buffer[1024] = "hello!";

    write(fd, buffer, 1024);

    ++count;
    ++success_count;

    if (count == 5)
    {
        reactor->Remove(fd, Reactor::READ);
        reactor->Add(fd, Reactor::READ, std::bind(WriteFiveMoreLines, fd, reactor));
    }
}

void ReadTenLines(int fd, Reactor *reactor)
{
    static int count = 0;
    char buffer[1024] = {0};

    read(fd, buffer, 1024);

    std::cout << buffer << std::endl;

    ++count;
    ++success_count;

    if (count == 10)
    {
        reactor->Remove(fd, Reactor::WRITE);
        close(fd);
        reactor->Stop();
    }
}
    

void TestReadFile()
{
    Reactor reactor;

    int fds[2] = {0};

    pipe(fds);

    reactor.Add(fds[0], Reactor::WRITE, std::bind(ReadTenLines, fds[0], &reactor));
    reactor.Add(fds[1], Reactor::READ, std::bind(WriteFiveLines, fds[1], &reactor));

    reactor.Run();

    if (success_count == 25)
    {
        std::cout << "SUCCESS Read Pipe test" << std::endl;
    }
    else
    {
        std::cout << "Failed Read Pipe test " << success_count << std::endl;
    }

    success_count = 0;
}

//TEST 2
void Sock1Write(int fd, Reactor *reactor)
{
    std::string buffer;

    std::getline(std::cin, buffer);

    write(fd, buffer.c_str(), buffer.size());

    reactor->Remove(fd, Reactor::READ);

    ++success_count;
}

void Sock2Read(int fd)
{
    char buffer[1024] = {0};

    read(fd, buffer, 1024);

    std::string out = std::string(buffer) + " viewed";

    write(fd, out.c_str(), out.size());

    ++success_count;
}

void Sock1Read(int fd, Reactor *reactor)
{
    static int count = 0;
    char buffer[1024] = {0};

    read(fd, buffer, 1024);

    std::cout << "recieved: \"" << buffer << "\"" << std::endl;

    ++count;
    ++success_count;

    reactor->Add(fd, Reactor::READ, std::bind(Sock1Write, fd, reactor));

    if (count == 5)
    {
        reactor->Stop();
    }
}



void TestSockets()
{
    Reactor reactor;

    std::cout << "Write 5 msgs: " << std::endl;

    // Setup for Write test (multiple writes)
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) 
	{
        std::cout << "SOCKET FAILED" << std::endl;
    }

    reactor.Add(sockets[1], Reactor::READ, std::bind(Sock1Write, sockets[1], &reactor));
    reactor.Add(sockets[0], Reactor::WRITE, std::bind(Sock2Read, sockets[0]));
    reactor.Add(sockets[1], Reactor::WRITE, std::bind(Sock1Read, sockets[1], &reactor));

    reactor.Run();

    close(sockets[0]);
    close(sockets[1]);

    if (success_count == 15)
    {
        std::cout << "SUCCESS Read Write socket test" << std::endl;
    }
    else
    {
        std::cout << "Failed Read Write socket  test" << std::endl;
    }
}


int main()
{
    TestReadFile();
    TestSockets();

    return 0;
}




