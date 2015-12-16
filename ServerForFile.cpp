/*********************************************************************************
  *Copyright    Xuan Li, Shuo Zhang, Zeyu Ni
  *FileName:  server.cpp
  *Author:  Xuan Li, Shuo Zhang, Zeyu Ni
  *Version:  1.0
  *Date:  2015.12.3
  *Description:  // The server program can:
                        1: receive message from client program
                        2. transfer message from one client to another client
                        3. receive request from one client and broadcast this message to all client
                        4. receive file from one client and transfer this steam to another client
  *Others:  none
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <winsock2.h>
#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")  //Loading ws2_32.dll

#define BUF_SIZE 1024
using namespace std;

// creating  receive  message thread
 vector< Client > clients;
DWORD WINAPI ClientThread (LPVOID ssAccept)
    {
        SOCKET sAccept  = (SOCKET)ssAccept;
        //cout<<sAccept<<endl;
        int RET =  0;
        char RecvBuffer[BUF_SIZE];
         char sendvBuffer[BUF_SIZE];
        while(true){
        memset(RecvBuffer, 0x00, BUF_SIZE);
        RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
        if(RET == 0||RET == SOCKET_ERROR)
        {
            break;
        }
        cout<<"message is "<<RecvBuffer<<endl;

        send(clients[0].getClientSOCKET(), RecvBuffer, strlen(RecvBuffer), 0);  //send data to client
          memset(sendvBuffer, 0, BUF_SIZE);
        return 0;
    }

int main(){
    cout << "==========================================================\n";
    cout << "|This program is wirten by Xuan Li, Shuo Zhang, Zeyu Ni  |\n";
    cout << "|This program  using socket lib and tcp/ip portocol      |\n";
    cout << "==========================================================\n";

    //initializing DLL;
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);

    //Creating socket
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

    //Binding socket
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //filling every byte with 0
    sockAddr.sin_family = PF_INET;  //Using IPv4 address
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //the server's IP address is local host
    sockAddr.sin_port = htons(5000);  //setting port number
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //Going into listening state
    listen(servSock, 20);

    //Receiving the request from client
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE] = {0};  //Buffer Area
    HANDLE WINAPI hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);//open a thread for receiving message from server

    while(1){
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        int strLen = recv(clntSock, buffer, BUF_SIZE, 0);  //Receiving the data from client
        send(clntSock, buffer, strLen, 0);  //Sending back the data as its original version

        closesocket(clntSock);  //Closing socket
        memset(buffer, 0, BUF_SIZE); //Resetting buffer
    }
    while(3){
         // 定义客户端的socket地址结构client_addr，当收到来自客户端的请求后，调用accept
            // 接受此请求，同时将client端的地址和端口等信息写入client_addr中
            struct sockaddr_in client_addr;
            socklen_t          length = sizeof(client_addr);

            // 接受一个从client端到达server端的连接请求,将客户端的信息保存在client_addr中
            // 如果没有连接请求，则一直等待直到有连接请求为止，这是accept函数的特性，可以
            // 用select()来实现超时检测
            // accpet返回一个新的socket,这个socket用来与此次连接到server的client进行通信
            // 这里的new_server_socket代表了这个通信通道
            int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);
            if (new_server_socket < 0)
            {
                printf("Server Accept Failed!\n");
                break;
            }

            char buffer[BUFFER_SIZE];
            bzero(buffer, sizeof(buffer));
            length = recv(new_server_socket, buffer, BUFFER_SIZE, 0);
            if (length < 0)
            {
                printf("Server Recieve Data Failed!\n");
                break;
            }

            char file_name[FILE_NAME_MAX_SIZE + 1];
            bzero(file_name, sizeof(file_name));
            strncpy(file_name, buffer,
                    strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));

            FILE *fp = fopen(file_name, "r");
            if (fp == NULL)
            {
                printf("File:\t%s Not Found!\n", file_name);
            }
            else
            {
                bzero(buffer, BUFFER_SIZE);
                int file_block_length = 0;
                while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
                {
                    printf("file_block_length = %d\n", file_block_length);

                    // 发送buffer中的字符串到new_server_socket,实际上就是发送给客户端
                    if (send(new_server_socket, buffer, file_block_length, 0) < 0)
                    {
                        printf("Send File:\t%s Failed!\n", file_name);
                        break;
                    }

                    bzero(buffer, sizeof(buffer));
                }
                fclose(fp);
                printf("File:\t%s Transfer Finished!\n", file_name);
            }

            close(new_server_socket);
    }

    //Closing socket
    closesocket(servSock);

    //stopping the use of DLL
    WSACleanup();

    return 0;
}
