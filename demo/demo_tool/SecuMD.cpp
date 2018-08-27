/*******************************************************************************************************/
/*
/*    1、此C++ Demo使用异步发送接收模式；
/*    2、Demo连接的是恒生仿真测试环境，如要对接券商环境，需要修改t2sdk.ini文件中的IP和许可证文件等；
/*    3、不同环境的验证信息可能不一样，更换连接环境时，包头字段设置需要确认；
/*    4、接口字段说明请参考接口文档"恒生统一接入平台_周边接口规范(UF2.0).xls"；
/*    5、T2函数技术说明参考开发文档“T2SDK 外部版开发指南.docx"；
/*    6、如有UFX接口技术疑问可联系大金融讨论组（261969915）；
/*    7、SecuTrade.h和SecuTrade.cpp用来测试普通交易；
/*    8、SecuMD.h和SecuMD.cpp 用来测试订阅成交回报主推委托主推；
/*    9、UFX技术支持网站 https://ufx.hscloud.cn/；
/*    10、demo仅供参考。
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

// 以下各回调方法的实现仅仅为演示使用
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
		//成功,应用程序不能释放lpBizMessageRecv消息
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
							puts("订阅成功...");
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
							printf("该信息为证券成交主推信息！\n");
							ShowPacket(lpUnPacker_key);
							lpUnPacker_key->Release();
						}
						if (lpMsg->GetIssueType() == ISSUE_TYPE_ENTR_BACK){
							printf("该信息为证券委托主推信息！\n");
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
				lpUnPacker->AddRef();//添加释放内存引用
				ShowPacket(lpUnPacker);
				lpUnPacker->Release();
			}
			else
			{
				printf("业务包是空包，错误代码：%d，错误信息:%s\n", lpMsg->GetErrorNo(), lpMsg->GetErrorInfo());
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
	//加载配置文件登陆 
	lpConfig->Load("t2sdk.ini");
	const char *p_fund_account = lpConfig->GetString("ufx", "fund_account", "");
	const char *p_password = lpConfig->GetString("ufx", "password", "");
	strcpy(m_AccountName,p_fund_account);
	strcpy(m_Password,p_password);
	m_EntrustWay='7';
	//cout<<"start initializing serverAddr"<<endl;
	//配置连接对象
	//lpConfig->SetString("t2sdk", "servers", serverAddr);
	////cout<<"start checking license"<<endl;
	//lpConfig->SetString("t2sdk", "license_file", licFile);
	////cout<<"start loading clientname"<<endl;
	//lpConfig->SetString("t2sdk", "login_name", clientName);
	//cout<<"Connect Successful"<<endl;

	//如果接入ar设置了safe_level，则需要做以下代码
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
		cerr<<"初始化失败.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
		return -1;
	}
	if (0 != (iRet = lpConnection->Connect(5000)))
	{
		cerr<<"连接.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
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

//331100 登入
int CMdRequestMode::ReqFunction331100()
{
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(331100);
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	IF2Packer *pPacker=NewPacker(2);
	if (!pPacker)
	{
	  printf("取打包器失败！\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
	pPacker->AddField("op_entrust_way", 'C', 1);//委托方式 
	pPacker->AddField("op_station", 'S', 255);//站点地址
	pPacker->AddField("branch_no", 'I', 5);     
	pPacker->AddField("input_content", 'C'); 
	pPacker->AddField("account_content", 'S', 30);
	pPacker->AddField("content_type", 'S', 6);  
	pPacker->AddField("password", 'S', 10);      
	pPacker->AddField("password_type", 'C');   
	///加入对应的字段值
	pPacker->AddInt(m_op_branch_no);						
	pPacker->AddChar(m_EntrustWay);				
	pPacker->AddStr(m_opStation.c_str());				
	pPacker->AddInt(22);			
	pPacker->AddChar('1');					
	pPacker->AddStr(m_AccountName);			
	pPacker->AddStr("0");	
	pPacker->AddStr(m_Password);			
	pPacker->AddChar('2');	
	
	///结束打包
	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//620001_12订阅证券成交回报
int CMdRequestMode::SubFunction12(int issue_type)
{
  int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///应答业务消息
	IBizMessage* lpBizMessageRecv = NULL;
	//功能号
	lpBizMessage->SetFunction(MSGCENTER_FUNC_REG);
	lpBizMessage->SetSequeceNo(12);
	//请求类型
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);  
	
	///加入对应的字段值
	pPacker->AddInt(m_BranchNo);					
	pPacker->AddStr(m_AccountName);				
	pPacker->AddInt(m_op_branch_no);							
	pPacker->AddChar(m_EntrustWay);	
	pPacker->AddStr(m_opStation.c_str());	//op_station
	pPacker->AddStr(m_client_id);		
	pPacker->AddStr(m_Password);					
	pPacker->AddStr(m_opUserToken.c_str());		
	pPacker->AddInt(issue_type);			
	///结束打包
	pPacker->EndPack();
	lpBizMessage->SetKeyInfo(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpConnection->SendBizMsg(lpBizMessage,1);
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//62001_23 订阅证券委托回报
 int CMdRequestMode::SunFunction23(int issue_type)
 {
	 int iRet = 0, hSend = 0;
	IF2UnPacker *pMCUnPacker = NULL;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///应答业务消息
	IBizMessage* lpBizMessageRecv = NULL;
	//功能号
	lpBizMessage->SetFunction(MSGCENTER_FUNC_REG);
	lpBizMessage->SetSequeceNo(13);
	//请求类型
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);		
	//pPacker->AddField("op_branch_no", 'I', 5);	
	//pPacker->AddField("op_entrust_way",'C',1);
	//pPacker->AddField("op_station", 'S', 255);
	//pPacker->AddField("client_id",'S',18);
	//pPacker->AddField("password",'S',10);
	//pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);  
	
	///加入对应的字段值
	pPacker->AddInt(m_BranchNo);					
	pPacker->AddStr(m_AccountName);				
	//pPacker->AddInt(m_op_branch_no);							
	//pPacker->AddChar(m_EntrustWay);	
	//pPacker->AddStr(m_opStation.c_str());	//op_station
	//pPacker->AddStr(m_client_id);		
	//pPacker->AddStr(m_Password);					
	//pPacker->AddStr(m_opUserToken.c_str());		
	pPacker->AddInt(issue_type);			
	///结束打包
	pPacker->EndPack();
	lpBizMessage->SetKeyInfo(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpConnection->SendBizMsg(lpBizMessage,1);
	pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release(); 
 }

 //证券普通委托
int CMdRequestMode::ReqFunction333002()
{   
	int iRet = 0, hSend = 0;	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	//IF2UnPacker *pUnPacker = NULL;
	
	IBizMessage* lpBizMessage = NewBizMessage();

	lpBizMessage->AddRef();
	
	///应答业务消息
	IBizMessage* lpBizMessageRecv = NULL;
	//功能号
	lpBizMessage->SetFunction(333002);
	//请求类型
	lpBizMessage->SetPacketType(REQUEST_PACKET);

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//客户ID
	pPacker->AddField("fund_account", 'S', 18);//资金账号
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
	
	///加入对应的字段值
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