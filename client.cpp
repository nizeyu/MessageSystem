/*********************************************************************************
  *Copyright    Xuan Li, Shuo Zhang, Zeyu Ni
  *FileName:  client.cpp
  *Author:  Xuan Li, Shuo Zhang, Zeyu Ni
  *Version:  1.0
  *Date:  2015.12.2
  *Description:  // The server program can:
                        1. sent message to server
                        2. sent broadcasting request with messasge to server
                        4. sent file to sevrver and request for resenting to another client
  *Others:  none
**********************************************************************************/
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
#define FILE_NAME_MAX_SIZE    512
using namespace std;

int  splitString(const string & strSrc, const std::string& strDelims, vector<string>& strDest)
{
typedef std::string::size_type ST;
string delims = strDelims;
std::string STR;
if(delims.empty()) delims = "\n\r";


ST pos=0, LEN = strSrc.size();
while(pos < LEN ){
STR="";
while( (delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN) ) ++pos;
if(pos==LEN) return strDest.size();
while( (delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN) ) STR += strSrc[pos++];
if( ! STR.empty() ) strDest.push_back(STR);
}
return strDest.size();
}

HANDLE hMutex;
DWORD WINAPI Rec(LPVOID ipParameter)
{
	SOCKET sockClient = (SOCKET)ipParameter;
	char bufRecv[BUF_SIZE] = {0};//creating receiving buffer

	int byte = 0;
	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		memset(bufRecv, 0, BUF_SIZE);
		byte = recv(sockClient, bufRecv, BUF_SIZE , 0);//receiving the data from Server

		cout << "Message form server:" << bufRecv << '\n';
		if (byte <= 0)
		{
			break;
		}
		memset(bufRecv, 0, BUF_SIZE);  //resetting the buffer area
		ReleaseMutex(hMutex);
	}
	printf("thread closed");
	closesocket(sockClient);//closing socket for Client
	return 0;
}

