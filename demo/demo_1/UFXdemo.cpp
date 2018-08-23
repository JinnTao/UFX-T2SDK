#include "Include/t2sdk_interface.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <process.h>
using namespace std;


class CErrorInfo
{
public:
	CErrorInfo():ErrorCode(0),ErrorMsg(""){}
	int    ErrorCode;
	string ErrorMsg;
};

void       ShowPacket(IF2UnPacker* unPacker);
CErrorInfo Connect(const char* serverAddr,CConnectionInterface** connection);
CErrorInfo Login(CConnectionInterface* connection, const char* operatorNo, const char* password, string& userToken);
CErrorInfo Entrust(CConnectionInterface* connection,const char* userToken, const char* combiNo,const char* marketNo, const char* stockCode, const char* entrustDirection, double entrustPrice, int entrustAmount,int& entrustNo);
CErrorInfo QueryEntrusts(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo,int entrustNo, IF2UnPacker** responseUnPacker);
CErrorInfo QueryDeals(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo,int entrustNo, IF2UnPacker** responseUnPacker);
CErrorInfo QueryAccount(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo, IF2UnPacker** responseUnPacker);
CErrorInfo QueryCombiStock(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo,const char* marketNo, const char* stockCode, IF2UnPacker** responseUnPacker);
CErrorInfo QueryAccountCode(CConnectionInterface* connection, const char* userToken,const char* accountCode, IF2UnPacker** responseUnPacker,string &out_accountCode);
CErrorInfo QueryAssetCode(CConnectionInterface* connection, const char* userToken, const char* capitaAccount, const char* accountCode, const char *assetCode, IF2UnPacker** responseUnPacker,string & asscetCode);
CErrorInfo QueryCombiCode(CConnectionInterface* connection, const char* userToken, const char* accountCode, const char* assetCode, const char* combiNo, IF2UnPacker** responseUnPacker, string &out_combi_no);
CErrorInfo QueryFundaset(CConnectionInterface* connection, const char* userToken, const char* accountCode, IF2UnPacker** responseUnPacker);



void       HeartBeat(CConnectionInterface* connection,const char* userToken);

