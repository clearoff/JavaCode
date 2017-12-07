#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <json/json.h>
#include <fstream>
using namespace std;

#define DIR_PATH "./FILE_DIR/"
#define ERROR_PATH "./FILE_DIR/error.json"
#define BUF_SIZE 1024
#define PATH "./FILE_DIR/config.json"

void CRC_16(const vector<int>& data,int num,int &CRC_Value);
string _itoa(int data);
string _IntArrayToStrng(const vector<int>& data);
int JudgeFunction(const int& num , string protocol);

//服务端协议类型
//$N40;E20001,30001,40001,50001,60001,70001;23857;#
class ServerProtocol {
public:
	ServerProtocol();
	//statusCode : 'N' , 'X', 'W' , 'R'
	string WriteProtocol(const string& statusCode,int devel);
	bool AnalysisProtocol(const char* buf,string& status);
	vector<int> SplitData(const char* buf, int num,int& pos);
	int getDevel();
	bool ReSendClient(int sock);
	void HandelProtocol(int sock);
private:
	string root;
	int _devel ; 
	vector<int> _checkArray ;
	int _crcValue ;
	char _statusCode ;
	bool _queryFlag;
};

//客户端协议类型
class ClientProtocol{
public:
	ClientProtocol();

	bool ModifyConfig(const string& protocol,const vector<int>& data);
	int JudgeFunction(const int& num , string protocol);
	string WriteProtocol(string protocol,vector<int> data,string address,int date);
	//121,101,120,109;E62212,33763,63210,7156,47009,2175;62982;#
	//数据包分割
	vector<int> SplitData(const char* buf, int num,int& pos);
	bool AnalysisProtocol(const char* pro);
	vector<int> QueryData(string protocol,int date);
	void HandleProtocol(int sock);
	void writeRoot(string tempString);
	bool JudgeProtocol();
	void WriteJson(string protocol,int date, const vector<int>& data);
private:
	string root ; //main 
	vector<int> _data;
	vector<int> _checkArray;
	int _crcValue;
	char _protocol;
	bool _modifyFlag ;
	bool _queryFlag ;
	int _date;

};