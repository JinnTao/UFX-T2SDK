/*******************************************************************************************************/
/*
/*    1����C++ Demoʹ���첽���ͽ���ģʽ��
/*    2��Demo���ӵ��Ǻ���������Ի�������Ҫ�Խ�ȯ�̻�������Ҫ�޸�t2sdk.ini�ļ��е�IP�����֤�ļ��ȣ�
/*    3����ͬ��������֤��Ϣ���ܲ�һ�����������ӻ���ʱ����ͷ�ֶ�������Ҫȷ�ϣ�
/*    4���ӿ��ֶ�˵����ο��ӿ��ĵ�"����ͳһ����ƽ̨_�ܱ߽ӿڹ淶(��Ʊ��Ȩ).xls"��
/*    5��T2��������˵���ο������ĵ���T2SDK �ⲿ�濪��ָ��.docx"��
/*    6������UFX�ӿڼ������ʿ���ϵ����������飨261969915����
/*    7��SecuTrade.h��SecuTrade.cpp����������ͨ���ף�
/*    8��SecuMD.h��SecuMD.cpp �������Զ��ĳɽ��ر�����ί�����ƣ�
/*    9��UFX����֧����վ https://ufx.hscloud.cn/��
/*    10��demo�����ο���
/*
/*******************************************************************************************************/

#include "SecuTrade.h"
unsigned long CTradeCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CTradeCallback::AddRef()
{
    return 0;
}

unsigned long CTradeCallback::Release()
{
    return 0;
}



void CTradeCallback::OnConnect(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnConnect");
}

void CTradeCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnSafeConnect");
}

void CTradeCallback::OnRegister(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnRegister");
}

//�Ͽ����Ӻ�����Onclose��������ʾ���ӶϿ�
void CTradeCallback::OnClose(CConnectionInterface *lpConnection)
{
    puts("CTradeCallback::OnClose");
}

void CTradeCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
	puts("CTradeCallback::Onsent");
}

void CTradeCallback::Reserved1(void *a, void *b, void *c, void *d)
{
	puts("CTradeCallback::Reserved1");
}


void CTradeCallback::Reserved2(void *a, void *b, void *c, void *d)
{
	puts("CTradeCallback::Reserved2");
}

void CTradeCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
	puts("CTradeCallback::OnReceivedBizEx");
}

//�첽���ܵ��ú����������첽���Ͱ�������߳̽��ܷ��ذ�
void CTradeCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
	puts("CTradeCallback::OnReceivedBizMsg");
	if (lpMsg!=NULL)
	{
		//�ɹ�,Ӧ�ó������ͷ�lpBizMessageRecv��Ϣ
		if (lpMsg->GetReturnCode() ==0)
		{
			//���Ҫ����Ϣ�ŵ������̴߳����������п������������£�
			//int iMsgLen = 0;
			//void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
			//��lpMsgBuffer�����ߣ�Ȼ���������߳��лָ�����Ϣ�ɽ������²�����
			//lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
			//û�д�����Ϣ
			int iLen = 0;
			const void * lpBuffer = lpMsg->GetContent(iLen);
			IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
			switch(lpMsg->GetFunction())
			{
			case 331100:
				if (lpUnPacker){
					lpUnPacker->AddRef();
					ShowPacket(lpUnPacker);
					OnResponse_331100(lpUnPacker);
					lpUnPacker->Release();
				}
				break;
			case 333104:
				if (lpUnPacker){
					lpUnPacker->AddRef();
					OnResponse_333104(lpUnPacker);
					lpUnPacker->Release();
				}
				break;
			default:
				if (lpUnPacker){
					lpUnPacker->AddRef();
					ShowPacket(lpUnPacker);
					lpUnPacker->Release();
				}
				break;
			}
		}
		else
		{
			int iLen = 0;
			const void * lpBuffer = lpMsg->GetContent(iLen);
			IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer, iLen);
			if (lpUnPacker != NULL)
			{
				lpUnPacker->AddRef();//����ͷ��ڴ�����
				ShowPacket(lpUnPacker);
				lpUnPacker->Release();
			}
			else
			{
				printf("ҵ����ǿհ���������룺%d��������Ϣ:%s\n", lpMsg->GetErrorNo(), lpMsg->GetErrorInfo());
			}
		}
	}
}
void CTradeCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{

}