int main(){
    cout << "==========================================================\n";
    cout << "|This program is wirten by Xuan Li, Shuo Zhang, Zeyu Ni  |\n";
    cout << "|This program  using socket lib and tcp/ip portocol      |\n";
    cout << "==========================================================\n";
    //initializing DLL;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //Originating the request to server
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));//filling every byte with 0
    sockAddr.sin_family = AF_INET;//Using IPv4 address
    sockAddr.sin_addr.s_addr = inet_addr("155.246.46.30");// the server's IP address is local host
    sockAddr.sin_port = htons(5000);//setting port number

    char bufSend[BUF_SIZE] = {0};//creating sending buffer

    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//creating socket
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));//connecting

    //vector<char*> Namelist;    //user list which is login to server
    cout << "Please Input your username:  ";//Asking for Inputing your Username
    gets(bufSend);//Getting client username from Client and Sending to Server

    send(sock, bufSend, strlen(bufSend), 0);//login in
    memset(bufSend, 0, BUF_SIZE);
		int rec;
		char bufRecv1[BUF_SIZE] = { 0 };//creating receiving buffer
		rec = recv(sock, bufRecv1, BUF_SIZE, 0);
          cout << bufRecv1;
        //now we need split it into different short string

    int n;
    cout << "Please input a number\n1-Send Message to other user;\n2-Sent Broadcast Request;\n3-Send Files\nPlease input:";
    cin >> n;
    /*
	1.ask to input username and send to server.
	2.receive a clientnumber from server
	3.input 1/2/3 represent send message or ask for broad cast or file transfer
	while(2)
	while(3)
	*/HANDLE WINAPI hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);//open a thread for receiving message from server
    if(n==1){

      cout << "The usernames who are online now: \n";


        const char * split = "; ";
        vector<string> splitStrs;
        splitString(bufRecv1,split, splitStrs);
        vector<string>::iterator iter;
        int temp1 = 0;
        for (iter = splitStrs.begin(); iter != splitStrs.end(); ++iter) {
                   if(temp1 %2==0){
                         cout  <<*iter <<":";
                    }else{
                        cout  <<*iter << endl;
                    }
                    temp1++;
            }
            bool flag = false;
            while(!flag){
            cout << "Choose a name you want to chat with: (if you want to exit please input 'exit')\n ";
            //gets(bufSend);//Getting chat friend name from Client and Sending to Server
            cin >> bufSend;
            if(bufSend=="exit"){
                return 0;
            }
            int a=atoi(bufSend);
            if(a>temp1||a<0){
            flag = false;
                }else{
            flag = true;
    }
}

    send(sock, bufSend, strlen(bufSend), 0);

    //users choosing the different works
    while(1){
        cout << "Input a string(input 'quit' to quit): \n ";
        gets(bufSend);//getting data from client and sending to server
        if (bufSend[0] == 'q'&&bufSend[1] == 'u'&&bufSend[2] == 'i'&&bufSend[3] == 't') { break; }//user input quit to quit the message system
        send(sock, bufSend, strlen(bufSend), 0);

        cout << "accepted client IP:" << inet_ntoa(sockAddr.sin_addr) << '\t';
        cout << "port:" << ntohs(sockAddr.sin_port)  << '\n';
     closesocket(sock);  //closing socket
    }
   }
   if(n==2){
        cout << "Input a string(input 'quit' to quit): \n ";
        gets(bufSend);//getting data from client and sending to server
      //  if (bufSend[0] =="quit") {
               // break;
       // }//user input quit to quit the message system
        send(sock, bufSend, strlen(bufSend), 0);

        cout << "accepted client IP:" << inet_ntoa(sockAddr.sin_addr) << '\t';
        cout << "port:" << ntohs(sockAddr.sin_port)  << '\n';
        closesocket(sock);  //closing socket
   }
   if(n==3){
        char file_name[FILE_NAME_MAX_SIZE + 1];
        memset(file_name, 0, sizeof(file_name));
        printf("Please Input File Name On Server.\t");
        scanf("%s", file_name);

        char buffer[BUF_SIZE];
        memset(buffer, 0, sizeof(buffer));
        strncpy(buffer, file_name, strlen(file_name) > BUF_SIZE ? BUF_SIZE : strlen(file_name));
        // 向服务器发送buffer中的数据，此时buffer中存放的是客户端需要接收的文件的名字
        send(sock, buffer, BUF_SIZE, 0);

        FILE *fp = fopen(file_name, "w");
        if (fp == NULL)
        {
            printf("File:\t%s Can Not Open To Write!\n", file_name);
            exit(1);
        }

        // 从服务器端接收数据到buffer中
        memset(buffer, 0, sizeof(buffer));
        int length = 0;
        while(length = recv(sock, buffer, BUF_SIZE, 0))
        {
            if (length < 0)
            {
                //printf("Recieve Data From Server %s Failed!\n", argv[1]);
                break;
            }

            int write_length = fwrite(buffer, sizeof(char), length, fp);
            if (write_length < length)
            {
                printf("File:\t%s Write Failed!\n", file_name);
                break;
            }
            memset(buffer, 0, BUF_SIZE);
        }

       // printf("Recieve File:\t %s From Server[%s] Finished!\n", file_name, argv[1]);

   }

   CloseHandle(hThread);//close the receive thread.
   closesocket(sock); //close socket. because hThread uses sock as socket, so if we should close hThread sock first
   WSACleanup();  //stopping using DLL
   return 0;
        /*//Sending data in a loop until reach the end of file
         char buffer[BUF_SIZE] = {0};  //Buffer Area for file
         int nCount;
         while( (nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0 ){
             fwrite(buffer, nCount, 1, fp);
         }
         fclose(fp);
         */

    //printing the buffer
        //printf("accepted client IP:[%s],port:[%d]\n",inet_ntoa(sockAddr.sin_addr),ntohs(sockAddr.sin_port));
        //printf("Message form server: %s\n", bufRecv);

        //memset(bufRecv, 0, BUF_SIZE);  //reseting the buffer area

}
