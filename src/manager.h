#pragma once
#include <thread>
#include <csignal>
#include <chrono>

#include "cTraderSpi.h"
#include "UFXTrade.h"

class manager{
public:
    manager();
    ~manager();
    int32 loadConf();
    int32 init();
    int32 start();

    int32 monitor_process();
    void setStatus();
    void setFutureAccount(sTradingAccountInfo&);
    void showFutureAccount(sTradingAccountInfo *m_accountInfo);
private:


    double warning_level_ = 0.15;
    double stop_level_ = 0.2;

    volatile std::sig_atomic_t is_running = true;
    int timespan = 5;
    
    ctpConfig ctp_config_;
    o32Config o32_config_;
    cTraderSpiPtr ctp_td_spi_;
    UFXTradePtr ufx_td_ptr_;
    //CTradeCallbackPtr o32_trade_spi_;
    std::thread inner_thread_;
    // raw pointer follow t2sdk
    CConfigInterface* lpConfig_;

    //ctp account message
    sTradingAccountInfo future_account;
    std::map<std::string, std::string> o32_account;



};

