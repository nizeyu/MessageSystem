#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll

#define BUF_SIZE 100

int main(){
    WSADATA wsaData;

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
    memset(&sockAddr, 0, sizeof(sockAddr));  /
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
    char buffer[BUF_SIZE] = {0};  //creat buffer area
    while(1){
       sAccept = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
       if(sAccept==INVALID_SOCKET)
		{
			printf("accept()failed:%d\n",WSAGetLastError());
			break;
		}
		printf("accepted client IP:[%s],port:[%d]\n",inet_ntoa(sockAddr.sin_addr),ntohs(sockAddr.sin_port));
        int strLen = recv(sAccept, buffer, BUF_SIZE, 0);  //
        printf("Message form client: %s\n", buffer);
        printf("Input a string: ");
        gets(buffer);
        send(sAccept, buffer, strLen, 0);  //send data to client

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
