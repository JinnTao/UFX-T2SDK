// create by JinnTao 2018/08/24 .
// Tel:314315805@qq.com
//


#include "RiskManagerApp.h"
#include <map>
#include <QtWidgets/QApplication>
#include <qmetatype.h>

#include "easylogging++.h"  

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    el::Configurations conf("easylogging.conf");
    el::Loggers::reconfigureAllLoggers(conf);

    QApplication a(argc, argv);

    // register meta type
    qRegisterMetaType<o32_account_info_map>("o32_account_info_map");
    qRegisterMetaType<sTradingAccountInfo>("sTradingAccountInfo");

    RiskManagerApp w;
    w.show();

    return a.exec();
}





















//#include "manager.h"
//#include <csignal>
//volatile std::sig_atomic_t is_running;
//
//extern "C" void signal_handler(int signal) {
//    RISK_LOG("Dectect signal:" << signal);
//    is_running = false;
//}
//
//int main(){
//    try{
//        manager riskManager;
//        int32 result = 0;
//        is_running = true;
//
//        std::signal(SIGTERM, signal_handler);  // program termination
//        std::signal(SIGINT, signal_handler);   // interrupt by user
//
//        result = riskManager.loadConf();
//        if (result != 0){
//            RISK_LOG("Load conf failed! Result:" << result);
//            return -1;
//        }
//        result = riskManager.init();
//        if (result != 0){
//            RISK_LOG("Init failed! Result:" << result);
//            return -1;
//        }
//        result = riskManager.start();
//        if (result != 0){
//            RISK_LOG("Start failed! Result:" << result);
//            return -1;
//        }
//        while (is_running) {
//            // do anything?
//
//
//            std::this_thread::sleep_for(std::chrono::seconds(10));
//        }
//
//    }
//    catch (std::exception e){
//        
//    }
//
//}