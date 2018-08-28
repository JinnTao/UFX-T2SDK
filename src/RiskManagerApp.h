#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RiskManagerApp.h"
#include "manager.h"
class RiskManagerApp : public QMainWindow
{
    Q_OBJECT

public:
    RiskManagerApp(QWidget *parent = Q_NULLPTR);
    void init();

public slots:
    void on_startMonitorPushButton_clicked();
    void on_reStartPushButton_clicked();
private:
    Ui::RiskManagerApp ui;
    manager riskManager;
};
