#include "manager.h"

#define riskManagerConf "riskManagerConf.ini"

manager::manager(){

    // config fix
    lpConfig = nullptr;
    lpConfig = NewConfig();
    lpConfig->AddRef();// must do
}

manager::~manager(){
    lpConfig->Release();

}
int32 manager::loadConf(){
    try{
        // o32 config
        lpConfig->Load(riskManagerConf);
        o32_config_.server_address = lpConfig->GetString("t2sdk", "servers", "");
        o32_config_.fund_account = lpConfig->GetString("ufx", "fund_account", "");
        o32_config_.passwd = lpConfig->GetString("ufx", "password", "");
        o32_config_.license_file = lpConfig->GetString("t2sdk", "license_file", "license.dat");
        o32_config_.login_name = lpConfig->GetString("t2sdk", "login_name", "riskManager");

        // ctp config
        ctp_config_.brokerId = lpConfig->GetString("ctp_account", "brokerID", "");
        ctp_config_.tdAddress = lpConfig->GetString("ctp_account", "tdAddress", "");
        ctp_config_.passwd = lpConfig->GetString("ctp_account", "password", "");
        ctp_config_.userId = lpConfig->GetString("ctp_account", "userID", "");
        ctp_config_.td_flow_path = lpConfig->GetString("ctp_account", "tdFlowPath", "");
        ctp_config_.md_flow_path = lpConfig->GetString("ctp_account", "mdFlowPath", "");
        ctp_config_.mdAddress = lpConfig->GetString("ctp_account", "mdAddress ", "");
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
    // init ctp trade
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