#include "tool.h"

#define  MSGCENTER_FUNC_HEART		 620000                                           //��Ϣ�����������ܺ�
#define  MSGCENTER_FUNC_REG			 620001                                           //��Ϣ���Ķ��Ĺ��ܺ�
#define  MSGCENTER_FUNC_REG_CANCLE   620002                                           //��Ϣ����ȡ�����Ĺ��ܺ�
#define  MSGCENTER_FUNC_SENDED		 620003                                           //��Ϣ�������ƹ��ܺ�
# define ISSUE_TYPE_REALTIME_SECU    12                                              // ����֤ȯ�ɽ��ر�
# define ISSUE_TYPE_ENTR_BACK        23                                              //����ί�лر�

class CMdRequestMode;
// �Զ�����CCallback��ͨ���̳У�ʵ�֣�CCallbackInterface�����Զ�������¼����������ӳɹ���
// ���ӶϿ���������ɡ��յ����ݵȣ�����ʱ�Ļص�����
class CMdCallback : public CCallbackInterface
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
public:
	void SetRequestMode(CMdRequestMode* lpMode);
	//331100 ����
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
	//ά������
	void OnHeartbeat(IBizMessage* lpMsg);
	//331100 ����
	int ReqFunction331100();
	//620001_12����֤ȯ�ɽ��ر�����
	int SubFunction12(int issue_type);
	//62001_23 ����֤ȯί�лر�
	int SunFunction23(int issue_type);
	//333002 ��ͨί�� 
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