#include "common.h"
using namespace std;


void CRC_16(const vector<int>& data,int num,int &CRC_Value)
{
    int i,j;
    unsigned short Test=0x0001;
    unsigned short CRC=0xA001;              //CRC校验多项式的简式（把简式反过来输入,原来是8005H）
    CRC_Value =0xFFFF;                      //CRC寄存器置一
    for(i=0;i<num;i++)
    {
        CRC_Value=CRC_Value^data[i];        //前一字节的CRC校验值低字节与当前字节进行异或

        for(j=0;j<8;j++)
        {
            if(CRC_Value & Test)            //判断最低位是否为1
            {
                CRC_Value>>=1;              //右移1位
                CRC_Value^=CRC;             //进行异或运算
            }
            else
            {
                CRC_Value>>=1;              //右移1位
            }
        }
    }

}

string _itoa(int data)
{
	char buf[8] = {0};
	sprintf(buf,"%d",data);
	//cout<<buf<<endl;
	return buf;
}

string _IntArrayToStrng(const vector<int>& data)
{
	string newData ;
	char buf[1024] = {0};
	for(int i=0;i<data.size();i++){
		if(newData.size()!=0&&i!=(data.size())){
			newData += ",";
		}
		newData += _itoa(data[i]);
		//cout<<newData<<endl;
	}
	//cout<<"end"<<endl;
	memcpy(buf, newData.c_str(), newData.size());
	return buf;
}

int ServerProtocol::getDevel()
{
	return _devel;
}

//从服务端或得到的数据
// 
void ServerProtocol::HandelProtocol(int sock)
{
	if(_queryFlag==false){
		cout<<"begin control:"<<_devel<<endl;
		switch(_devel){
			case 0:{
				cout<<"当前环境参数正常，不需要进行处理"<<endl;
				break;
			}
			case 1:{
				cout<<"当前环境参数存在细微问题,需要进行简单处理"<<endl;
				break;
			}
			case 2:{
				cout<<"当前环境参数存在部分问题，需要进行处理"<<endl;
				break;
			}
			case 3:{

			}
			case 4:{
				cout<<"当前环境参数存在严重问题，需要进行处理"<<endl;
				break;
			}
			default :{
				cout<<"devel error"<<_devel<<endl;
			}
		}
	}
}

ServerProtocol::ServerProtocol()
	:root("$")
{}

//statusCode : 
//	N:全部数据正常，不需要进行调整;
//	W:环境数据异常，需要根据devel进行调整;
//	R:对方CRC校验错误，需要客户端重新发送报文;
//	X:数据帧格式错误;
bool ServerProtocol::AnalysisProtocol(const char* buf,string& status)
{
	int pos = 1;
	bool flag = false;
	vector<int>	tempData;
	//vector<int> checkArray; 
	//int crcValue;
	//int data;
	_queryFlag = false;
	_statusCode = buf[pos]; 
	status = "norepate";
	if (buf[pos] == 'X'){
		cout << "数据传送过程中出现错误，请重新发送" << endl;
		return false;
	}
	else if(buf[pos]=='R'){
		cout<<"对方要求重新发送数据帧"<<endl;
		
		status = "repate";
		return false;
	}
	else if(buf[pos]=='N'||buf[pos]=='W'){//N, W
		pos = pos + 3;
		
		if(buf[pos]=='-'){
			pos++;
			flag = true;
		}
		if(flag == true){
			_devel = -(buf[pos] - '0');
		}
		else{
			_devel = buf[pos] - '0';
		}
		cout<<"devel:"<<_devel<<endl;
		pos += 3 ;
		_checkArray = SplitData(buf + pos,6,pos);
		pos = pos + 1;
		vector<int> crcData = SplitData(buf+pos,1,pos);
		_crcValue = crcData[0];
	}else{//$T;L4;10,20,30,40;t1022;E47120,25251,52972,26763,12880,15272;57332;#
		_queryFlag = true;
		pos +=2;
		int tempPos = 0;
		char tempBuf[8] = {0};
		
		for (int i = pos+1; buf[i] != ';'; i++){
			tempBuf[tempPos++] = buf[i];
			pos = i;
		}
		int tempLength = atoi(tempBuf);
		
		pos+=2;
		tempData=SplitData(buf + pos,tempLength,pos);
		
		pos+=8;
		_checkArray=SplitData(buf + pos,6,pos);
		
		vector<int> crcData;
		//读取CRC校验码
		
		crcData = SplitData(buf + pos + 1, 1, pos);
		_crcValue = crcData[0];
	}
	int tempCrc;
	CRC_16(_checkArray,6,tempCrc);
	
	if(tempCrc != _crcValue){
		return false;
	}
	cout<<"查询数据的结果为：";
	for(int i=0;i<tempData.size();i++){
		cout<<tempData[i]<<" ";
	}
	cout<<endl;
	return true;
}

