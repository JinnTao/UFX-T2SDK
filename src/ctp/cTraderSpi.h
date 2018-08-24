#ifndef __CTRADERSPI_H__
#define __CTRADERSPI_H__
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "common.h"
#include <map>
#include <memory>
#include <functional>
#include <mutex>
using namespace std;

class cTraderSpi : public CThostFtdcTraderSpi
{
public:
    cTraderSpi();
    ~cTraderSpi();
    // After making a succeed connection with the CTP server, the client should send the login request to the CTP server.
    virtual void OnFrontConnected();
    // When the connection between client and the CTP server disconnected, the following function will be called
    virtual void OnFrontDisconnected(int nReason);
    // After receiving the login request from the client, the CTP server will send the following response to notify the client whether the login success or not
    virtual void OnRspUserLogin( CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast );
    // investor settlement information confirmation response
    virtual void OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast );
    virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
        CThostFtdcRspInfoField*               pRspInfo,
        int                                   nRequestID,
        bool                                  bIsLast);
    // query trading account response
    virtual void OnRspQryTradingAccount( CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast );
    virtual void OnHeartBeatWarning( int nTimeLapse );
    void         ReqQrySettlementInfoConfirm();
    void ReqSettlementInfoConfirm();
    bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);
    void ReqQryTradingAccount();
    bool IsFlowControl(int iResult);
    const sTradingAccountInfo* GetTradingAccountInfo() const { return m_accountInfo; }
    int32 init(const ctpConfig& ctp_config);
    int32 stop();
    int32 reConnect(const ctpConfig& ctp_config);
    int32 start();
    void clearCallBack();
private:
 

    TThostFtdcOrderRefType    order_ref_;;
    TThostFtdcFrontIDType    front_id_;
    TThostFtdcSessionIDType    session_id_;
    string trade_day_;
    
    sTradingAccountInfo* m_accountInfo = nullptr;

    using CtpTdApiPtr = std::unique_ptr<CThostFtdcTraderApi, std::function<void(CThostFtdcTraderApi*)>>;
    CtpTdApiPtr ctpTdApi_;
    int32       request_id_ = 0;
    std::function<void()> on_connected_fun_;
    std::function<void(CThostFtdcRspUserLoginField*,CThostFtdcRspInfoField*)> on_login_fun_;
    std::function<void(int32)>                                                on_disconnected_fun_;
    std::function<void()>                                                     on_started_fun_;
    std::mutex                                                                mut_;
    ctpConfig                                                                 ctp_config_;

};
using cTraderSpiPtr = std::shared_ptr<cTraderSpi>;
#endif

