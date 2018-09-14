#include "manager.h"
#include <future>
#include <functional>

#define riskManagerConf "riskManagerConf.ini"

manager::manager(){

    // config fix
    lpConfig_ = nullptr;
    lpConfig_ = NewConfig();
    lpConfig_->AddRef();// must do.
    memset(&future_account, 0, sizeof(sTradingAccountInfo));
    if (o32_account.size() != 0){
        o32_account.clear();
    }
    
}

manager::~manager(){
    lpConfig_->Release();

}
int32 manager::loadConf(){
    try{
        // o32 config
        lpConfig_->Load(riskManagerConf);
        o32_config_.server_address = lpConfig_->GetString("t2sdk", "servers", "");
        o32_config_.fund_account = lpConfig_->GetString("ufx", "fund_account", "");
        o32_config_.passwd = lpConfig_->GetString("ufx", "password", "");
        o32_config_.license_file = lpConfig_->GetString("t2sdk", "license_file", "license.dat");
        o32_config_.login_name = lpConfig_->GetString("t2sdk", "login_name", "riskManager");
        o32_config_.authorization_id = lpConfig_->GetString("ufx", "authorization_id", "hd123456");
        o32_config_.asset_no = lpConfig_->GetString("ufx", "asset_no", "");

        // ctp config
        ctp_config_.brokerId = lpConfig_->GetString("ctp_account", "brokerID", "");
        ctp_config_.tdAddress = lpConfig_->GetString("ctp_account", "tdAddress", "");
        ctp_config_.passwd = lpConfig_->GetString("ctp_account", "password", "");
        ctp_config_.userId = lpConfig_->GetString("ctp_account", "userID", "");
        ctp_config_.td_flow_path = lpConfig_->GetString("ctp_account", "tdFlowPath", "");
        ctp_config_.md_flow_path = lpConfig_->GetString("ctp_account", "mdFlowPath", "");
        ctp_config_.mdAddress = lpConfig_->GetString("ctp_account", "mdAddress ", "");
        return 0;

    }
    catch (std::exception &e){
        RISK_LOG("Load conf failed,msg: " << e.what());
        return -1;
    }
    catch (...){
        RISK_LOG("Load conf failed. ");
        return -2;
    }

}

int32 manager::init(){
    // init  trade
    int32 init_result;
    try{
        if (ctp_td_spi_){
            ctp_td_spi_.reset();
        }
        ctp_td_spi_ = std::make_shared<cTraderSpi>();
        init_result = ctp_td_spi_->init(ctp_config_);
        if (init_result != 0) {
            RISK_LOG("Td init failed! Result:" << init_result);
            return -1;
        }

        if (ufx_td_ptr_){
            ufx_td_ptr_.reset();
        }
        ufx_td_ptr_ = std::make_shared<UFXTrade>();
        init_result = ufx_td_ptr_->init(lpConfig_,o32_config_);
        if (init_result != 0){
            RISK_LOG("UFX init failed! Result:" << init_result);
            return -2;
        }
        // global connect set
        global::need_reconnect.store(false);
        return 0;

    }
    catch (std::exception& e){
        RISK_LOG("Manager init failed,error:" << e.what());
        return -3;
    }
    catch (...){
        RISK_LOG("Manager init failed.");
        return -4;
    }

}

int32 manager::start(){
    int32 init_result;
    try{
        // start up ctp td 
        init_result = ctp_td_spi_->start();
        if (init_result != 0) {
            RISK_LOG("Td start failed! Result:" << init_result);
            return -1;
        }
        // start up ctp td 
        init_result = ufx_td_ptr_->start();
        if (init_result != 0) {
            RISK_LOG("Ufx start failed! Result:" << init_result);
            return -2;
        }

        // start monitor
        // inner_thread_ = std::thread(&manager::monitor_process, this);


        return 0;

    }
    catch(std::exception &e){
        RISK_LOG("Manager start failed,error:" << e.what());
        return -2;
    }
    catch (...){
        RISK_LOG("Manager start failed.");
        return -3;
    }
}

int32 manager::monitor_process(){

    //obtain future account message
    std::promise<bool> obtain_future_account;
    std::future<bool> is_obtain_future_account = obtain_future_account.get_future();

    this->ctp_td_spi_->setOnFutureAccount([&obtain_future_account,this](sTradingAccountInfo& tradeInfo){
        this->setFutureAccount(tradeInfo);
        obtain_future_account.set_value(true);
    });
    this->ctp_td_spi_->ReqQryTradingAccount();
    if (is_obtain_future_account.valid()){
        auto obtain_result = is_obtain_future_account.wait_for(std::chrono::seconds(10));
        if (obtain_result != std::future_status::ready || is_obtain_future_account.get() != true) {
            std::cout << " future account no refresh,please check internet:" << std::endl;
        }
        else{
            std::cout << " future account refresh:" << std::endl;
        }
    }
    //this->showFutureAccount(&(this->future_account));


    // obtain o32 account messgae
    this->ufx_td_ptr_->QueryFundaset();
    this->ufx_td_ptr_->QueryFutuBail();
    this->ufx_td_ptr_->QueryOptionBail();

    this->o32_account = ufx_td_ptr_->getFundAssetMap();
    //this->ufx_td_ptr_->ShowFundAssetMap();


    return 0;

}

void manager::setStatus(){
    is_running = !is_running;
}

void manager::setFutureAccount(sTradingAccountInfo& trade_account){
    memcpy(&future_account, &trade_account, sizeof(sTradingAccountInfo));
}
void manager::showFutureAccount(sTradingAccountInfo *m_accountInfo){
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

}


sTradingAccountInfo* manager::getFutureInfo(){
    return &future_account;
}
o32_account_info_map manager::getO32Info(){
    return o32_account;

}
int32 manager::reConnect(){
    int32 reConnect_result = 0;
    try{
        reFresh();

        reConnect_result = loadConf();
        if (reConnect_result != 0){
            // Reconnect Failed;
            return -1;
        }

        reConnect_result = init();
        if (reConnect_result != 0){
            // Reconnect Failed;
            return -2;
        }
        reConnect_result = start(); 
        if (reConnect_result != 0){
            // Restart Failed;
            return -3;
        }
        global::need_reconnect.store(false);
        return 0;
    
    }
    catch (std::exception e){
        // other Error
        return -4;
    }
}
void manager::reFresh(){
    // config fix
    if (lpConfig_ != nullptr){
        lpConfig_->Release();
    }
    lpConfig_ = nullptr;
    lpConfig_ = NewConfig();
    lpConfig_->AddRef();// must do.
}