int main(int argc, char** argv)
{
	/*
	if(argc < 4)
	{
		cout << "Usage: UFXDemo serverAddr operatorNo, password, accountCode combiNo" << endl
			 << "Examples:" << endl
			 << "	UFXDemo 192.168.54.57:9012 1000 0 184693 1000005" << endl;
		getchar();
		return 0;
	}*/

	string serverAddr  = "192.168.41.93:9003"; //UFX��������ַ
	string operatorNo  = "2700";               //����Ա
	string password    = "tf123456";                  //����
	string accountCode = "2700";             //�˻�����
	string combiNo     = "2700";            //��ϴ���  	
    string capitalNo = "";             //�ʲ���Ԫ���
    string assetCode = "";
    string out_accountCode;
    string out_assetCode;
    string out_combi_no;

	if(argc >=4)
	{
		serverAddr  = argv[1];
		accountCode = argv[2];
		combiNo     = argv[3];
	}

	string marketNo         = "1";         //�����г�
	string stockCode        = "600000";    //֤ȯ����
	string entrustDirection = "1";         //ί�з���
	double entrustPrice     = 10.10;       //ί�м۸�
	int    entrustAmount    = 100;         //ί������

	CErrorInfo            errorInfo;
	string                userToken;
	CConnectionInterface* connection;
	int                   entrustNo;
	IF2UnPacker*          responseUnPacker;

	//����
	errorInfo = Connect(serverAddr.c_str(),&connection);
	if(errorInfo.ErrorCode != 0)
	{		
		cout << "����ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
		getchar();
		return -1;
	}     
	else
	{
		cout << "���ӳɹ���" << endl;
	}

	//��¼
	errorInfo = Login(connection,operatorNo.c_str(),password.c_str(),userToken);
	if(errorInfo.ErrorCode != 0)
	{
		cout << "��¼ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
		connection->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "��¼�ɹ���" << endl;
	}

	//����
	HeartBeat(connection,userToken.c_str());
    //���˻����
    errorInfo = QueryAccountCode(connection, userToken.c_str(), accountCode.c_str(), &responseUnPacker, out_accountCode);
    if (errorInfo.ErrorCode != 0)
    {
        cout << "���˻����ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
        connection->Release();
        getchar();
        return -1;
    }
    else
    {
        cout << "---------��Ϣ-----------" << endl;
        ShowPacket(responseUnPacker);
    }
    //���ʲ���Ԫ
    errorInfo = QueryAssetCode(connection, userToken.c_str(), capitalNo.c_str(), out_accountCode.c_str(), assetCode.c_str(), &responseUnPacker, out_assetCode);
    if (errorInfo.ErrorCode != 0)
    {
        cout << "���ʲ���Ԫ��������ϢΪ��" << errorInfo.ErrorMsg << endl;
        connection->Release();
        getchar();
        return -1;
    }
    else
    {
        cout << "---------��Ϣ-----------" << endl;
        ShowPacket(responseUnPacker);
    }


    //����ϱ��
    errorInfo = QueryCombiCode(connection, userToken.c_str(), out_accountCode.c_str(), out_assetCode.c_str(), combiNo.c_str(), &responseUnPacker, out_combi_no);
    if (errorInfo.ErrorCode != 0)
    {
        cout << "����ϱ�š�������ϢΪ��" << errorInfo.ErrorMsg << endl;
        connection->Release();
        getchar();
        return -1;
    }
    else
    {
        cout << "---------��Ϣ-----------" << endl;
        ShowPacket(responseUnPacker);
    }

    //���˻��ʲ���ѯ
    errorInfo = QueryFundaset(connection, userToken.c_str(), out_accountCode.c_str(),&responseUnPacker);
    if (errorInfo.ErrorCode != 0)
    {
        cout << "���˻��ʲ���������ϢΪ��" << errorInfo.ErrorMsg << endl;
        connection->Release();
        getchar();
        return -1;
    }
    else
    {
        cout << "---------�˻��ʲ�-----------" << endl;
        ShowPacket(responseUnPacker);
    }
















    //���ʽ�
    errorInfo = QueryAccount(connection, userToken.c_str(), out_accountCode.c_str(), out_combi_no.c_str(), &responseUnPacker);
    if (errorInfo.ErrorCode != 0)
    {
        cout << "���ʽ�ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
        connection->Release();
        getchar();
        return -1;
    }
    else
    {
        cout << "---------�ʽ���Ϣ-----------" << endl;
        ShowPacket(responseUnPacker);
    }



    //��ֲ�
    errorInfo = QueryCombiStock(connection, userToken.c_str(), accountCode.c_str(), combiNo.c_str(), marketNo.c_str(), stockCode.c_str(), &responseUnPacker);
    if (errorInfo.ErrorCode != 0)
    {
        cout << "��ֲ�ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
        connection->Release();
        getchar();
        return -1;
    }
    else
    {
        cout << "---------�ֲ���Ϣ-----------" << endl;
        ShowPacket(responseUnPacker);
    }






	//ί��
	errorInfo = Entrust(connection,userToken.c_str(),combiNo.c_str(),marketNo.c_str(),stockCode.c_str(),entrustDirection.c_str(),entrustPrice,entrustAmount,entrustNo);
	if(errorInfo.ErrorCode != 0)
	{
		cout << "ί��ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
		connection->Release();
		getchar();
		return -1;
	}
	else
	{
		cout << "ί�гɹ���" << endl;
	}	

	//��ί��
	errorInfo = QueryEntrusts(connection,userToken.c_str(),accountCode.c_str(),combiNo.c_str(),entrustNo,&responseUnPacker);
	if(errorInfo.ErrorCode != 0)
	{
		cout << "��ί��ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
		connection->Release();
		getchar();
		return -1;
	}
	else
	{		
		cout << "---------ί����Ϣ-----------" << endl;
		ShowPacket(responseUnPacker);
	}	

	//��ɽ�
	errorInfo = QueryDeals(connection,userToken.c_str(),accountCode.c_str(),combiNo.c_str(),entrustNo,&responseUnPacker);
	if(errorInfo.ErrorCode != 0)
	{
		cout << "��ɽ�ʧ�ܡ�������ϢΪ��" << errorInfo.ErrorMsg << endl;
		connection->Release();
		getchar();
		return -1;
	}
	else
	{		
		cout << "---------�ɽ���Ϣ-----------" << endl;
		ShowPacket(responseUnPacker);
	}	

	connection->Release();
    getchar();    
    return 0;
}


