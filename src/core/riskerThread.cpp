#include "riskerThread.h"
#include "qdebug.h"
#include "time.h"
#include <ctime>

riskerThread::riskerThread(QObject *parent) :QThread(parent)
{
    qDebug() << "Risker thread :" << QThread::currentThreadId();
    time_span_ = 3;
}

riskerThread::~riskerThread(){
    is_running = false;
    requestInterruption();
    quit();
    wait();
}

void riskerThread::run(){
    if (init()!=0){
        qDebug() << "Risker init failed :" << QThread::currentThreadId();
        emit sendStatusMsg(QString("Init failed,please check internet."));
        return ;
    }
    else{
        emit sendStatusMsg(QString("Init Success"));
    }
    if (riskManager.start() != 0){
        emit sendStatusMsg(QString("Start failed,please reStart."));
        return;
    }else{
        emit sendStatusMsg(QString("Start Success"));
    }
    while (is_running){
        if (!global::need_reconnect.load()){
            riskManager.monitor_process();

            memcpy(&future_account_, riskManager.getFutureInfo(), sizeof(sTradingAccountInfo));
            o32_account_ = riskManager.getO32Info();
            auto now_time = std::chrono::system_clock::now();
            auto date_time = std::chrono::system_clock::to_time_t(now_time);
            char date_time_buff[50];
            std::strftime(date_time_buff, 50, "%Y-%m-%d %H:%M:%S", localtime(&date_time));
            future_account_.updateTime = date_time_buff;
            emit resultReady(future_account_, o32_account_);

            msleep(time_span_ * 1000);
        }else{
            emit sendStatusMsg(QString("Internet connect error.,reconnecting.."));
            if (riskManager.reConnect() != 0){
                // reConnect failed.
                msleep(5000);
            }
            else{
                emit sendStatusMsg(QString("Reconnect success!."));
            }
        }


    };
    emit sendStatusMsg(QString("Monitor stop success!"));
}


void riskerThread::setRiskLevel(int timeSpan,double warningLevel, double stopLevel,bool status){
    time_span_ = timeSpan;
    is_running = status;
}

int32 riskerThread::init(){
    try{

        int32 result = 0;

        result = riskManager.loadConf();
        if (result != 0){
            RISK_LOG("Load conf failed! Result:" << result);
            return -1;
        }
        result = riskManager.init();
        if (result != 0)    {
            RISK_LOG("Init failed! Result:" << result);
            return -2;
        }
        return 0;
    }
    catch (std::exception e){
        RISK_LOG("Init failed! Result:" << e.what());
        return -3;
    }
}