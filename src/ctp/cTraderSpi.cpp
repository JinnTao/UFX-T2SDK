#include <cTraderSpi.h>
#include <memory>
#include <future>
#include <chrono>
#include <thread>
#include <regex>
#include <functional>
#include <iostream>

cTraderSpi::cTraderSpi() {
}
cTraderSpi::~cTraderSpi() {
    if (m_accountInfo){
        delete m_accountInfo;
    }
}

// After making a succeed connection with the CTP server, the client should send the login request to the CTP server.
void cTraderSpi::OnFrontConnected() {
    
    RISK_LOG("cTraderSpi::OnFrontConnected.");
    cout << "cTraderSpi::OnFrontConnected." << endl;
    if (on_connected_fun_) {
        on_connected_fun_();
    }
}

// When the connection between client and the CTP server disconnected, the following function will be called
void cTraderSpi::OnFrontDisconnected(int nReason) {
    // in this case, API will reconnect, the client application can ignore this
    RISK_LOG("cTraderSpi::OnFrontDisconnected,Reason: "<< nReason);
    if (on_disconnected_fun_) {
        on_disconnected_fun_(nReason);
    }
}

// After receiving the login request from the client, the CTP server will send the following response to notify the
// client whether the login success or not
void cTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
                                CThostFtdcRspInfoField*      pRspInfo,
                                int                          nRequestID,
                                bool                         bIsLast) {
    // std::lock_guard<std::mutex> guard(mut_);
    if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin) {
        front_id_        = pRspUserLogin->FrontID;
        session_id_      = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        trade_day_        = string(pRspUserLogin->TradingDay);

        iNextOrderRef++;
        sprintf(order_ref_, "%d", iNextOrderRef);
        RISK_LOG("cTraderSpi::OnRspUserLogin,TradeDate: " << pRspUserLogin->TradingDay << "SessionID: " << pRspUserLogin->SessionID << "FrontID: " << pRspUserLogin->FrontID);
    }
    if (bIsLast) {
        if (on_login_fun_) {
            on_login_fun_( pRspUserLogin, pRspInfo);
        }
    }
}
void cTraderSpi::ReqQrySettlementInfoConfirm(){
    std::lock_guard<std::mutex>          guard(mut_);
    CThostFtdcQrySettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy_s(req.BrokerID, sizeof(TThostFtdcBrokerIDType), ctp_config_.brokerId.c_str());
    strcpy_s(req.InvestorID, sizeof(TThostFtdcInvestorIDType), ctp_config_.userId.c_str());
    int iResult = ctpTdApi_->ReqQrySettlementInfoConfirm(&req, ++request_id_);
    RISK_LOG("ReqSettlementInforConrim,Result:" << iResult << ",requestId:" << request_id_ << "." );
}

void cTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
                                           CThostFtdcRspInfoField*               pRspInfo,
                                           int                                   nRequestID,
                                           bool                                  bIsLast){
    if (!IsErrorRspInfo(pRspInfo) && pSettlementInfoConfirm) {
        RISK_LOG("OnRspSettlementInfoConfirm: Success!ConfirmDate:" << 
            pSettlementInfoConfirm->ConfirmDate << ",ConfirmTime:" 
            << pSettlementInfoConfirm->ConfirmTime << ",nRequestID:" 
            << nRequestID << "." );
    }
    if (bIsLast) {
        if (pSettlementInfoConfirm) {
            ReqQryTradingAccount();
        }else{
            ReqSettlementInfoConfirm();
        }
    }

}


void cTraderSpi::ReqSettlementInfoConfirm() {
    std::lock_guard<std::mutex>          guard(mut_);
    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy_s(req.BrokerID, sizeof(TThostFtdcBrokerIDType), ctp_config_.brokerId.c_str());
    strcpy_s(req.InvestorID, sizeof(TThostFtdcInvestorIDType), ctp_config_.userId.c_str());
    int iResult = ctpTdApi_->ReqSettlementInfoConfirm(&req, ++request_id_); 
    RISK_LOG("ReqSettlementInforConrim,Result:" << iResult << ",requestId:" << request_id_ << ".");
}

void cTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
                                            CThostFtdcRspInfoField*               pRspInfo,
                                            int                                   nRequestID,
                                            bool                                  bIsLast) {

    if (!IsErrorRspInfo(pRspInfo) && pSettlementInfoConfirm) {
        
        RISK_LOG("OnRspSettlementInfoConfirm: Success!ConfirmDate:" <<
            pSettlementInfoConfirm->ConfirmDate << ",ConfirmTime:"
            << pSettlementInfoConfirm->ConfirmTime << ",nRequestID:"
            << nRequestID << ".");
    }
    if (bIsLast) {
        ReqQryTradingAccount();
    }
}

void cTraderSpi::ReqQryTradingAccount() {
    std::lock_guard<std::mutex>      guard(mut_);
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy_s(req.BrokerID, sizeof TThostFtdcBrokerIDType, ctp_config_.brokerId.c_str());
    strcpy_s(req.InvestorID, sizeof TThostFtdcInvestorIDType, ctp_config_.userId.c_str());
    while (true) {
        int iResult = ctpTdApi_->ReqQryTradingAccount(&req, ++request_id_);
        RISK_LOG("ReqQryTradingAccount,Result:" << iResult << ",requestId:" << request_id_ << ".");
        if (IsFlowControl(iResult)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        } else {
            break;
        }
    } 

}

void cTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount,
                                        CThostFtdcRspInfoField*        pRspInfo,
                                        int                            nRequestID,
                                        bool                           bIsLast) {
    m_accountInfo = new sTradingAccountInfo;
    memset(m_accountInfo, 0, sizeof(sTradingAccountInfo));
    if (!IsErrorRspInfo(pRspInfo) && pTradingAccount) {
        strcpy_s(m_accountInfo->BrokerID, sizeof(TThostFtdcBrokerIDType), pTradingAccount->BrokerID);
        strcpy_s(m_accountInfo->AccountID, sizeof(TThostFtdcAccountIDType), pTradingAccount->AccountID);
        m_accountInfo->PreBalance     = pTradingAccount->PreBalance;
        m_accountInfo->Balance        = pTradingAccount->Balance;
        m_accountInfo->Available      = pTradingAccount->Available;
        m_accountInfo->WithdrawQuota  = pTradingAccount->WithdrawQuota;
        m_accountInfo->CurrMargin     = pTradingAccount->CurrMargin;
        m_accountInfo->CloseProfit    = pTradingAccount->CloseProfit;
        m_accountInfo->PositionProfit = pTradingAccount->PositionProfit;
        m_accountInfo->Commission     = pTradingAccount->Commission;
        m_accountInfo->FrozenMargin   = pTradingAccount->FrozenMargin;
    }
    if (bIsLast){

        printf("Account Summary:\n");
        printf("   AccountID:%s\n", m_accountInfo->AccountID);
        printf("   PreBalance:%.2f\n", m_accountInfo->PreBalance);
        printf("   Balance:%.2f\n", m_accountInfo->Balance);
        printf("   WithdrawQuota:%f\n", m_accountInfo->WithdrawQuota);
        printf("   totalPnl:%.2f\n", m_accountInfo->CloseProfit + m_accountInfo->PositionProfit);
        printf("   CloseProfit:%.2f\n", m_accountInfo->CloseProfit);
        printf("   PositionProfit:%.2f\n", m_accountInfo->PositionProfit);
        printf("   Commission:%.2f\n", m_accountInfo->Commission);
        printf("   Available:%.2f\n", m_accountInfo->Available);
        printf("   CurrMargin:%.2f\n", m_accountInfo->CurrMargin);
        if (on_started_fun_){
            on_started_fun_();
        }

    }
}

void cTraderSpi::OnHeartBeatWarning(int nTimeLapse) {

    //ILOG("OnHeartBeatWarning,TimeLaps:{}.", nTimeLapse);
}

bool cTraderSpi::IsFlowControl(int iResult) {
    return ((iResult == -2) || (iResult == -3));
}



