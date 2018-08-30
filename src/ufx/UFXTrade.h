#include <string>
#include <memory>
#include <map>
#include <functional>
#include <mutex>

#include "t2sdk_interface.h"
#include "common.h"

class UFXTrade:public CCallbackInterface
{
public:
    UFXTrade();
    ~UFXTrade();

    // ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release(); // �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
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
    void SetConfigInterface(CConfigInterface* lpConfig);
    //331100 ����
    void OnResponse_331100(IF2UnPacker *lpUnPacker){};
    void OnResponse_333104(IF2UnPacker *lpUnPacker){};

    void ShowPacket(IF2UnPacker *lpUnPacker);
    int32 init(CConfigInterface* lpConfig,o32Config o32_config);
    int32 start();
    int32 reConnect(CConfigInterface* lpConfig);
    int32 stop();
    ErrorInfo CallService(CConnectionInterface* connection, int functionNo, IF2Packer* requestPacker, IF2UnPacker** responseUnPacker);
    ErrorInfo GetErrorInfo(IF2UnPacker* responseUnPacker);
    // ��ѯ�˻��ʽ����
    ErrorInfo QueryFundaset();

    // ��ѯ�ڻ���֤��
    ErrorInfo QueryFutuBail();
    // ��ѯ��Ȩ��֤��
    ErrorInfo QueryOptionBail();
    void makeFundaAsset(IF2UnPacker *lpUnPacker, std::string prefix);


    bool ParseFundaAssetInfo_UFX(o32_fundasset *fundassetInfo, char *inDataStr);
    std::map<std::string, string> getFundAssetMap();
    void ShowFundAssetMap();
private:
    IF2Packer* MakeLoginPacker(std::string operatorNo, std::string password);
private:

    CConnectionInterface *lpConnection_;
    
    char pout_[255];
    char account_name_[12];
    char password_[8];
    char entrustway_;
    std::string futures_account_;
    std::string op_station_;
    std::string user_token_;

    char op_user_token_[512];
    int branch_no_;
    char client_id_[18];
    int system_id_;
    int op_branch_no_;

    o32Config o32_config_;

    std::shared_ptr<o32_fundasset> o32_fundasset_;
    std::map<std::string, string> o32_fund_asset_map_;
    std::function<void()> on_disconnect_;

    std::mutex mut_;
    
};
using UFXTradePtr = std::shared_ptr<UFXTrade>;