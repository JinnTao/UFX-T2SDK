#pragma once
#include "cTraderSpi.h"
#include "UFXTrade.h"
#include <thread>
class manager{
public:
    manager();
    ~manager();
    int32 loadConf();
    int32 init();
    int32 start();
    int32 monitor_process();

private:
    ctpConfig ctp_config_;
    o32Config o32_config_;
    cTraderSpiPtr ctp_td_spi_;
    UFXTradePtr ufx_td_ptr_;
    //CTradeCallbackPtr o32_trade_spi_;
    std::thread inner_thread_;
    // raw pointer follow t2sdk
    CConfigInterface* lpConfig_;
};