int32 cTraderSpi::init(const ctpConfig& ctp_config) {
    // 1.create td api instance
    {
        auto tdapi = CThostFtdcTraderApi::CreateFtdcTraderApi(ctp_config.td_flow_path.c_str());
        if (tdapi == nullptr) {
            RISK_LOG("CreateFtdcTraderApi instance failed");
            return -1;
        }
        // unique_ptr->ctp's document release just call Release api, release 2018/07/11 JinnTao
        ctpTdApi_ = { tdapi, [](CThostFtdcTraderApi* tdapi) {
            if (tdapi != nullptr) {
                tdapi->Release();
            }
            RISK_LOG("Release tradeApi.");
        } };
        ctpTdApi_->RegisterSpi(this);
        RISK_LOG("Td create instance success!");
    }

    // 2.connect to td Front
    {
        this->clearCallBack();

        ctpTdApi_->RegisterFront(const_cast<char*>(ctp_config.tdAddress.c_str()));
        std::promise<bool> connect_result;
        std::future<bool>  is_connected = connect_result.get_future();
        on_connected_fun_ = [&connect_result] { connect_result.set_value(true); };
        ctpTdApi_->Init();
        auto wait_result = is_connected.wait_for(std::chrono::seconds(5));
        if (wait_result != std::future_status::ready || is_connected.get() != true) {
            return -2;
        }
        RISK_LOG("Td connect front success!");
        ctpTdApi_->SubscribePrivateTopic(THOST_TERT_QUICK);  // Private QUICK recieve exchange send all msg after login
        ctpTdApi_->SubscribePublicTopic(THOST_TERT_QUICK);   // Public QUICK recieve exchange send all msg after login
    }

    // 3.login to Td.
    {
        this->clearCallBack();
        std::promise<bool> login_result;
        std::future<bool>  is_logined = login_result.get_future();
        on_login_fun_ = [&login_result](CThostFtdcRspUserLoginField* login, CThostFtdcRspInfoField* info) {
            if (info->ErrorID == 0) {
                login_result.set_value(true);
            }
            else {
                login_result.set_value(false);
            }
        };
        CThostFtdcReqUserLoginField req;

        memset(&req, 0, sizeof(req));
        strcpy_s(req.BrokerID, sizeof(TThostFtdcBrokerIDType), ctp_config.brokerId.c_str());
        strcpy_s(req.UserID, sizeof(TThostFtdcInvestorIDType), ctp_config.userId.c_str());
        strcpy_s(req.Password, sizeof TThostFtdcPasswordType, ctp_config.passwd.c_str());
        ctp_config_ = ctp_config;  // just no use deep copy
        // Try login
        auto req_login_result = ctpTdApi_->ReqUserLogin(&req, ++request_id_);
        if (req_login_result != 0) {
            RISK_LOG("Td request login failed!");
            return -3;
        }
        auto wait_result = is_logined.wait_for(std::chrono::seconds(5));
        if (wait_result != std::future_status::ready || is_logined.get() != true) {
            RISK_LOG("Td request login TimeOut!");
            return -3;
        }
        RISK_LOG("Td login success");
    }

    // 4.set callback
    {
        this->clearCallBack();
        on_disconnected_fun_ = [](int32 reason) {
            RISK_LOG("Td disconnect! reason:"<< reason);
        };
    }
    return 0;
}
int32 cTraderSpi::stop() {
    if (ctpTdApi_) {
        ctpTdApi_.reset(nullptr);
    }
    return 0;
}
int32 cTraderSpi::reConnect(const ctpConfig& ctp_config) {
    return 0;
}
int32 cTraderSpi::start() {
    std::promise<bool> start_result;
    std::future<bool>  is_started = start_result.get_future();
    on_started_fun_ = [&start_result]() { start_result.set_value(true); };
    this->ReqQrySettlementInfoConfirm();
    auto wait_result = is_started.wait_for(std::chrono::minutes(5));
    if (wait_result == std::future_status::timeout) {
        RISK_LOG("Td start timeout.");
        return -1;
    }
    else if (wait_result != std::future_status::ready || is_started.get() != true) {
        RISK_LOG("Td start failed,but not timeout with 5min");
        return -2;
    }

    return 0;
}

void cTraderSpi::clearCallBack() {
    std::lock_guard<std::mutex> guard(mut_);
    on_connected_fun_ = {};
    on_disconnected_fun_ = {};
    on_login_fun_ = {};
    on_started_fun_ = {};
}


bool cTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo) {
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult) {
        RISK_LOG("ErrorID: " << pRspInfo->ErrorID << ",ErrorMsg:" << pRspInfo->ErrorMsg << ".");
    }
    return bResult;
}