int  CTradeCallback::Reserved3()
{
    return 0;
}

void CTradeCallback::Reserved4()
{
}

void CTradeCallback::Reserved5()
{
}

void CTradeCallback::Reserved6()
{
}

void CTradeCallback::Reserved7()
{
}

void CTradeCallback::SetRequestMode(SecuRequestMode* lpMode)
{
	lpReqMode = lpMode; 
	  puts("CTradeCallback::SetRequestMode");
}

void CTradeCallback::OnResponse_331100(IF2UnPacker *lpUnPacker)
{   
	int iSystemNo=-1;
	puts("CTradeCallBack::331100");
	// cout<<"CompanyID:"<<lpUnPacker->GetStr("company_id");
	/*lpReqMode->m_BranchNo = lpUnPacker->GetInt("branch_no");
	cout<<lpReqMode->m_BranchNo;*/
	const char *pClientId = lpUnPacker->GetStr("client_id");
	if(pClientId)
	strcpy(lpReqMode->m_client_id, pClientId);
	//cout<<"clientId:"<<lpReqMode->m_client_id;
	if(lpUnPacker->GetStr("user_token") != NULL)
		{
			strcpy(lpReqMode->m_opUserToken , lpUnPacker->GetStr("user_token"));
	    }
	//cout<<"UserToken:"<<lpReqMode->m_opUserToken<<endl;
	if(lpUnPacker->GetStr("branch_no") != NULL)
		lpReqMode->m_BranchNo = lpUnPacker->GetInt("branch_no");
	//cout<<"BranchNo:"<<lpUnPacker->GetStr("branch_no")<<endl;
	lpReqMode->iSystemNo = lpUnPacker->GetInt("sysnode_id");
	cout<<"iSystemNo:"<<lpReqMode->iSystemNo<<endl;
	if (lpUnPacker->GetInt("op_branch_no")!=NULL)
		lpReqMode->m_op_branch_no=lpUnPacker->GetInt("op_branch_no");
	    cout<<"op_branch_no:"<<lpReqMode->m_op_branch_no<<endl;
    //����ϵͳ��
			//iSystemNo = lpUnPacker->GetInt("sysnode_id");
	return;
}

void CTradeCallback::OnResponse_333104(IF2UnPacker *lpUnPacker)
{
if (lpUnPacker->GetInt("error_no") != 0)
	{
		cout<<lpUnPacker->GetStr("error_info")<<endl;
		return;
	}
	else
	{
		int ct = lpUnPacker->GetRowCount();
		string pos = "";
		while (!lpUnPacker->IsEOF())
		{
			const char* lpStrPos = lpUnPacker->GetStr("position_str");
			if( lpStrPos == 0)
				pos = "";
			else
				pos = lpStrPos;
		
			lpUnPacker->Next();
		}

		if( pos.length() != 0)
		{
				ShowPacket(lpUnPacker);
			lpReqMode->ReqFunction333104( pos.c_str());
		}
	}

	return;
}


