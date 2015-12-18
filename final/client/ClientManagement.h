#ifndef CLIENTMANAGEMENT_H
#define CLIENTMANAGEMENT_H

#include <stdio.h>
#include <WinSock2.h>                          //for socket
#include <windows.h>
#include <iostream>                            //for input and output
#include <string>                              // for memset
#include <stdlib.h>                             // for exit
#include <vector>                              // for Client
#include"Client.h"
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")  //loading  ws2_32.dll
#define BUF_SIZE             1024

/**
 * @Class：ClientManagement
 * @description: All data object and method object are definited in this Class.
                 The data object include: IP, port, socket, Client, buffer, thread.
                 The method object in this class can complete the initialization, connection between client and server,
                 users log in, sending message, receiving message, broadcast, closing program, etc.
                 These methods can be realized in "client.cpp".
 * @author: Xuan Li, Zeyu Ni, Shuo Zhang
 * @version: 1.0,
 * @date: 12/3/2015
 */
class ClientManagement
{
private:
    char* ipAddress;//object: IP address
    int port;//object: port numner
    SOCKET sock;//object in socket type
    Client clientO;//object of Client class in "Client.h"
    char bufSend[BUF_SIZE];//object: sending buffer
    HANDLE WINAPI hThread;//Tread definition
public:
    sockaddr_in sockAddr;
    ClientManagement(int pt,  char* ipAddres);//constructor
    void initialization();//definition initialization method
    void connection();//definition connection method
    char* login(char* username);//definition login method
    void close();//definition close method
    void sendMessage(char* userlist);
    void boradcast();//definition boradcast method
    int transferFile(char* userlist);

    void receiveMessage();//definition receiveMessage method
    virtual ~ClientManagement();//definition ~ClientManagement method for stopping chat

    //function for Get and set ip address
    char* GetipAddress() {
        return ipAddress;
    }
    void SetipAddress(char* val) {
        ipAddress = val;
    }
    //function for Get and set port
    int Getport() {
        return port;
    }
    void Setport(int val) {
        port = val;
    }
    //function for Get and set socket
    SOCKET Getsock() {
        return sock;
    }
    void Setsock(SOCKET val) {
        sock = val;
    }
    //function for Get and set client
    Client GetClient() {
        return clientO;
    }
    void SetClient(Client val) {
        clientO = val;
    }
};
#endif // CLIENTMANAGEMENT_H