void ShowPacket(IF2UnPacker* unPacker)
{
	int i = 0, t = 0, j = 0, k = 0;

	for (i = 0; i < unPacker->GetDatasetCount(); ++i)
	{
		// ���õ�ǰ�����
		unPacker->SetCurrentDatasetByIndex(i);

		// ��ӡ�ֶ�
		for (t = 0; t < unPacker->GetColCount(); ++t)
		{
			printf("%20s", unPacker->GetColName(t));
		}

		putchar('\n');

		// ��ӡ���м�¼
		for (j = 0; j < (int)unPacker->GetRowCount(); ++j)
		{
			// ��ӡÿ����¼
			for (k = 0; k < unPacker->GetColCount(); ++k)
			{
				switch (unPacker->GetColType(k))
				{
				case 'I':
					printf("%20d", unPacker->GetIntByIndex(k));
					break;

				case 'C':
					printf("%20c", unPacker->GetCharByIndex(k));
					break;

				case 'S':
					printf("%20s", unPacker->GetStrByIndex(k));
					break;

				case 'F':
					printf("%20f", unPacker->GetDoubleByIndex(k));
					break;

				case 'R':
					{
						int nLength = 0;
						void *lpData = unPacker->GetRawByIndex(k, &nLength);

						// ��2�������ݽ��д���
						break;
					}

				default:
					// δ֪��������
					printf("δ֪�������͡�\n");
					break;
				}
			}

			putchar('\n');

			unPacker->Next();
		}

		putchar('\n');
	}
}

CErrorInfo GetErrorInfo(IF2UnPacker* responseUnPacker)
{
	CErrorInfo errorInfo;
	errorInfo.ErrorCode = responseUnPacker->GetInt("ErrorCode");
	if(responseUnPacker->GetStr("ErrorMsg"))
		errorInfo.ErrorMsg  = responseUnPacker->GetStr("ErrorMsg");
	return errorInfo;
}

CErrorInfo CallService(CConnectionInterface* connection,int functionNo, IF2Packer* requestPacker, IF2UnPacker** responseUnPacker)
{
	CErrorInfo errorInfo;
	int ret = connection->SendBiz(functionNo,requestPacker);
	if (ret > 0)
	{		
		void* Pointer;
		ret = connection->RecvBiz(ret,&Pointer,3000);
		switch(ret)
		{
		case 0:
		case 1:
			//Ӧ���һ������������ݼ���
			//��һ�����ݼ���ͷ����Ϣ��������ErrorCode��ErrorMsg����Ϣ
			//�ڶ������ݼ�����������ҵ������
			(*responseUnPacker) = (IF2UnPacker*)Pointer;
			errorInfo = GetErrorInfo(*responseUnPacker);
			if((*responseUnPacker)->GetDatasetCount() > 1) (*responseUnPacker)->SetCurrentDatasetByIndex(1);
			break;
		case 2:
			errorInfo.ErrorCode = 2;
			errorInfo.ErrorMsg = (char*)Pointer;
			break;
		default:
			errorInfo.ErrorCode = 3;
			errorInfo.ErrorMsg = connection->GetErrorMsg(ret);
			break;

		}
	}
	else
	{
		errorInfo.ErrorCode = ret;
		errorInfo.ErrorMsg = connection->GetErrorMsg(ret);
	}
	return errorInfo;
}

