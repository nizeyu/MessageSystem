/*********************************************************************************
  *Copyright    Xuan lI
  *FileName:  server.cpp
  *Author:  Xuan Li
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
#include <winsock2.h>
#include <iostream>
#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll

#define BUF_SIZE 100


using namespace std;
// create  receive  message thread
DWORD WINAPI ClientThread (LPVOID ipParameter)
    {
        SOCKET ClientScoket  = (SOCKET)ipParameter;
        int RET =  0;
        char RecvBuffer[100];
         char sendvBuffer[100];
        while(true){
        memset(RecvBuffer,0x00,sizeof(RecvBuffer));
        RET  =  recv(ClientScoket,RecvBuffer,100,0);
        if(RET == 0||RET == SOCKET_ERROR)
        {
            cout<<"failed,exit"<<endl;
            break;
        }
        cout<<"message is "<<RecvBuffer<<endl;
        printf("Input a string: ");
        gets(sendvBuffer);
        send(ClientScoket, sendvBuffer, strlen(sendvBuffer), 0);  //send data to client
    }

    return 0;
}
int main(){
    printf("==========================================================\n");

    printf("|This program is wirten by Xuan Li, Zeyu Ni, Shuo Zhang  |\n");
    printf("|This program  using socket lib and tcp/ip portocol      |\n");
    printf("==========================================================\n");
    WSADATA wsaData;
       HANDLE      hThread =  NULL;

        if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
        {
            printf("Winsock load failed\n");
            return 0;
        }
        //create socket
        SOCKET        servSock,sAccept;
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
    sockAddr.sin_addr.s_addr =INADDR_ANY;
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
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    char buffer[BUF_SIZE] = {0};  //create buffer area
     cout<<"the server has running"<<endl;
    while(1){
       sAccept = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
       if(sAccept==INVALID_SOCKET)
		{
			printf("accept()failed:%d\n",WSAGetLastError());
			break;
		}
		printf("accepted client IP:[%s],port:[%d]\n",inet_ntoa(sockAddr.sin_addr),ntohs(sockAddr.sin_port));
		  hThread  = CreateThread(NULL,0,ClientThread,(LPVOID)sAccept,0,NULL);
		     if(hThread == NULL)
        {
           cout<<"creat thread failed"<<endl;
           break;
        }
       CloseHandle(hThread);
      //  int strLen = recv(sAccept, buffer, BUF_SIZE, 0);  //
      //  printf("Message form client: %s\n", buffer);
      //  printf("Input a string: ");
       // gets(buffer);
        //send(sAccept, buffer, strlen(buffer), 0);  //send data to client

        closesocket(sAccept);
//close socket
        memset(buffer, 0, BUF_SIZE);  //reset buffer area
    }
//close socket
    closesocket(servSock);
 //stop using dll
    WSACleanup();

    return 0;
}
