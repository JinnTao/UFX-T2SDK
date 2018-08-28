#include "RiskManagerApp.h"

RiskManagerApp::RiskManagerApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}
void RiskManagerApp::on_startMonitorPushButton_clicked(){
    ui.gdsybl->setText("on_startMonitorPushButton_clicked");
}
void RiskManagerApp::on_reStartPushButton_clicked(){
        try{

            int32 result = 0;

            result = riskManager.loadConf();
            if (result != 0){
                RISK_LOG("Load conf failed! Result:" << result);
                //return -1;
            }
            result = riskManager.init();
            if (result != 0){
                RISK_LOG("Init failed! Result:" << result);
                //return -1;
            }
            //result = riskManager.start();
            if (result != 0){
                RISK_LOG("Start failed! Result:" << result);
                //return -1;
            }

        }
        catch (std::exception e){
            
        }
}