int SecuRequestMode::InitConn()
{   
	lpConfig->Load("t2sdk.ini");
	const char *p_fund_account = lpConfig->GetString("ufx", "fund_account", "");
	const char *p_password = lpConfig->GetString("ufx", "password", "");
	strcpy(m_AccountName,p_fund_account);
	strcpy(m_Password,p_password);
	m_EntrustWay='7';
	
	//�������Ӷ���
	//lpConfig->SetString("t2sdk", "servers", serverAddr);
	////cout<<"start checking license"<<endl;
	//lpConfig->SetString("t2sdk", "license_file", licFile);
	////cout<<"start loading clientname"<<endl;
	//lpConfig->SetString("t2sdk", "login_name", clientName);
	//cout<<"Connect Successful"<<endl;

	//�������ar������safe_level������Ҫ�����´���
	//begin
	//lpConfig->SetString("safe", "safe_level", "ssl");
	//lpConfig->SetString("safe", "cert_file", "c20121011.pfx");
	//lpConfig->SetString("safe", "cert_pwd", "111111");
	//end

	int iRet = 0;
		

	if(lpConnection != NULL)
	{
		lpConnection->Release();
		lpConnection = NULL;
	}
	
	lpConnection = NewConnection(lpConfig);
	//cout<<"Connected successfully"<<endl;
	lpConnection->AddRef();
	if (0 != (iRet = lpConnection->Create2BizMsg(&callback)))
    {
		cerr<<"��ʼ��ʧ��.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
		return -1;
	}
	if (0 != (iRet = lpConnection->Connect(3000)))
	{
		cerr<<"����.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
		return -1;
	}

	


	return 0;
}

unsigned long SecuRequestMode::Release()
{
	delete this;
	return 0;
};