CErrorInfo Connect(const char* serverAddr,CConnectionInterface** connection)
{
	CErrorInfo errorInfo;
	//����T2SDK���ö��󣬲�����UFX��������ַ�Լ�License�ļ�
	CConfigInterface * config = NewConfig();
	config->AddRef();
	config->SetString("t2sdk", "servers", serverAddr);	
	config->SetString("t2sdk", "license_file", "license.dat"); 

	//�������Ӷ��󣬲�����UFX������
	CConnectionInterface* conn = NewConnection(config);
	conn->AddRef();
	int ret = conn->Create(NULL);
	if( ret != 0)
	{
		conn->Release();
		config->Release();
		errorInfo.ErrorCode = ret;
		errorInfo.ErrorMsg =  conn->GetErrorMsg(ret);		
		return errorInfo;
	}

	//����UFX������������3000Ϊ��ʱ��������λ����
	ret = conn->Connect(3000); 
	if( ret != 0)
	{
		conn->Release();
		config->Release();
		errorInfo.ErrorCode = ret;
		errorInfo.ErrorMsg =  conn->GetErrorMsg(ret);		
		return errorInfo;
	}

	config->Release();
	(*connection) = conn;
	return errorInfo;
}

IF2Packer* MakeLoginPacker(const char* operatorNo, const char* password)
{
	IF2Packer* requestPacker = NewPacker(2);
	requestPacker->AddRef();
	requestPacker->BeginPack();
	requestPacker->AddField("operator_no",     'S',16, 0);
	requestPacker->AddField("password",        'S',32, 0);
	requestPacker->AddField("mac_address",     'S',255,0);
	requestPacker->AddField("op_station",      'S',255,0);
	requestPacker->AddField("ip_address",      'S',32, 0);
	requestPacker->AddField("authorization_id",'S',64, 0);
	requestPacker->AddStr(operatorNo);
	requestPacker->AddStr(password);
	requestPacker->AddStr("5C-26-0A-2F-82-4F");
	requestPacker->AddStr("192.168.88.123|5C-26-0A-2F-82-4F");
	requestPacker->AddStr("192.168.88.123");
	requestPacker->AddStr("hd123456");    
	requestPacker->EndPack();
	return requestPacker;
}

CErrorInfo GetUserToken(IF2UnPacker* responseUnPacker, string& userToken)
{
	CErrorInfo errorInfo;
	if (responseUnPacker->FindColIndex("user_token") >=0 )
	{
		userToken = responseUnPacker->GetStr("user_token");
	}
	else
	{
		errorInfo.ErrorCode = -100;
		errorInfo.ErrorMsg  = "user_token�ֶ���Ӧ����в�����";
	}
	return errorInfo;
}

CErrorInfo Login(CConnectionInterface* connection, const char* operatorNo, const char* password, string& userToken)
{
	IF2Packer* lpRequestPacker = MakeLoginPacker(operatorNo,password);
	IF2UnPacker* lpUnPacker;
	//��¼���ܺţ�10001
	CErrorInfo errorInfo = CallService(connection,10001,lpRequestPacker,&lpUnPacker);
	if(errorInfo.ErrorCode == 0)
	{
		errorInfo = GetUserToken(lpUnPacker,userToken);
	}
	lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
	lpRequestPacker->Release();
	return errorInfo;
}

IF2Packer* MakeEntrustPacker(const char* userToken, const char* combiNo,const char* marketNo
	, const char* stockCode, const char* entrustDirection, double entrustPrice, int entrustAmount)
{
	IF2Packer* requestPacker = NewPacker(2);
	requestPacker->AddRef();
	requestPacker->BeginPack();
	requestPacker->AddField("user_token",        'S',512,0);
	requestPacker->AddField("combi_no"  ,        'S',8,0); 
	requestPacker->AddField("market_no" ,        'S',3,0);
	requestPacker->AddField("stock_code",        'S',16,0);
	requestPacker->AddField("entrust_direction", 'S',1,0);
	requestPacker->AddField("price_type",        'S',1,0);
	requestPacker->AddField("entrust_price",     'F',9,3);
	requestPacker->AddField("entrust_amount",    'F',16,2);  
	requestPacker->AddStr(userToken);
	requestPacker->AddStr(combiNo);
	requestPacker->AddStr(marketNo);
	requestPacker->AddStr(stockCode);
	requestPacker->AddStr(entrustDirection);
	requestPacker->AddStr("0");                  //�޼�
	requestPacker->AddDouble(entrustPrice);
	requestPacker->AddDouble(entrustAmount);
	requestPacker->EndPack();
	return requestPacker;
}