bool ServerProtocol::ReSendClient(int sock){
	
	string temproot;	
	vector<int> checkArray;
	int crcValue ;

	srand((unsigned)time(NULL));
    int temp ; 
    for(int i=0; i < 6; i++){
        temp = rand()%65535;
        checkArray.push_back(temp);
    }
    CRC_16(checkArray,6,crcValue);

	temproot = "$R;";
	temproot += "$D-1;";
	temproot += "E" + _IntArrayToStrng(checkArray) + ";";
	temproot += _itoa(crcValue) + ";";
	temproot += "#";
	
	if(write(sock,temproot.c_str(),temproot.size()) < 0){
		return false;
	}
	return true;
}

vector<int> ServerProtocol::SplitData(const char* buf, int num,int& pos)
{
	vector<int> tempData;
	char tempBuf[8] = { 0 };
	int tempPos = 0;
	int tempNum = 0;
	for (int i = 0; buf[i] != ';' && tempNum != num; i++,pos++){
		if (buf[i] == ','){
			tempNum++;
			tempPos = 0;
			//cout << tempBuf << " ";
			tempData.push_back(atoi(tempBuf));
			memset(tempBuf, '\0', 8);
			i++;
			pos++;
		}
		tempBuf[tempPos++] = buf[i];
	}
	tempData.push_back(atoi(tempBuf));

	return tempData;
}

string ServerProtocol::WriteProtocol(const string& statusCode,int devel)
{
	root.clear();
	root = "$";
	vector<int> checkArray;
	int CRC_Value;
	srand((unsigned)time(NULL));
    int temp ; 
    for(int i=0; i < 6; i++){
        temp = rand()%65535;
        checkArray.push_back(temp);
    }
    CRC_16(checkArray ,6,CRC_Value);

	root += statusCode + ";";
	if(statusCode!="N"&& statusCode !="W"){

	}
	else{
		root += "D" + _itoa(devel) + ";";
	}
	root += "E" + _IntArrayToStrng(checkArray) + ";";
	root += _itoa(CRC_Value) + ";";
	root += "#";
	return root;
}

ClientProtocol::ClientProtocol()
	:root("$")
	,_modifyFlag(false)
{}

vector<int> ClientProtocol::QueryData(string protocol,int date)
{
	Json::Value root ;
	Json::Reader reader ;
	vector<int> tempData;
	ifstream fin ;
	string file = "./FILE_DIR/"+protocol + ".json";
	fin.open(file.c_str()) ;
	if(reader.parse(fin, root)){
		
		string index = _itoa(date);
		Json::Value array = root[index.c_str()];
		for(int i=0; i<array.size(); i++){
			int temp = array[i].asInt();
			tempData.push_back(temp);
		}
	}
	fin.close();
	return tempData;
}

int ClientProtocol::JudgeFunction(const int& num , string protocol)
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
			
			if(num < array[i].asInt()){
				break;
			}
		}
	}
	fout.close();
	return pos;
}

bool ClientProtocol::JudgeProtocol()
{
	if(_protocol=='R'){
		return false;
	}else{
		return true;
	}
}

string ClientProtocol::WriteProtocol(string protocol,vector<int> data,string address,int date)
{
	root.clear();
	root = "$";
	int CRC_Value;
	vector<int> checkArray;
	srand((unsigned)time(NULL));
    int temp ; 
    for(int i=0; i < 6; i++){
        temp = rand()%65535;
        checkArray.push_back(temp);
    }
    CRC_16(checkArray ,6,CRC_Value);
	root += protocol + ";";
	root += "L" + _itoa(data.size())+";";
	root += _IntArrayToStrng(data) + ";";
	root += "t"+_itoa(date)+";";
	root += "E" + _IntArrayToStrng(checkArray) + ";";
	root += _itoa(CRC_Value) + ";";
	root += "#";
	cout<<root<<endl;
	return root;
}

vector<int> ClientProtocol::SplitData(const char* buf, int num,int& pos)
{
	vector<int> tempData;
	char tempBuf[8] = { 0 };
	int tempPos = 0;
	int tempNum = 0;
	for (int i = 0; buf[i] != ';' && tempNum != num; i++,pos++){
		if (buf[i] == ','){
			tempNum++;
			tempPos = 0;
			//cout << tempBuf << " ";
			tempData.push_back(atoi(tempBuf));
			memset(tempBuf, '\0', 8);
			i++;
			pos++;
		}
		tempBuf[tempPos++] = buf[i];
	}
	tempData.push_back(atoi(tempBuf));
	return tempData;
}

