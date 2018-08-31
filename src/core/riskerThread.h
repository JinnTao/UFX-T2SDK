#pragma once

#include <qthread.h>
#include <signal.h>
#include "manager.h"
class riskerThread : public QThread
{
    Q_OBJECT

public:
     explicit riskerThread(QObject *parent = Q_NULLPTR);
    ~riskerThread();
    virtual void run() Q_DECL_OVERRIDE;
    void setRiskLevel(int timeSpan,double warningLevel,double stopLevel,bool status);
    int32 init();
signals:
    void sendStatusMsg(QString msg);
    void resultReady(sTradingAccountInfo futureInfo, o32_account_info_map o32_account_info);

private:
    // risk control parameters

    manager riskManager;

    std::sig_atomic_t is_running = false;
    sTradingAccountInfo future_account_;
    o32_account_info_map o32_account_;
    int time_span_;
};
