//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_MY_SOCKET_H
#define MRAVECSEMESTRALKA_MY_SOCKET_H


#include <winsock2.h>
#include <string>

class MySocket {
public:
    static MySocket *createConnection(std::string hostName, short port);

    ~MySocket();

    void sendData(const std::string &data);

    void sendEndMessage();

protected:
    MySocket(SOCKET socket);

private:
    static const char *endMessage;
    SOCKET connectSocket;
};


#endif //MRAVECSEMESTRALKA_MY_SOCKET_H
