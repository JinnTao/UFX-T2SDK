#include <string>
#include <memory>

#include "t2sdk_interface.h"
#include "common.h"

class UFXTrade:public CCallbackInterface
{
public:
    UFXTrade();
    ~UFXTrade();

    // 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release(); // 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
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
    void SetConfigInterface(CConfigInterface* lpConfig);
    //331100 登入
    void OnResponse_331100(IF2UnPacker *lpUnPacker){};
    void OnResponse_333104(IF2UnPacker *lpUnPacker){};

    void ShowPacket(IF2UnPacker *lpUnPacker);
    int32 init(CConfigInterface* lpConfig,o32Config o32_config);
    int32 start();
    int32 reConnect(CConfigInterface* lpConfig);
    int32 stop();
    ErrorInfo CallService(CConnectionInterface* connection, int functionNo, IF2Packer* requestPacker, IF2UnPacker** responseUnPacker);
    ErrorInfo GetErrorInfo(IF2UnPacker* responseUnPacker);
    // 查询账户资金情况
    ErrorInfo QueryFundaset(std::shared_ptr<o32_fundasset>);
    void makeFundaAsset(IF2UnPacker *lpUnPacker,std::shared_ptr<o32_fundasset> pO32_fundasset);
private:
    IF2Packer* MakeLoginPacker(std::string operatorNo, std::string password);
private:
    CConfigInterface* lpConfig_;
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
};
using UFXTradePtr = std::shared_ptr<UFXTrade>;