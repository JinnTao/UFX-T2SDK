/*******************************************************************************************************/
/*
/*    1����C++ Demoʹ���첽���ͽ���ģʽ��
/*    2��Demo���ӵ��Ǻ���������Ի�������Ҫ�Խ�ȯ�̻�������Ҫ�޸�t2sdk.ini�ļ��е�IP�����֤�ļ��ȣ�
/*    3����ͬ��������֤��Ϣ���ܲ�һ�����������ӻ���ʱ����ͷ�ֶ�������Ҫȷ�ϣ�
/*    4���ӿ��ֶ�˵����ο��ӿ��ĵ�"����ͳһ����ƽ̨_�ܱ߽ӿڹ淶(UF2.0).xls"��
/*    5��T2��������˵���ο������ĵ���T2SDK �ⲿ�濪��ָ��.docx"��
/*    6������UFX�ӿڼ������ʿ���ϵ����������飨261969915����
/*    7��SecuTrade.h��SecuTrade.cpp����������ͨ���ף�
/*    8��SecuMD.h��SecuMD.cpp �������Զ��ĳɽ��ر�����ί�����ƣ�
/*    9��UFX����֧����վ https://ufx.hscloud.cn/��
/*    10��demo�����ο���
/*
/*******************************************************************************************************/
#include "SecuMD.h"

unsigned long CMdCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CMdCallback::AddRef()
{
    return 0;
}

unsigned long CMdCallback::Release()
{
    return 0;
}

// ���¸��ص�������ʵ�ֽ���Ϊ��ʾʹ��
void CMdCallback::OnConnect(CConnectionInterface *lpConnection)
{
    puts("CMdCallback::OnConnect");
}

void CMdCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("CMdCallback::OnSafeConnect");
}

void CMdCallback::OnRegister(CConnectionInterface *lpConnection)
{
    puts("CMdCallback::OnRegister");
}

void CMdCallback::OnClose(CConnectionInterface *lpConnection)
{
    puts("CMdCallback::OnClose");
}

void CMdCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
}

void CMdCallback::Reserved1(void *a, void *b, void *c, void *d)
{
}

void CMdCallback::Reserved2(void *a, void *b, void *c, void *d)
{
}

void CMdCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
	
}
void CMdCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{   
	if (lpMsg!=NULL)
	{
		//�ɹ�,Ӧ�ó������ͷ�lpBizMessageRecv��Ϣ
		if (lpMsg->GetReturnCode() ==0)
		{
			int iLen = 0;
			const void * lpBuffer = lpMsg->GetContent(iLen);
			IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
			
			int iLen_ley = 0;
			const void * lpBuffer_key = lpMsg->GetKeyInfo(iLen_ley);
			IF2UnPacker * lpUnPacker_key = NewUnPacker((void *)lpBuffer_key,iLen_ley);
			
			switch(lpMsg->GetFunction())
			{
				case 331100:
				{
					int iLen = 0;
					const void * lpBuffer = lpMsg->GetContent(iLen);
					IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer, iLen);
					if (lpUnPacker){
						lpUnPacker->AddRef();
						ShowPacket(lpUnPacker);
						OnResponse_331100(lpUnPacker);
						lpUnPacker->Release();
					}
					break;
				}
				case 333002:
				{
					int iLen = 0;
					const void * lpBuffer = lpMsg->GetContent(iLen);
					IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer, iLen);
					if (lpUnPacker){
						lpUnPacker->AddRef();
						ShowPacket(lpUnPacker);
						lpUnPacker->Release();
					}
							   break;
				}
				case MSGCENTER_FUNC_HEART:
				{
					if (lpMsg->GetPacketType() == REQUEST_PACKET)
						cout << "heartBeat.." << endl;
					lpReqMode->OnHeartbeat(lpMsg);
					break;
				}
				case MSGCENTER_FUNC_REG:
				{
					int iLen_ley = 0;
					const void * lpBuffer_key = lpMsg->GetKeyInfo(iLen_ley);
					IF2UnPacker * lpUnPacker_key = NewUnPacker((void *)lpBuffer_key, iLen_ley);
					if (lpUnPacker_key){
						lpUnPacker_key->AddRef();
						if (lpUnPacker_key->GetInt("error_no") != 0){
							puts(lpUnPacker_key->GetStr("error_info"));
							lpUnPacker_key->Release();
						}
						else{
							puts("���ĳɹ�...");
							ShowPacket(lpUnPacker_key);
							lpUnPacker_key->Release();
						}
					}
					break;
				}
				case MSGCENTER_FUNC_REG_CANCLE:
					break;
				case MSGCENTER_FUNC_SENDED:
				{   
					int iLen_ley = 0;
					const void * lpBuffer_key = lpMsg->GetKeyInfo(iLen_ley);
					IF2UnPacker * lpUnPacker_key = NewUnPacker((void *)lpBuffer_key, iLen_ley);
					if (lpUnPacker_key){
						lpUnPacker_key->AddRef();
						if (lpMsg->GetIssueType() == ISSUE_TYPE_REALTIME_SECU){
							printf("����ϢΪ֤ȯ�ɽ�������Ϣ��\n");
							ShowPacket(lpUnPacker_key);
							lpUnPacker_key->Release();
						}
						if (lpMsg->GetIssueType() == ISSUE_TYPE_ENTR_BACK){
							printf("����ϢΪ֤ȯί��������Ϣ��\n");
							ShowPacket(lpUnPacker_key);
							lpUnPacker_key->Release();
						}
							
					}
					break;			
                 }
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

void CMdCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{

}

int  CMdCallback::Reserved3()
{
    return 0;
}

void CMdCallback::Reserved4()
{
}

void CMdCallback::Reserved5()
{
}

void CMdCallback::Reserved6()
{
}

void CMdCallback::Reserved7()
{
}

void CMdCallback::SetRequestMode(CMdRequestMode* lpMode)
{
	lpReqMode = lpMode; 
}

void CMdCallback::OnResponse_331100(IF2UnPacker *lpUnPacker)
{
	int iSystemNo=-1;
	puts("CTradeCallBack::331100");
	/*lpReqMode->m_BranchNo = lpUnPacker->GetInt("branch_no");
	cout<<lpReqMode->m_BranchNo;*/
	const char *pClientId = lpUnPacker->GetStr("client_id");
	if(pClientId)
	strcpy(lpReqMode->m_client_id, pClientId);
	cout<<"clientId:"<<lpReqMode->m_client_id;
	if(lpUnPacker->GetStr("user_token") != NULL)
		{
			lpReqMode->m_opUserToken = lpUnPacker->GetStr("user_token");
	    }
	cout<<"UserToken:"<<lpUnPacker->GetStr("user_token")<<endl;
	if(lpUnPacker->GetStr("branch_no") != NULL)
		lpReqMode->m_BranchNo = lpUnPacker->GetInt("branch_no");
	cout<<"BranchNo:"<<lpUnPacker->GetStr("branch_no")<<endl;
	iSystemNo = lpUnPacker->GetInt("sysnode_id");
	cout<<"iSystemNo:"<<lpUnPacker->GetInt("sysnode_id")<<endl;

	return;
}

int CMdRequestMode::InitConn()
{   
	//���������ļ���½ 
	lpConfig->Load("t2sdk.ini");
	const char *p_fund_account = lpConfig->GetString("ufx", "fund_account", "");
	const char *p_password = lpConfig->GetString("ufx", "password", "");
	strcpy(m_AccountName,p_fund_account);
	strcpy(m_Password,p_password);
	m_EntrustWay='7';
	//cout<<"start initializing serverAddr"<<endl;
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
	if (0 != (iRet = lpConnection->Connect(5000)))
	{
		cerr<<"����.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
		return -1;
	}

	return 0;
}

unsigned long CMdRequestMode::Release()
{
	delete this;
	return 0;
};
void CMdRequestMode::OnHeartbeat(IBizMessage* lpMsg)
{
	lpMsg->ChangeReq2AnsMessage();

	lpConnection->SendBizMsg(lpMsg, 1);
	return;
}

//331100 ����
int CMdRequestMode::ReqFunction331100()
{
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
	pPacker->AddField("content_type", 'S', 6);  
	pPacker->AddField("password", 'S', 10);      
	pPacker->AddField("password_type", 'C');   
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);				
	pPacker->AddStr(m_opStation.c_str());				
	pPacker->AddInt(22);			
	pPacker->AddChar('1');					
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr("0");	
	pPacker->AddStr(m_Password);			
	pPacker->AddChar('2');	
	
	///�������
	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//620001_12����֤ȯ�ɽ��ر�
int CMdRequestMode::SubFunction12(int issue_type)
{
  int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
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
	lpBizMessage->SetFunction(MSGCENTER_FUNC_REG);
	lpBizMessage->SetSequeceNo(12);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);  
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_BranchNo);					
	pPacker->AddStr(m_AccountName);				
	pPacker->AddInt(m_op_branch_no);							
	pPacker->AddChar(m_EntrustWay);	
	pPacker->AddStr(m_opStation.c_str());	//op_station
	pPacker->AddStr(m_client_id);		
	pPacker->AddStr(m_Password);					
	pPacker->AddStr(m_opUserToken.c_str());		
	pPacker->AddInt(issue_type);			
	///�������
	pPacker->EndPack();
	lpBizMessage->SetKeyInfo(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpConnection->SendBizMsg(lpBizMessage,1);
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//62001_23 ����֤ȯί�лر�
 int CMdRequestMode::SunFunction23(int issue_type)
 {
	 int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
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
	lpBizMessage->SetFunction(MSGCENTER_FUNC_REG);
	lpBizMessage->SetSequeceNo(13);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	///��ʼ���
	pPacker->BeginPack();
	
	///�����ֶ���
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);		
	//pPacker->AddField("op_branch_no", 'I', 5);	
	//pPacker->AddField("op_entrust_way",'C',1);
	//pPacker->AddField("op_station", 'S', 255);
	//pPacker->AddField("client_id",'S',18);
	//pPacker->AddField("password",'S',10);
	//pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);  
	
	///�����Ӧ���ֶ�ֵ
	pPacker->AddInt(m_BranchNo);					
	pPacker->AddStr(m_AccountName);				
	//pPacker->AddInt(m_op_branch_no);							
	//pPacker->AddChar(m_EntrustWay);	
	//pPacker->AddStr(m_opStation.c_str());	//op_station
	//pPacker->AddStr(m_client_id);		
	//pPacker->AddStr(m_Password);					
	//pPacker->AddStr(m_opUserToken.c_str());		
	pPacker->AddInt(issue_type);			
	///�������
	pPacker->EndPack();
	lpBizMessage->SetKeyInfo(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpConnection->SendBizMsg(lpBizMessage,1);
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release(); 
 }

 //֤ȯ��ͨί��
int CMdRequestMode::ReqFunction333002()
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
	lpBizMessage->SetFunction(333002);
	//��������
	lpBizMessage->SetPacketType(REQUEST_PACKET);

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
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_account", 'S', 15);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_amount", 'F', 19, 2);
	pPacker->AddField("entrust_price", 'S', 18);
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
	pPacker->AddStr(m_opUserToken.c_str());			
	pPacker->AddStr("1");					
	pPacker->AddStr(" ");
	pPacker->AddStr("600570");				
	pPacker->AddDouble(100);
	pPacker->AddStr(" ");		
	pPacker->AddChar('1');					
	pPacker->AddStr("U");					
	pPacker->AddInt(13);						
	
	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}