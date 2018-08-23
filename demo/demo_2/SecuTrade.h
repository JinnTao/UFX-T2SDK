#include "tool.h"

class SecuRequestMode;
// 自定义类CCallback，通过继承（实现）CCallbackInterface，来自定义各种事件（包括连接成功、
// 连接断开、发送完成、收到数据等）发生时的回调方法
class CTradeCallback : public CCallbackInterface
{
public:
    // 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    // 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
    // Reserved?为保留方法，为以后扩展做准备，实现时可直接return或return 0。
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);
    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);
    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);
    int  FUNCTION_CALL_MODE Reserved3();
    void FUNCTION_CALL_MODE Reserved4();
    void FUNCTION_CALL_MODE Reserved5();
    void FUNCTION_CALL_MODE Reserved6();
    void FUNCTION_CALL_MODE Reserved7();
    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);
	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
	void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);
	//int FUNCTION_CALL_MODE EncodeEx(const char *pIn, char *pOut);
public:
	void SetRequestMode(SecuRequestMode* lpMode);
	//331100 登入
	void OnResponse_331100(IF2UnPacker *lpUnPacker);
	void OnResponse_333104(IF2UnPacker *lpUnPacker);
	
private:
	SecuRequestMode* lpReqMode;
};

class SecuRequestMode
{
public:
	SecuRequestMode()
	{
		lpConfig = NULL;
		lpConnection = NULL;
		callback.SetRequestMode(this);

		lpConfig = NewConfig();
		lpConfig->AddRef();
		memset(m_opUserToken,0,sizeof(m_opUserToken));
	    m_BranchNo=0;
	    memset(m_client_id,0,sizeof(m_client_id));
	    iSystemNo=0;
		m_op_branch_no=0;
		memset(m_AccountName,0,sizeof(m_AccountName));
	    memset(m_Password,0,sizeof(m_Password));
		memset(pout,0,sizeof(pout));
	    m_EntrustWay='\0';
	    m_FuturesAccount="0";
	    m_opStation="0";
	};

	~SecuRequestMode()
	{
		lpConnection->Release();
		lpConfig->Release();
	};

	int InitConn();
	unsigned long Release();
public:
	char m_opUserToken[512];
	int m_BranchNo;
	char m_client_id[18];
	int iSystemNo;
	int m_op_branch_no;

	//331100 登入
	int ReqFunction331100();
	//400 证券行情查询
	int ReqFunction400(char* exchange_type, char* stock_code);
	//333002 普通委托 
	int ReqFunction333002(char* exchange_type,char* stock_code, double entrust_amount,double entrust_price,char entrust_bs);
	//333104 证券持仓查询
	int ReqFunction333104(const char* position_str);
	//331101 用户修改密码借口
	int ReqFunction331101();
	//331300证券股东信息查询
	int ReqFunction331300();
	//33017证券委托撤单
	int ReqFunction333017();
	//333101证券委托查询
	int ReqFunction333101();
	//333102证券成交查询
	int ReqFunction333102();	
	//333101证券委托查询
	
private:
	CConfigInterface* lpConfig;
	CConnectionInterface *lpConnection;
	CTradeCallback callback;
	char pout[255];
	char m_AccountName[12];
    char m_Password[8];
	char m_EntrustWay;
	string m_FuturesAccount;
	string m_opStation;
	
};

