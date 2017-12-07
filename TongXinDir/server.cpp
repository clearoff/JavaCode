#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <stdio.h>
#include "common.h"

#define BUF_SIZE 1024

int HandleSock(int sock){
	//cout<<"Sock:"<<sock<<endl;
	char buf[BUF_SIZE+1] = {0};
	int ReadSize = 0;
	
	while(1){
		ServerProtocol sevPro ;
		ClientProtocol cliPro ;
		ReadSize = recv(sock,buf,1024,0);
		cout<<"ReadSize:"<<ReadSize<<endl;
		if(ReadSize==0){
			cout<<"client "<<sock<<" is quit"<<endl;
			break;
		}
		else if(ReadSize < 0){
			cout<<"server read error"<<endl;
			continue;
		}
		string tempStr = buf;
		memset(buf,0,BUF_SIZE);
		bool AnalysisFlag = cliPro.AnalysisProtocol(tempStr.c_str());
		cout<<"Analysis bool:"<<AnalysisFlag<<endl;
		if(AnalysisFlag){
			cout<<"Begin handel Client protocol"<<endl;
			cliPro.HandleProtocol(sock);
		}
		else{
			cout<<"Repate"<<endl;
			sevPro.ReSendClient(sock);
		}
		sleep(1);
	}

	// while(1){
	// FLAG:
	// 	string tempStr;
	// 	ServerProtocol sevPro ;
	// 	ClientProtocol cliPro ;
	// 	memset(buf, 0,BUF_SIZE);
	// 	ReadSize = recv(sock, buf, 128,0);
	// 	cout<<"ReadSize:"<<ReadSize<<endl;
	// 	if(ReadSize == 0){
	// 		cout<<"client is quit"<<endl;
	// 		break;
	// 	}else if(ReadSize < 0){
	// 		cout<<"read error"<<endl;
	// 		continue;
	// 	}else{
	// 		cout<<buf<<endl;
	// 		string tempStr = buf;
	// 		memset(buf, '\0',BUF_SIZE);
	// 		cout<<"buf:"<<buf<<endl;
	// 		cout<<"Analysis:"<<cliPro.AnalysisProtocol(tempStr.c_str())<<endl;
	// 		if(cliPro.AnalysisProtocol(tempStr.c_str())){
	// 			cliPro.HandleProtocol(sock);
	// 		}else{
	// 			sevPro.ReSendClient(sock);
	// 			goto FLAG;
	// 		}
			
	// 	}
	// }

	return 1;
}

static void* AcceptRun(void* arg)
{
	int sock = (int)arg;
	pthread_detach(pthread_self());
	return (void*)HandleSock(sock);
}

int StartUp(char* ip, char* serverPort)
{
	//tcp listen_sock
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	cout<<"server listen sock:"<<sock<<endl;

	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(atoi(serverPort));
	sockaddr.sin_addr.s_addr = inet_addr(ip);
	if(bind(sock,(struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0){
		cout<<"socket bind error"<<endl;
		exit(1);
	}

	if(listen(sock, 5) < 0) {
		cout<<"listen sock error"<<endl;
		exit(2);
	}

	return sock;
}

int main(int argc , char* argv[])
{
	if(argc < 3){
		cout<<"Useage: ./server ip port"<<endl;
		exit(1);
	}

	int listen_sock = StartUp(argv[1], argv[2]);
	while(1){
		struct sockaddr_in addr ;
		socklen_t len = sizeof(addr);
		int sock;
		if((sock = accept(listen_sock,\
			(struct sockaddr*)&addr,&len)) < 0){
			cout<<"Server accept error"<<endl;
			continue;
		}
		cout<<"receive request:"<<sock<<endl;

		pthread_t pid;
		if(pthread_create(&pid, NULL, AcceptRun, (void*)sock)!=0){
			cout<<"pthread created error"<<endl;
			close(sock);
			continue;
		}
	}
	return 0;
}