//331100֤ȯ�ͻ���½
int SecuRequestMode::ReqFunction331100()
{   
	int iRet=0,hSend=0, iLen=0;
   // cout<<"password"<<m_Password<<endl;
	//EncodeEx(m_Password,pout);
	//cout<<"pout: "<<pout<<endl;
	cout<<"331100�ͻ���½������£�"<<endl;
	
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(331100);
	
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	IF2Packer *pPacker=NewPacker(2);
	
	if (!pPacker)
	{
	  printf("ȡ�����ʧ�ܣ�\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//������֧����
	pPacker->AddField("op_entrust_way", 'C', 1);//ί�з�ʽ 
	pPacker->AddField("op_station", 'S', 255);//վ���ַ
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("input_content", 'C'); 
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type",'S', 6);  
	pPacker->AddField("password",'S', 10);
	pPacker->AddField("password_type", 'C'); 
	//pPacker->AddField("asset_prop",'C');
	      
	  
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(0);						
	pPacker->AddChar(m_EntrustWay);				
	pPacker->AddStr(m_opStation.c_str());				
	pPacker->AddInt(22);
	pPacker->AddChar('1');					
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr("0");	
	pPacker->AddStr(m_Password);
	//pPacker->AddStr(pout);
	pPacker->AddChar('1');	
	//pPacker->AddChar('0');
	
	
	
	///�������
	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//��ӡ�����Ϣ
	/*IF2UnPacker * lpUnPacker = NewUnPacker(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpUnPacker->AddRef();
	ShowPacket(lpUnPacker);
	lpUnPacker->Release();

	//�첽ģʽ��½
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	*/
	//ͬ��ģʽ��½ 
	hSend=lpConnection->SendBizMsg(lpBizMessage, 0);

	IBizMessage* lpBizMessageRecv = NULL;

	iRet = lpConnection->RecvBizMsg(hSend, &lpBizMessageRecv, 5000);
	const void * lpBuffer = lpBizMessageRecv->GetContent(iLen);
	IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
	ShowPacket(lpUnPacker);
	pPacker->Release();
	lpBizMessage->Release();
	return 0;

}



//400֤ȯ�����ѯ 
int SecuRequestMode::ReqFunction400(char* exchange_type, char* stock_code)
{   
	cout<<"400֤ȯ�����ѯ���:"<<endl;
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(400);
	
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
	IF2Packer *pPacker=NewPacker(2);
	if (!pPacker)
	{
	  printf("ȡ�����ʧ�ܣ�\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	//�����ֶ���
	pPacker->AddField("exchange_type",'S');
	pPacker->AddField("stock_code",'S');

    //�����ֶ�ֵ
	pPacker->AddStr(exchange_type);
	pPacker->AddStr(stock_code);


	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//��ӡ�����Ϣ 
	IF2UnPacker * lpUnPacker = NewUnPacker(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpUnPacker->AddRef();
	ShowPacket(lpUnPacker);
	lpUnPacker->Release();

    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
	return 0;
}


//֤ȯ��ͨί��
int SecuRequestMode::ReqFunction333002(char* exchange_type,char* stock_code, double entrust_amount,double entrust_price,char entrust_bs)
{   
	cout<<"333002ί���������"<<endl;
	int iRet = 0, hSend = 0;	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///Ӧ��ҵ����Ϣ
	IBizMessage* lpBizMessageRecv = NULL;
	//���ܺ�
	lpBizMessage->SetFunction(333002);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	//lpBizMessage->SetSystemNo(iSystemNo);
	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//�ͻ�ID
	pPacker->AddField("fund_account", 'S', 18);//�ʽ��˺�
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);	
	//pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_account", 'S', 15);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_amount", 'F', 19, 2);
	pPacker->AddField("entrust_price", 'F', 18, 3);
	pPacker->AddField("entrust_bs", 'C', 1);
	pPacker->AddField("entrust_prop", 'S', 3);	
	pPacker->AddField("batch_no", 'I', 8);
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);					
	pPacker->AddStr(m_opStation.c_str());					
	pPacker->AddInt(m_BranchNo);				
	pPacker->AddStr(m_client_id);			 
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr(m_Password);				
	pPacker->AddChar('2');					
	//pPacker->AddStr(m_opUserToken);			
	pPacker->AddStr(exchange_type);					
	pPacker->AddStr("");
	pPacker->AddStr(stock_code);				
	pPacker->AddDouble(entrust_amount);
	pPacker->AddDouble(entrust_price);		
	pPacker->AddChar(entrust_bs);					
	pPacker->AddStr("0");					
	pPacker->AddInt(13);						
	
	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//��ӡ�����Ϣ 
	IF2UnPacker * lpUnPacker = NewUnPacker(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpUnPacker->AddRef();
	ShowPacket(lpUnPacker);
	lpUnPacker->Release();

    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
	return 0;
		
}

//ί�г���

int SecuRequestMode::ReqFunction333017()
{
	int iRet = 0, hSend = 0;	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///Ӧ��ҵ����Ϣ
	IBizMessage* lpBizMessageRecv = NULL;
	//���ܺ�
	lpBizMessage->SetFunction(333017);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//�ͻ�ID
	pPacker->AddField("fund_account", 'S', 18);//�ʽ��˺�
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);	
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("batch_flag", 'S', 15);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("entrust_no", 'S', 6);
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);					
	pPacker->AddStr(m_opStation.c_str());					
	pPacker->AddInt(m_BranchNo);				
	pPacker->AddStr(m_client_id);			 
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr(m_Password);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_opUserToken);			
	pPacker->AddStr("");					
	pPacker->AddStr("");
	pPacker->AddStr("");				
							
	
	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
	return 0;
		
}

//֤ȯί�в�ѯ
int SecuRequestMode::ReqFunction333101()
  {
	  cout<<"333101ί�в�ѯ���:"<<endl;
	   int iRet = 0, hSend = 0;	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///Ӧ��ҵ����Ϣ
	IBizMessage* lpBizMessageRecv = NULL;
	//���ܺ�
	lpBizMessage->SetFunction(333101);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//�ͻ�ID
	pPacker->AddField("fund_account", 'S', 18);//�ʽ��˺�
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);	
	pPacker->AddField("user_token", 'S', 40);
	//pPacker->AddField("exchange_type", 'S', 4);
	//pPacker->AddField("stcok_account",'S',11);
	//pPacker->AddField("stock_code",'S',16);
	//pPacker->AddField("sort_direction",'C',1);
	//pPacker->AddField("report_no",'I',8);
	//pPacker->AddField("action_in",'I',5);
	//pPacker->AddField("locate_entrust_no",'I',8);
	//pPacker->AddField("query_type",'C',1);
	//pPacker->AddField("query_mode",'C',1);
	//pPacker->AddField("position_str",'S',100);
	//pPacker->AddField("request_num",'N',10);
	//pPacker->AddField("etf_flag",'C',1);

	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);					
	pPacker->AddStr(m_opStation.c_str());					
	pPacker->AddInt(m_BranchNo);				
	pPacker->AddStr(m_client_id);			 
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr(m_Password);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_opUserToken);			
	//pPacker->AddStr("");					
	//pPacker->AddStr("");
	//pPacker->AddStr("");	
	//pPacker->AddChar(' ');
	//pPacker->AddInt( );
	//pPacker->AddInt( );
	//pPacker->AddInt( );
	//pPacker->AddChar(' ');
	//pPacker->AddChar(' ');
	//pPacker->AddStr("");
	//pPacker->AddInt();
	//pPacker->AddChar(' ');
	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());

	//��ӡ�����Ϣ 
	IF2UnPacker * lpUnPacker = NewUnPacker(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpUnPacker->AddRef();
	ShowPacket(lpUnPacker);
	lpUnPacker->Release();

    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
	return 0;
}