bool ClientProtocol::AnalysisProtocol(const char* pro)
{
	int pos = 3;
	int dataLength;
	char tempBuf[8] = { 0 };
	int crcValue;
	vector<int> checkArray;
	vector<int> tempData;
	_modifyFlag= false ;
	_queryFlag = false ;
	_protocol = pro[1];
	if(_protocol=='R'){
		cout<<"需要重新发送数据包"<<endl;
		return false;
	}

	if(_protocol == 'M'){
		cout<<"开始修改标准值"<<endl;
		_modifyFlag = true;
		_protocol = pro[2];
		pos++;
	}
	if(_protocol == 'Q'){
		cout<<"开始查询"<<endl;
		_queryFlag = true;
		_protocol = pro[2];
		pos++;
	}
	//解析数据包长度
	if (pro[pos] == 'L'){
		int tempPos = 0;
		for (int i = pos+1; pro[i] != ';'; i++){
			tempBuf[tempPos++] = pro[i];
			pos = i;
		}
		dataLength = atoi(tempBuf);
		cout << "Data Length:" << dataLength << endl;
	}
	pos += 2;
	
	tempData = SplitData(pro + pos,dataLength,pos);

	//解析校验位
	pos++;
	vector<int> tempdate;
	cout<<pro[pos]<<endl;
	if(pro[pos]=='t'){
		tempdate = SplitData(pro + pos+1,1,pos);
	}
	_date = tempdate[0] ;
	pos+=2;
	if (pro[pos] == 'E'){
		pos++;
		checkArray = SplitData(pro + pos, 6, pos);
	}
	vector<int> crcData;
	//读取CRC校验码
	crcData = SplitData(pro + pos + 1, 1, pos);
	crcValue = crcData[0];
	_data = tempData;
	_checkArray = checkArray;
	_crcValue = crcValue;
	cout<<"CRC :"<<_crcValue<<endl;

	int tempCrc;
	CRC_16(_checkArray,6,tempCrc);
	cout<<"tempCrc:"<<tempCrc<<endl;
	if(tempCrc != _crcValue){
		cout<<"CRC error"<<endl;
		return false;
	}
	cout<<"CRC校验成功"<<endl;
	return true;
}

void ClientProtocol::WriteJson(string protocol,int date, const vector<int>& data)
{
	Json::Value root ;
	Json::Reader reader ;
	ifstream fin ; 
	string file ;
	file = "./FILE_DIR/"+protocol+".json";
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

bool ClientProtocol::ModifyConfig(const string& protocol,const vector<int>& data)
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

	return true;
}

//
void ClientProtocol::HandleProtocol(int sock){
		int tempCrc ;
		string StatusCode ;
		CRC_16(_checkArray,6,tempCrc);
		int Average ;
		string protocolString ;
		ServerProtocol sevPag;
		ClientProtocol cliPag;
		string sendStr ;
		

		int sum ; 
		//对客户端发来的数据帧中的实际数据进行处理；
		for(int i=0;i<_data.size();i++){
			sum += _data[i];
		}
		Average = sum/_data.size();
		switch(_protocol){
			case 'T':{
				cout<<"TEM"<<endl;
				protocolString = "T";
				break;
			}
			case 'C':{
				cout<<"CO2"<<endl;
				protocolString = "C";
				break;
			}
			case 'P':{
				cout<<"PM2.5"<<endl;
				protocolString = "P";
				break;
			}
			case 'O':{
				cout<<"O2"<<endl;
				protocolString = "O";
				break;
			}
			case 'H':{
				cout<<"PH"<<endl;
				protocolString = "H";
				break;
			}
			default :{
				cout<<"Data error"<<endl;
			}
		}//switch
		
		
		if(_modifyFlag==true){
			if(ModifyConfig(protocolString.c_str(),_data)==true){
				cout<<"Modify Success"<<endl;
				sendStr = sevPag.WriteProtocol("N",0);
			}else{
				cout<<"Modify Fail"<<endl;
				sendStr = sevPag.WriteProtocol("X",-1);
			}
		}
		else if(_queryFlag == true){
			cout<<"query date:"<<_date<<endl;
			vector<int> date = QueryData(protocolString,_data[0]);
			if(date.size() > 0){
				cout<<"Query Success"<<endl;
			}
			cout<<"date size:"<<date.size()<<endl;
			for(int i=0;i<date.size();i++){
				cout<<date[i]<<" ";
			}
			cout<<endl;
			sendStr = WriteProtocol(protocolString,date,"0011",_date);
			cout<<"sendstr"<<sendStr<<endl;
		}
		else{
			WriteJson(protocolString,_date,_data);
			cout<<"success write"<<endl;
			int devel ;
			devel = JudgeFunction(Average, protocolString);
			if(devel>=1){
				StatusCode = 'W';
			}else{//devel < 2 :
				StatusCode = 'N';
			}
			sendStr = sevPag.WriteProtocol(StatusCode,devel);
		}
		cout<<sendStr<<endl;
		if(write(sock,sendStr.c_str(),sendStr.size()) < 0){
			cout<<"Error Write"<<endl;
		}
}


void ClientProtocol::writeRoot(string tempString){
	root = tempString;
}