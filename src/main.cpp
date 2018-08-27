// create by JinnTao 2018/08/24 .
// Tel:314315805@qq.com

#include "manager.h"
#include <csignal>
volatile std::sig_atomic_t is_running;

extern "C" void signal_handler(int signal) {
    RISK_LOG("Dectect signal:" << signal);
    is_running = false;
}

int main(){
    try{
        manager riskManager;
        int32 result = 0;
        is_running = true;

        std::signal(SIGTERM, signal_handler);  // program termination
        std::signal(SIGINT, signal_handler);   // interrupt by user

        result = riskManager.loadConf();
        if (result != 0){
            RISK_LOG("Load conf failed! Result:" << result);
            return -1;
        }
        result = riskManager.init();
        if (result != 0){
            RISK_LOG("Init failed! Result:" << result);
            return -1;
        }
        result = riskManager.start();
        if (result != 0){
            RISK_LOG("Start failed! Result:" << result);
            return -1;
        }
        while (is_running) {
            // do anything?


            std::this_thread::sleep_for(std::chrono::seconds(10));
        }

    }
    catch (std::exception e){
        
    }

}