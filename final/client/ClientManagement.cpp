#include "ClientManagement.h"
#include "tools.h"

#define FILE_NAME_MAX_SIZE    512


using namespace std;

ClientManagement::  ClientManagement(int pt,  char* ipAddres){
        ipAddress = ipAddres;
        port = pt;

}

void ClientManagement::  initialization(){//initializing DLL;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    memset(&sockAddr, 0, sizeof(sockAddr));//filling every byte with 0
    sockAddr.sin_family = AF_INET;//Using IPv4 address
    sockAddr.sin_addr.s_addr = inet_addr(ipAddress);// the server's IP address is local host
    sockAddr.sin_port = htons(port);//setting port number
    bufSend[BUF_SIZE] = {0};
}


void ClientManagement::  connection(){
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//creating socket
    if(connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR))==0){
            cout <<"success";
            }else{
                cout <<"false";
            }
    //connecting
}

//thread
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

		cout << "Message form other user:" << bufRecv << '\n';
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

void ClientManagement::  receiveMessage(){

  hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);



}
  void ClientManagement::  boradcast(){
        char ipstring = 'N';
        do{
       bufSend[0] = '9';
       bufSend[1] = '9';
       bufSend[2] = '9';
            //cout << bufSend[0];
          send(sock, bufSend, strlen(bufSend), 0);
            memset(bufSend, 0, BUF_SIZE);
        cout << "Input a string to broadcast(input 'quit' to quit): \n ";

         gets(bufSend);    gets(bufSend);
         cout << bufSend;     cout << "123";
       // gets(bufSend);//getting data from client and sending to server

       if (bufSend=="quit") {
               return ;
        }//user input quit to quit the message system
        send(sock, bufSend, strlen(bufSend), 0);
    memset(bufSend, 0, BUF_SIZE);
        cout <<"do you want to continue?(Y/N)"<<endl;
        cin >> ipstring;
        }while(ipstring=='Y');
        closesocket(sock);  //closing socket



  }
void ClientManagement::  sendMessage(char* bufRecv1){
        cout << "The usernames who are online now: \n";
        const char * split = "; ";
        vector<string> splitStrs;
        tool t;
        t.splitString(bufRecv1,split, splitStrs);
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
                return ;
            }
            int a=atoi(bufSend);
            if(a>temp1||a<0){
            flag = false;
                }else{
            flag = true;
    }
}
    cout <<bufSend;
    send(sock, bufSend, strlen(bufSend), 0);

    //users choosing the different works
    while(1){
        cout << "Input a string(input 'quit' to quit): \n ";
        gets(bufSend);//getting data from client and sending to server
        if (bufSend[0] == 'q'&&bufSend[1] == 'u'&&bufSend[2] == 'i'&&bufSend[3] == 't') { break; }//user input quit to quit the message system
        send(sock, bufSend, strlen(bufSend), 0);
        cout << "accepted client IP:" << inet_ntoa(sockAddr.sin_addr) << '\t';
        cout << "port:" << ntohs(sockAddr.sin_port)  << '\n';

    }  //closesocket(sock);  //closing socket

}


char* ClientManagement::  login(char* username){
    cout << username;
    send(sock, username, strlen(username), 0);//login in
    memset(username, 0, BUF_SIZE);
    hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);
    int rec;
		char bufRecv1[BUF_SIZE] = { 0 };//creating receiving buffer
		rec = recv(sock, bufRecv1, BUF_SIZE, 0);
		return bufRecv1;
}

ClientManagement::~ClientManagement()
{
    CloseHandle(hThread);//close the receive thread.
   closesocket(sock); //close socket. because hThread uses sock as socket, so if we should close hThread sock first
   WSACleanup();  //stopping using DLL
}


void printRights(){
      cout << "==========================================================\n";
    cout << "|This program is wirten by Xuan Li, Shuo Zhang, Zeyu Ni  |\n";
    cout << "|This program  using socket lib and tcp/ip portocol      |\n";
    cout << "==========================================================\n";
}
int main(){
    char buff[BUF_SIZE] = {0};
    printRights();
    ClientManagement cm(5000,"127.0.0.1");
    cm.initialization();
    cm.connection();
    cout << "Please Input your username:  ";//Asking for Inputing your Username
    gets(buff);//Getting client username from Client and Sending to Server
   char* userlist =  cm.login(buff);

     int n;
    cout << "Please input a number\n1-Send Message to other user;\n2-Sent Broadcast Request;\n3-Send Files\nPlease input:";
    cin >> n;
     /*
	1.ask to input username and send to server.
	2.receive a clientnumber from server
	3.input 1/2/3 represent send message or ask for broad cast or file transfer
	while(2)
	while(3)
	*/
	   if(n==1){

        cm.sendMessage(userlist);

        }else if(n==2){
 cm.boradcast();


	   }

}
