/*******************************************************************************************************/
/*
/*    1、此C++ Demo使用异步发送接收模式；
/*    2、Demo连接的是恒生仿真测试环境，如要对接券商环境，需要修改t2sdk.ini文件中的IP和许可证文件等；
/*    3、不同环境的验证信息可能不一样，更换连接环境时，包头字段设置需要确认；
/*    4、接口字段说明请参考接口文档"恒生统一接入平台_周边接口规范(股票期权).xls"；
/*    5、T2函数技术说明参考开发文档“T2SDK 外部版开发指南.docx"；
/*    6、如有UFX接口技术疑问可联系大金融讨论组（261969915）；
/*    7、SecuTrade.h和SecuTrade.cpp用来测试普通交易；
/*    8、SecuMD.h和SecuMD.cpp 用来测试订阅成交回报主推委托主推；
/*    9、UFX技术支持网站 https://ufx.hscloud.cn/；
/*    10、demo仅供参考。
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

//断开连接后会调用Onclose函数，提示连接断开
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

//异步接受调用函数，用于异步发送包请求后，线程接受返回包
void CTradeCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
	puts("CTradeCallback::OnReceivedBizMsg");
	if (lpMsg!=NULL)
	{
		//成功,应用程序不能释放lpBizMessageRecv消息
		if (lpMsg->GetReturnCode() ==0)
		{
			//如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
			//int iMsgLen = 0;
			//void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
			//将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
			//lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
			//没有错误信息
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
    //设置系统号
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
	if (0 != (iRet = lpConnection->Connect(3000)))
	{
		cerr<<"连接.iRet="<<iRet<<" msg:"<<lpConnection->GetErrorMsg(iRet)<<endl; 
		return -1;
	}

	


	return 0;
}

unsigned long SecuRequestMode::Release()
{
	delete this;
	return 0;
};


//331100证券客户登陆
int SecuRequestMode::ReqFunction331100()
{   
	int iRet=0,hSend=0, iLen=0;
   // cout<<"password"<<m_Password<<endl;
	//EncodeEx(m_Password,pout);
	//cout<<"pout: "<<pout<<endl;
	cout<<"331100客户登陆入参如下："<<endl;
	
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
	pPacker->AddField("content_type",'S', 6);  
	pPacker->AddField("password",'S', 10);
	pPacker->AddField("password_type", 'C'); 
	//pPacker->AddField("asset_prop",'C');
	      
	  
	///加入对应的字段值
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
	
	
	
	///结束打包
	pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//打印入参信息
	/*IF2UnPacker * lpUnPacker = NewUnPacker(pPacker->GetPackBuf(),pPacker->GetPackLen());
	lpUnPacker->AddRef();
	ShowPacket(lpUnPacker);
	lpUnPacker->Release();

	//异步模式登陆
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	*/
	//同步模式登陆 
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



//400证券行情查询 
int SecuRequestMode::ReqFunction400(char* exchange_type, char* stock_code)
{   
	cout<<"400证券行情查询入参:"<<endl;
	IBizMessage* lpBizMessage=NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetFunction(400);
	
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
	IF2Packer *pPacker=NewPacker(2);
	if (!pPacker)
	{
	  printf("取打包器失败！\r\n");
	  return -1;
	}
	pPacker->AddRef();
	pPacker->BeginPack();

	//加入字段名
	pPacker->AddField("exchange_type",'S');
	pPacker->AddField("stock_code",'S');

    //加入字段值
	pPacker->AddStr(exchange_type);
	pPacker->AddStr(stock_code);


	pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
	//打印入参信息 
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


//证券普通委托
int SecuRequestMode::ReqFunction333002(char* exchange_type,char* stock_code, double entrust_amount,double entrust_price,char entrust_bs)
{   
	cout<<"333002委托入参如下"<<endl;
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
	//lpBizMessage->SetSystemNo(iSystemNo);
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
	//pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_account", 'S', 15);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_amount", 'F', 19, 2);
	pPacker->AddField("entrust_price", 'F', 18, 3);
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
	//打印入参信息 
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

//委托撤单

int SecuRequestMode::ReqFunction333017()
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
	lpBizMessage->SetFunction(333017);
	//请求类型
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
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
	pPacker->AddField("batch_flag", 'S', 15);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("entrust_no", 'S', 6);
	
	///加入对应的字段值
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

//证券委托查询
int SecuRequestMode::ReqFunction333101()
  {
	  cout<<"333101委托查询入参:"<<endl;
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
	lpBizMessage->SetFunction(333101);
	//请求类型
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
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

	
	///加入对应的字段值
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

	//打印入参信息 
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

//证券成交查询
int SecuRequestMode::ReqFunction333102()
{
	 cout<<"333102证券成交查询入参"<<endl;
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
	lpBizMessage->SetFunction(333102);
	//请求类型
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSystemNo(iSystemNo);
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

	
	///加入对应的字段值
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
	//打印入参信息 
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


//证券持仓查询
int SecuRequestMode::ReqFunction333104(const char* position_str)
{
    	
	cout<<"333104证券持仓查询入参:"<<endl;
	
	 IBizMessage* lpBizMessage = NewBizMessage();
		lpBizMessage->AddRef();
		
		
		IBizMessage* lpBizMessageRecv = NULL;
		
		//功能号
		lpBizMessage->SetFunction(333104);
		//请求类型
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		//设置系统号
		lpBizMessage->SetSystemNo(iSystemNo);
	
		
		
		///获取版本为2类型的pack打包器
		IF2Packer *pPacker = NewPacker(2);
		if(!pPacker)
		{
			printf("取打包器失败!\r\n");
			return -1;
		}
		pPacker->AddRef();
		
		///开始打包
		pPacker->BeginPack();
		
		///加入字段名
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
		
		///加入对应的字段值
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

	
		///结束打包
		pPacker->EndPack();

	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();
}

//证券股东信息查询 
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
		
		//功能号
		lpBizMessage->SetFunction(331300);
		//请求类型
		lpBizMessage->SetPacketType(REQUEST_PACKET);
		//设置营业部号
		//设置系统号
		lpBizMessage->SetSystemNo(iSystemNo);
	
		
		///获取版本为2类型的pack打包器
		IF2Packer *pPacker = NewPacker(2);
		if(!pPacker)
		{
			printf("取打包器失败!\r\n");
			return -1;
		}
		pPacker->AddRef();
		
		///开始打包
		pPacker->BeginPack();
		
		///加入字段名
		pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
		pPacker->AddField("op_entrust_way", 'C', 1);
		pPacker->AddField("op_station", 'S', 255);
		pPacker->AddField("branch_no", 'I', 5); 
		pPacker->AddField("client_id", 'S', 18);
		pPacker->AddField("fund_account", 'S', 18);
		pPacker->AddField("password", 'S', 10);
		pPacker->AddField("password_type", 'C', 1); 
		pPacker->AddField("user_token", 'S', 512);
		pPacker->AddField("exchange_type", 'S', 4);

	
		
		///加入对应的字段值
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


	
		///结束打包
		pPacker->EndPack();
	lpBizMessage->SetContent(pPacker->GetPackBuf(),pPacker->GetPackLen());
    lpConnection->SendBizMsg(lpBizMessage, 1);
    pPacker->FreeMem(pPacker->GetPackBuf());
	pPacker->Release();
	lpBizMessage->Release();

}