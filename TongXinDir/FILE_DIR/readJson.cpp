#include <iostream>
using namespace std;
#include <json/json.h>
#include <fstream>
#include <string>
#include <vector>

#define PATH "./config.json"
#define DATA_PATH ".json"
//return val: pos
int JudgeFunction(const int& num , string protocol)
{

	int pos = -1;
	Json::Reader reader ;
	Json::Value root ;
	ifstream fout ;
	fout.open(PATH);
	if(reader.parse(fout, root)){
		Json::Value array = root[protocol.c_str()];
		for(int i=0; i<array.size(); i++){
			pos = i;
			cout<<i<<endl;
			if(num < array[i].asInt()){
				break;
			}
		}
	}
	fout.close();
	return pos;
}


void ModifyConfig(string protocol,const vector<int>& data)
{
	Json::Value root ;
	Json::Reader reader ;
	ifstream fin ; 
	fin.open(PATH);
	if(reader.parse(fin, root)){
		Json::Value array = root[protocol.c_str()];
		for(int i=0; i<data.size(); i++){
			array[i] = data[i];
		}
		root[protocol.c_str()] = array;
	}
	fin.close();
	ofstream fout;
	fout.open(PATH);
	fout<<root.toStyledString();
	fout.close();
}

string _itoa(int data)
{
	char buf[8] = {0};
	sprintf(buf,"%d",data);
	//cout<<buf<<endl;
	return buf;
}

void QueryData(string protocol,const vector<int>& data)
{
	Json::Value root ;
	Json::Reader reader ;
	ifstream fin ;
	string file = protocol + ".json";
	fin.open(file.c_str()) ;
	if(reader.parse(fin, root)){
		
		for(int i=0;i<data.size();i++)
		{
			string index = _itoa(data[i]);
			Json::Value array;
			for(int i=0; i<data.size(); i++){
				cout<<array[i]<<endl;
			}
		}
	}
	fin.close();
}

void WriteJson(string protocol,int date, const vector<int>& data)
{
	Json::Value root ;
	Json::Reader reader ;
	ifstream fin ; 
	string file ;
	file = protocol+".json";
	cout<<file<<endl;
	fin.open(file.c_str());
	if(reader.parse(fin, root)){
		string index = _itoa(date);
		Json::Value array;
		for(int i=0;i<data.size();i++){
			array[i] = data[i];
		}
		root[index.c_str()] = array;
	}
	fin.close();
	ofstream fout;
	fout.open(file.c_str());
	fout<<root.toStyledString();
	fout.close();
}

int main()
{
	//cout<<JudgeFunction(40,"PM")<<endl;
	int num[] = {1020,1021,1022,1023};
	vector<int> data(num,num+4);
	WriteJson("PM",1029,data);
	return 0;
}