CErrorInfo GetEntrustNo(IF2UnPacker* responseUnPacker, int& entrustNo)
{
	CErrorInfo errorInfo;
	if (responseUnPacker->FindColIndex("entrust_no") >=0 )
	{
		entrustNo = responseUnPacker->GetInt("entrust_no");
	}
	else
	{
		errorInfo.ErrorCode = -100;
		errorInfo.ErrorMsg  = "entrust_no�ֶ���Ӧ����в�����";
	}
	return errorInfo;
}

CErrorInfo Entrust(CConnectionInterface* connection,const char* userToken, const char* combiNo,const char* marketNo, const char* stockCode, const char* entrustDirection, double entrustPrice, int entrustAmount,int& entrustNo)
{
	IF2Packer* lpRequestPacker = MakeEntrustPacker(userToken,combiNo,marketNo,stockCode,entrustDirection,entrustPrice,entrustAmount);
	IF2UnPacker* lpUnPacker;
	//��¼���ܺţ�91001
	CErrorInfo errorInfo = CallService(connection,91001,lpRequestPacker,&lpUnPacker);
	if(errorInfo.ErrorCode == 0)
	{
		errorInfo = GetEntrustNo(lpUnPacker,entrustNo);
	}
	lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
	lpRequestPacker->Release();
	return errorInfo;
}

IF2Packer* MakeQueryEntrustsPacker(const char* userToken, const char* accountCode, const char* combiNo,int entrustNo)
{
	IF2Packer* requestPacker = NewPacker(2);
	requestPacker->AddRef();
	requestPacker->BeginPack();
	requestPacker->AddField("user_token", 'S',512);
	requestPacker->AddField("account_no", 'S',32);
	requestPacker->AddField("combi_no",   'S',8);
	requestPacker->AddField("entrust_no", 'I');	
	requestPacker->AddStr(userToken);
	requestPacker->AddStr(accountCode);
	requestPacker->AddStr(combiNo);
	requestPacker->AddInt(entrustNo);  
	requestPacker->EndPack();
	return requestPacker;
}

CErrorInfo QueryEntrusts(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo,int entrustNo, IF2UnPacker** responseUnPacker)
{
	IF2Packer* lpRequestPacker = MakeQueryEntrustsPacker(userToken,accountCode,combiNo,entrustNo);	
	//ί�в�ѯ���ܺţ�32001
	CErrorInfo errorInfo = CallService(connection,32001,lpRequestPacker,responseUnPacker);	
	lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
	lpRequestPacker->Release();
	return errorInfo;
}

IF2Packer* MakeQueryDealsPacker(const char* userToken, const char* accountCode, const char* combiNo,int entrustNo)
{
	IF2Packer* requestPacker = NewPacker(2);
	requestPacker->AddRef();
	requestPacker->BeginPack();
	requestPacker->AddField("user_token", 'S',512);
	requestPacker->AddField("account_no", 'S',32);
	requestPacker->AddField("combi_no",   'S',8);
	requestPacker->AddField("entrust_no", 'I');	
	requestPacker->AddStr(userToken);
	requestPacker->AddStr(accountCode);
	requestPacker->AddStr(combiNo);
	requestPacker->AddInt(entrustNo);  
	requestPacker->EndPack();
	return requestPacker;
}

CErrorInfo QueryDeals(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo,int entrustNo, IF2UnPacker** responseUnPacker)
{
	IF2Packer* lpRequestPacker = MakeQueryDealsPacker(userToken,accountCode,combiNo,entrustNo);	
	//ί�в�ѯ���ܺţ�33001
	CErrorInfo errorInfo = CallService(connection,33001,lpRequestPacker,responseUnPacker);	
	lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
	lpRequestPacker->Release();
	return errorInfo;
}

