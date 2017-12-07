#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include "common.h"

#define BUFSIZE 1024

string GetProStr()
{
	int Length ;
	string ProStr;
	char ProCode ;
	char ProChar ;
	cout<<"请输入相关字符：\nQ:查询\tM:修改标准值\n其他：表示采集实际数据"<<endl;
    cin>>ProCode;
	switch(ProCode){
		case 'Q':{
			ProStr = "Q";
			break;
		}
		case 'M':{
			ProStr = "M";
			break;
		}
		default:{
			break;
		}
	}
	cout<<"请输入相关数据类型：\nT:温度\tP:土壤酸碱度\tH:土壤湿度\tO:土壤含氧量\tC:土壤二氧化碳浓度"<<endl;
    cin>>ProChar;
    switch(ProChar){
    	case 'T':{
    		ProStr+="T";
    		break;
    	}
    	case 'O':{
    		ProStr+="O";
    		break;
    	}
    	case 'P':{
    		ProStr+="P";
    		break;
    	}
    	case 'C':{
    		ProStr+="C";
    		break;
    	}
    	case 'H':{
    		ProStr+="H";
    		break;
    	}
    	default :{
    		cout<<"Input error"<<endl;
    	}
    }
    //cout<<ProStr<<endl;
    return ProStr;
}

int main(int argc, char* argv[]){
	if(argc < 3){
		cout<<"Useage: [connect_server] [connect_port]"<<endl;
		exit(1);
	}
	char buf[BUFSIZE] = {0}; 

	int conSock = socket(AF_INET, SOCK_STREAM, 0);
	if(conSock < 0){
		cout<<"create sock error"<<endl;
		exit(2);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(conSock,(struct sockaddr*)&addr, sizeof(addr)) < 0){
		cout<<"client connect server error"<<endl;
		exit(3);
	}
	cout<<"Client connect success"<<endl;

	
	ClientProtocol clipro;
	ServerProtocol sevPag;
	int num[] = {10,20,30,40};
	vector<int> data;
	bool ResultBool;
	bool ReSendFlag = false;
	string temp;
	int date;

	while(1){
		data.clear();
		
		temp = GetProStr();
		int DataLength ; 
		cout<<"Input Data Length:";
		cin>>DataLength;
		cout<<endl<<"Input data:"<<endl;
		for(int i=0;i<DataLength;i++){
			int tempData ;
			cin>>tempData;
			data.push_back(tempData);
		}
		cout<<"Input date:";
		cin>>date;
		cout<<endl;

		FLAG1:
		string out = clipro.WriteProtocol(temp,data,"0011",date);
		cout<<"生成的数据帧为:"<<out<<endl;
		
		if(send(conSock, out.c_str(), out.size(),0) < 0){
			cout<<"send error" <<endl;
			continue;
		}
		else{
		FLAG2:
			if(recv(conSock,buf,BUFSIZE,0) < 0){
				cout<<"recv error" <<endl;
				continue;
			}else{
				cout<<"从服务端接收到的数据帧#"<<buf<<endl;
			}
		}
		string status;
		ResultBool = sevPag.AnalysisProtocol(buf,status);

		if(ResultBool==true){
			sevPag.HandelProtocol(conSock);
			break;
		}
		else{
			if(status=="repate"){
				cout<<"begin repate"<<endl;
				goto FLAG1;
			}
			else{
				goto FLAG2;
			}
		}
		memset(buf,0,BUFSIZE);
		sleep(1);
	}
	
	cout<<"CLient is exit"<<endl;
	close(conSock);
	return 0;
}
