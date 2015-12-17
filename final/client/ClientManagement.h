#ifndef CLIENTMANAGEMENT_H
#define CLIENTMANAGEMENT_H

#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <string>                              // for memset
#include <stdlib.h>                             // for exit
#include <vector>
#include"Client.h"
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")  //loading  ws2_32.dll
#define BUF_SIZE             1024
class ClientManagement
{
private:
        char* ipAddress;
        int port;
        SOCKET sock;
        Client clientO;
        char bufSend[BUF_SIZE];
        HANDLE WINAPI hThread;
public:
        sockaddr_in sockAddr;
        ClientManagement(int pt,  char* ipAddres);
        void initialization();
        void connection();
        char* login(char* username);
        void close();
        void sendMessage(char* userlist);
        void boradcast();
        void receiveMessage();
        virtual ~ClientManagement();

        char* GetipAddress() {
            return ipAddress;
            }
        void SetipAddress(char* val) {
            ipAddress = val;
            }
        int Getport() {
             return port;
              }
        void Setport(int val) {
            port = val;
             }

        SOCKET Getsock() {
            return sock;
             }
        void Setsock(SOCKET val) {
            sock = val;
            }
        Client GetClient() {
            return clientO;
            }
        void SetClient(Client val) {
            clientO = val;
            }

};

#endif // CLIENTMANAGEMENT_H