IF2Packer* MakeQueryAccountPacker(const char* userToken, const char* accountCode, const char* combiNo)
{
	IF2Packer* requestPacker = NewPacker(2);
	requestPacker->AddRef();
	requestPacker->BeginPack();
	requestPacker->AddField("user_token", 'S',512);
	requestPacker->AddField("account_no", 'S',32);
	requestPacker->AddField("combi_no",   'S',8);	
	requestPacker->AddStr(userToken);
	requestPacker->AddStr(accountCode);
	requestPacker->AddStr(combiNo);	
	requestPacker->EndPack();
	return requestPacker;
}

CErrorInfo QueryAccount(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo, IF2UnPacker** responseUnPacker)
{
	IF2Packer* lpRequestPacker = MakeQueryAccountPacker(userToken,accountCode,combiNo);	
	//ί�в�ѯ���ܺţ�34001
	CErrorInfo errorInfo = CallService(connection,34001,lpRequestPacker,responseUnPacker);	
	lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
	lpRequestPacker->Release();
	return errorInfo;
}

IF2Packer* MakeQueryCombiStockPacker(const char* userToken, const char* accountCode, const char* combiNo, const char* marketNo, const char* stockCode)
{
	IF2Packer* requestPacker = NewPacker(2);
	requestPacker->AddRef();
	requestPacker->BeginPack();
	requestPacker->AddField("user_token", 'S',512);
	requestPacker->AddField("account_no", 'S',32);
	requestPacker->AddField("combi_no",   'S',8);	
	requestPacker->AddField("market_no" , 'S',3);
	requestPacker->AddField("stock_code", 'S',16);
	requestPacker->AddStr(userToken);
	requestPacker->AddStr(accountCode);
	requestPacker->AddStr(combiNo);	
	requestPacker->AddStr(marketNo);	
	requestPacker->AddStr(stockCode);	
	requestPacker->EndPack();
	return requestPacker;
}

CErrorInfo QueryCombiStock(CConnectionInterface* connection,const char* userToken, const char* accountCode, const char* combiNo,const char* marketNo, const char* stockCode, IF2UnPacker** responseUnPacker)
{
	IF2Packer* lpRequestPacker = MakeQueryCombiStockPacker(userToken,accountCode,combiNo,marketNo,stockCode);		
	//ί�в�ѯ���ܺţ�34001
	CErrorInfo errorInfo = CallService(connection,34001,lpRequestPacker,responseUnPacker);	
	lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
	lpRequestPacker->Release();
	return errorInfo;
}

IF2Packer* MakeHeartBeatPacker(const char* userToken)
{
	IF2Packer* requestPacker = NewPacker(2);
	requestPacker->AddRef();
	requestPacker->BeginPack();
	requestPacker->AddField("user_token", 'S',512);
	requestPacker->AddStr(userToken);;  
	requestPacker->EndPack();
	return requestPacker;
}

struct ThreadFuncParam
{
	CConnectionInterface* connection;
	string userToken;
};
void HeartBeatThreadFunc(void* lp)
{
	ThreadFuncParam* param = (ThreadFuncParam*)lp;
	CConnectionInterface* connection = param->connection;
	IF2Packer* lpRequestPacker = MakeHeartBeatPacker(param->userToken.c_str());	
	IF2UnPacker* lpUnPacker;	
	while(true)
	{				
		//�������ܺţ�10000
		CallService(connection,10000,lpRequestPacker,&lpUnPacker);	
		Sleep(3 * 60 * 1000);
	}
	lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
	lpRequestPacker->Release();
	delete param;
}

void HeartBeat(CConnectionInterface* connection,const char* userToken)
{
	ThreadFuncParam* param = new ThreadFuncParam();
	param->connection = connection;	
	param->userToken = userToken;	
	_beginthread(HeartBeatThreadFunc,0,param);	
}


