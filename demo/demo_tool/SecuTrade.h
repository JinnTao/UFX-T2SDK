#include "tool.h"
#include <memory>
class SecuRequestMode;
// �Զ�����CCallback��ͨ���̳У�ʵ�֣�CCallbackInterface�����Զ�������¼����������ӳɹ���
// ���ӶϿ���������ɡ��յ����ݵȣ�����ʱ�Ļص�����
class CTradeCallback : public CCallbackInterface
{
public:
    // ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    // �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
    // Reserved?Ϊ����������Ϊ�Ժ���չ��׼����ʵ��ʱ��ֱ��return��return 0��
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
	//331100 ����
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

	//331100 ����
	int ReqFunction331100();
	//400 ֤ȯ�����ѯ
	int ReqFunction400(char* exchange_type, char* stock_code);
	//333002 ��ͨί�� 
	int ReqFunction333002(char* exchange_type,char* stock_code, double entrust_amount,double entrust_price,char entrust_bs);
	//333104 ֤ȯ�ֲֲ�ѯ
	int ReqFunction333104(const char* position_str);
	//331101 �û��޸�������
	int ReqFunction331101();
	//331300֤ȯ�ɶ���Ϣ��ѯ
	int ReqFunction331300();
	//33017֤ȯί�г���
	int ReqFunction333017();
	//333101֤ȯί�в�ѯ
	int ReqFunction333101();
	//333102֤ȯ�ɽ���ѯ
	int ReqFunction333102();	
	//333101֤ȯί�в�ѯ
	
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

using CTradeCallbackPtr = std::shared_ptr<CTradeCallback>;

