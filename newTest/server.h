#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include<string>
#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll


#define BUF_SIZE 100
class Server
{
private:
   SOCKET   servSock,sAccept;
   int port;
   int clinetNumber;
public:
        sockaddr_in clntAddr;
        Server(){
                port = 0;
                clinetNumber  = 0;
        }
        void printCopyRight(){
            printf("==========================================================\n");
            printf("|This program is wirten by Xuan Li, Zeyu Ni, Shuo Zhang  |\n");
            printf("|This program  using socket lib and tcp/ip portocol      |\n");
            printf("==========================================================\n");
        }
        int  loadWSAD(){
            WSADATA wsaData;
            HANDLE WINAPI  thread[255];    //thread
            DWORD WINAPI dwThId[255];    // thread id number
            int index=0;
            if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
                {
                printf("Winsock load failed\n");
                return 0;
                }
                return 1;
            }


            int initialize(int portNumber){
                servSock = socket(AF_INET, SOCK_STREAM, 0);
                if(servSock==INVALID_SOCKET)
                {
                    printf("socket failed:%d\n",WSAGetLastError());
                    return 0;
                }
                //bind socket
            sockaddr_in sockAddr;
            memset(&sockAddr, 0, sizeof(sockAddr));
            sockAddr.sin_family = AF_INET;  //using ipv4 adress
            sockAddr.sin_addr.s_addr =INADDR_ANY;// local
            sockAddr.sin_port = htons(5000);  //set the port
            if( bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR))==SOCKET_ERROR)
            {
                printf("bind()failed:%d\n",WSAGetLastError());
                return 0;
            }
            //begin listening
            if(listen(servSock,20)==SOCKET_ERROR)
            {
                printf("listen()failed:%d\n",WSAGetLastError());
                    return 0;
            }//listen and judge weather it can work
            //revieve the request from client
            //4,waiting and accept client connect request...


            }
         ~Server() {

         }

};

#endif // SERVER
