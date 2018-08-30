#pragma once

#include <QtWidgets/QMainWindow>
#include <qthread.h>
#include "ui_RiskManagerApp.h"
#include "manager.h"
#include "riskerThread.h"
class RiskManagerApp : public QMainWindow
{
    Q_OBJECT

public:
    RiskManagerApp(QWidget *parent = Q_NULLPTR);
    void init();

public slots:
    void on_startMonitorPushButton_clicked();
    void on_reStartPushButton_clicked();
    void handleResults(sTradingAccountInfo futureInfo, o32_account_info_map o32_account_info);
    void handleStatusMsg(QString msg);
private:
    Ui::RiskManagerApp ui;
    // risk control parameters
    riskerThread inner_thread;


    double warning_level_;
    double stop_level_;

};