CErrorInfo QueryAccountCode(CConnectionInterface* connection, const char* userToken, const char* accountCode, IF2UnPacker** responseUnPacker,string& out_accountCode){
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token", 'S', 512);
    //requestPacker->AddField("account_no", 'S', 32);
    requestPacker->AddStr(userToken);
    //requestPacker->AddStr(accountCode);
    requestPacker->EndPack();

    //ί�в�ѯ���ܺţ�30001
    CErrorInfo errorInfo = CallService(connection, 30001, requestPacker, responseUnPacker);
    requestPacker->FreeMem(requestPacker->GetPackBuf());
    requestPacker->Release();

    if ((*responseUnPacker)->FindColIndex("account_code") >= 0)
    {
        out_accountCode = (*responseUnPacker)->GetStr("account_code");
        cout << "account_code: " << out_accountCode << endl;
    }
    else
    {
        errorInfo.ErrorCode = -100;
        errorInfo.ErrorMsg = "account_code�ֶ���Ӧ����в�����";
    }



    return errorInfo;
}
CErrorInfo QueryAssetCode(CConnectionInterface* connection, const char* userToken, const char* capitaAccount, const char* accountCode, const char *assetCode, IF2UnPacker** responseUnPacker,string &out_assetCode){
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token", 'S', 512);
    requestPacker->AddField("account_no", 'S', 32);
    requestPacker->AddStr(userToken);
    requestPacker->AddStr(accountCode);
    requestPacker->EndPack();

    //ί�в�ѯ���ܺţ�30001
    CErrorInfo errorInfo = CallService(connection, 30002, requestPacker, responseUnPacker);
    requestPacker->FreeMem(requestPacker->GetPackBuf());
    requestPacker->Release();

    if ((*responseUnPacker)->FindColIndex("asset_no") >= 0)
    {
        out_assetCode = (*responseUnPacker)->GetStr("asset_no");
        cout << "asset_no: " << out_assetCode << endl;
    }
    else
    {
        errorInfo.ErrorCode = -100;
        errorInfo.ErrorMsg = "asset_no�ֶ���Ӧ����в�����";
    }



    return errorInfo;
}
CErrorInfo QueryCombiCode(CConnectionInterface* connection, const char* userToken, const char* accountCode, const char* assetCode, const char* combiNo, IF2UnPacker** responseUnPacker,string &out_combi_no){
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token", 'S', 512);
    requestPacker->AddField("account_no", 'S', 32);
    requestPacker->AddField("asset_no", 'S', 32);

    requestPacker->AddStr(userToken);
    requestPacker->AddStr(accountCode);
    requestPacker->AddStr(assetCode);
    requestPacker->EndPack();

    //ί�в�ѯ���ܺţ�30001
    CErrorInfo errorInfo = CallService(connection, 30003, requestPacker, responseUnPacker);
    requestPacker->FreeMem(requestPacker->GetPackBuf());
    requestPacker->Release();

    if ((*responseUnPacker)->FindColIndex("combi_no") >= 0)
    {
        out_combi_no = (*responseUnPacker)->GetStr("combi_no");
        cout << "combi_no: " << out_combi_no << endl;
    }
    else
    {
        errorInfo.ErrorCode = -100;
        errorInfo.ErrorMsg = "out_combi_no�ֶ���Ӧ����в�����";
    }



    return errorInfo;
}

CErrorInfo QueryFundaset(CConnectionInterface* connection, const char* userToken, const char* accountCode, IF2UnPacker** responseUnPacker){
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token", 'S', 512);
    requestPacker->AddField("account_code", 'S', 32);


    requestPacker->AddStr(userToken);
    requestPacker->AddStr(accountCode);

    requestPacker->EndPack();

    CErrorInfo errorInfo = CallService(connection, 35003, requestPacker, responseUnPacker);
    requestPacker->FreeMem(requestPacker->GetPackBuf());
    requestPacker->Release();

    //if ((*responseUnPacker)->FindColIndex("combi_no") >= 0)
    //{
    //    out_combi_no = (*responseUnPacker)->GetStr("combi_no");
    //    cout << "combi_no: " << out_combi_no << endl;
    //}
    //else
    //{
    //    errorInfo.ErrorCode = -100;
    //    errorInfo.ErrorMsg = "out_combi_no�ֶ���Ӧ����в�����";
    //}



    return errorInfo;

}