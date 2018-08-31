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
    void handleResults(sTradingAccountInfo futureInfo, o32_account_info_map o32_account_info);
    void handleStatusMsg(QString msg);
private:
    Ui::RiskManagerApp ui;
    // risk control parameters
    riskerThread inner_thread;

    // fixed income
    double fixed_warning_level_;
    double fixed_stop_level_;
    // future & derivative
    double futureAndDeri_warning_level_;
    double futureAndDeri_stop_level_;
    // future assets
    double future_warning_level_;
    double future_stop_level_;
    // stock assets
    double stock_warning_level_;
    double stock_stop_level_;

};
