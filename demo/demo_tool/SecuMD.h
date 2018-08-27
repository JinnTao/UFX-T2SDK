#include "tool.h"

#define  MSGCENTER_FUNC_HEART		 620000                                           //消息中心心跳功能号
#define  MSGCENTER_FUNC_REG			 620001                                           //消息中心订阅功能号
#define  MSGCENTER_FUNC_REG_CANCLE   620002                                           //消息中心取消订阅功能号
#define  MSGCENTER_FUNC_SENDED		 620003                                           //消息中心主推功能号
# define ISSUE_TYPE_REALTIME_SECU    12                                              // 订阅证券成交回报
# define ISSUE_TYPE_ENTR_BACK        23                                              //订阅委托回报

class CMdRequestMode;
// 自定义类CCallback，通过继承（实现）CCallbackInterface，来自定义各种事件（包括连接成功、
// 连接断开、发送完成、收到数据等）发生时的回调方法
class CMdCallback : public CCallbackInterface
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
public:
	void SetRequestMode(CMdRequestMode* lpMode);
	//331100 登入
	void OnResponse_331100(IF2UnPacker *lpUnPacker);
private:
	CMdRequestMode* lpReqMode;
};

class CMdRequestMode
{
public:
	CMdRequestMode()
	{
		lpConfig = NULL;
		lpConnection = NULL;
		callback.SetRequestMode(this);

		lpConfig = NewConfig();
		lpConfig->AddRef();
		memset(m_client_id,0,sizeof(m_client_id));
		m_opUserToken="0";
	    m_BranchNo=0;
	    memset(m_client_id,0,sizeof(m_client_id));
	    iSystemNo=0;
		m_op_branch_no=0;
		memset(m_AccountName,0,sizeof(m_AccountName));
	    memset(m_Password,0,sizeof(m_Password));
	    m_EntrustWay='\0';
	    m_FuturesAccount="0";
	    m_opStation="0";
	};

	~CMdRequestMode()
	{
		lpConnection->Release();
		lpConfig->Release();
	};

	int InitConn();
	unsigned long Release();
public:
	string m_opUserToken;
	int m_BranchNo;
	char m_client_id[18];
	int iSystemNo;
	int m_op_branch_no;
	//维护心跳
	void OnHeartbeat(IBizMessage* lpMsg);
	//331100 登入
	int ReqFunction331100();
	//620001_12订阅证券成交回报功能
	int SubFunction12(int issue_type);
	//62001_23 订阅证券委托回报
	int SunFunction23(int issue_type);
	//333002 普通委托 
	int ReqFunction333002();
private:
	CConfigInterface* lpConfig;
	CConnectionInterface *lpConnection;
	CMdCallback callback;

	char m_AccountName[12];
	char m_Password[8];
	char m_EntrustWay;
	string m_FuturesAccount;
	string m_opStation;
	
	int m_SubSystemNo;
};