//֤ȯ�ɽ���ѯ
int SecuRequestMode::ReqFunction333102()
{
	 cout<<"333102֤ȯ�ɽ���ѯ���"<<endl;
	 int iRet = 0, hSend = 0;	
	///��ȡ�汾Ϊ2���͵�pack�����
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("ȡ�����ʧ��!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///��������
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///Ӧ��ҵ����Ϣ
	IBizMessage* lpBizMessageRecv = NULL;
	//���ܺ�
	lpBizMessage->SetFunction(333102);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
	///������Ӧ����Ϣ
	LPRET_DATA pRetData = NULL;
	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//�ͻ�ID
	pPacker->AddField("fund_account", 'S', 18);//�ʽ��˺�
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);	
	pPacker->AddField("user_token", 'S', 40);
	//pPacker->AddField("exchange_type", 'S', 4);
	//pPacker->AddField("stcok_account",'S',11);
	//pPacker->AddField("stock_code",'S',16);
	//pPacker->AddField("serial_no",'I',8);
	//pPacker->AddField("query_direction",'C',1);
	//pPacker->AddField("report_no",'I',8);
	//pPacker->AddField("query_type",'C',1);
	//pPacker->AddField("query_mode",'C',1);
	//pPacker->AddField("position_str",'S',100);
	//pPacker->AddField("request_num",'N',10);
	//pPacker->AddField("etf_flag",'C',1);

	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);					
	pPacker->AddStr(m_opStation.c_str());					
	pPacker->AddInt(m_BranchNo);				
	pPacker->AddStr(m_client_id);			 
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr(m_Password);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_opUserToken);			
	//pPacker->AddStr("");					
	//pPacker->AddStr("");
	//pPacker->AddStr("");	
	//pPacker->AddInt();
	//pPacker->AddChar(' ');
	//pPacker->AddInt( );
	//pPacker->AddChar(' ');
	//pPacker->AddChar(' ');
	//pPacker->AddStr("");
	//pPacker->AddInt();
	//pPacker->AddChar(' ');
	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//��ӡ�����Ϣ 
	IF2UnPacker * lpUnPacker = NewUnPacker(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpUnPacker->AddRef();
	ShowPacket(lpUnPacker);
	lpUnPacker->Release();

    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
	return 0;

}


//֤ȯ�ֲֲ�ѯ
int SecuRequestMode::ReqFunction333104(const char* position_str)
{
    	
	cout<<"333104֤ȯ�ֲֲ�ѯ���:"<<endl;
	
	 IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		
		
		IBizMessage* lpBizMessageRecv = NULL;
		
		//���ܺ�
		lpBizMessage->SetFunction(333104);
		//��������
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		//����ϵͳ��
		lpBizMessage->SetSystemNo(iSystemNo);
	
		
		
		///��ȡ�汾Ϊ2���͵�pack�����
		IF2Packer *pPacker = NewPacker(2);
		if(!pPacker)
		{
			printf("ȡ�����ʧ��!\r\n");
			return -1;
		}
		pPacker->AddRef();
		
		///��ʼ���
		pPacker->BeginPack();
		
		///�����ֶ���
		pPacker->AddField("op_branch_no", 'I', 5);
		pPacker->AddField("op_entrust_way", 'C', 1);
		pPacker->AddField("op_station", 'S', 255);
		pPacker->AddField("branch_no", 'I', 5); 		
		pPacker->AddField("client_id", 'S', 18);			
		pPacker->AddField("fund_account", 'S', 18); 	
		pPacker->AddField("password", 'S', 10); 					
		pPacker->AddField("password_type", 'C', 1); 	
		pPacker->AddField("user_token", 'S', 512);		
		pPacker->AddField("position_str",'S',100);
	    pPacker->AddField("request_num",'N',10);
		
		///�����Ӧ���ֶ�ֵ
		pPacker->AddInt(0); 			//	op_branch_no	
		pPacker->AddChar('7');			//		op_entrust_way
		pPacker->AddStr("hs");		//op_station
		pPacker->AddInt(m_BranchNo);	//	branch_no		
		pPacker->AddStr(m_client_id);	//		client_id
		pPacker->AddStr(m_AccountName);	//	fund_account	
		pPacker->AddStr(m_Password);		//	password	
		pPacker->AddChar('1');				//	password_type
		pPacker->AddStr(m_opUserToken);	//user_token
		pPacker->AddStr( position_str);
	    pPacker->AddInt(3);

	
		///�������
		pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//֤ȯ�ɶ���Ϣ��ѯ 
int SecuRequestMode::ReqFunction331300()
{
	 cout<<"isysnum:"<<iSystemNo<<endl;
	 cout<<"password:"<<m_Password<<endl;
	  cout<<"fund_account:"<<m_AccountName<<endl;
	 cout<<"m_opUserToken:"<<m_opUserToken<<endl;
	int hSend = 0;
	
		IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		
		
		IBizMessage* lpBizMessageRecv = NULL;
		
		//���ܺ�
		lpBizMessage->SetFunction(331300);
		//��������
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		//����Ӫҵ����
		//����ϵͳ��
		lpBizMessage->SetSystemNo(iSystemNo);
	
		
		///��ȡ�汾Ϊ2���͵�pack�����
		IF2Packer *pPacker = NewPacker(2);
		if(!pPacker)
		{
			printf("ȡ�����ʧ��!\r\n");
			return -1;
		}
		pPacker->AddRef();
		
		///��ʼ���
		pPacker->BeginPack();
		
		///�����ֶ���
		pPacker->AddField("op_branch_no", 'I', 5);//���� ���� ����
		pPacker->AddField("op_entrust_way", 'C', 1);
		pPacker->AddField("op_station", 'S', 255);
		pPacker->AddField("branch_no", 'I', 5); 
		pPacker->AddField("client_id", 'S', 18);
		pPacker->AddField("fund_account", 'S', 18);
		pPacker->AddField("password", 'S', 10);
		pPacker->AddField("password_type", 'C', 1); 
		pPacker->AddField("user_token", 'S', 512);
		pPacker->AddField("exchange_type", 'S', 4);

	
		
		///�����Ӧ���ֶ�ֵ
		pPacker->AddInt(0);			//op_branch_no			
		pPacker->AddChar('7');			//	op_entrust_way	
		pPacker->AddStr("hs");	//op_station				
		pPacker->AddInt(m_BranchNo);		//	branch_no	
		pPacker->AddStr(m_client_id);	//		client_id
		pPacker->AddStr(m_AccountName);	//	fund_account	
		pPacker->AddStr(m_Password);		//password		
		pPacker->AddChar('2');			//	password_type	
		pPacker->AddStr(m_opUserToken);	//	user_token	
		pPacker->AddStr("1");			//exchange_type		


	
		///�������
		pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();

}