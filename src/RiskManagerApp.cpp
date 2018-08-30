#include "RiskManagerApp.h" 
#include <qmetatype.h>
RiskManagerApp::RiskManagerApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // connect signals & slot
    connect(&inner_thread, SIGNAL(resultReady(sTradingAccountInfo, o32_account_info_map)), this, SLOT(handleResults(sTradingAccountInfo, o32_account_info_map)));
    connect(&inner_thread, SIGNAL(sendStatusMsg(QString)), this, SLOT(handleStatusMsg(QString)));

}
void RiskManagerApp::on_startMonitorPushButton_clicked(){
    if (ui.timeSpanSpinBox->isEnabled()){
        ui.startMonitorPushButton->setText(QString::fromLocal8Bit("终止监控"));
        ui.timeSpanSpinBox->setEnabled(false);
        ui.warnLevelSpinBox->setEnabled(false);
        ui.stopLevelDoubleSpinBox->setEnabled(false);
        ui.reStartPushButton->setEnabled(false);
        inner_thread.setRiskLevel(ui.timeSpanSpinBox->value(), 0, 0,true);
        warning_level_ = ui.warnLevelSpinBox->value();
        stop_level_ = ui.stopLevelDoubleSpinBox->value();

        if (!inner_thread.isRunning()){
            inner_thread.start();
        }
    }
    else{
        ui.startMonitorPushButton->setText(QString::fromLocal8Bit("启动监控"));
        ui.timeSpanSpinBox->setEnabled(true);
        ui.warnLevelSpinBox->setEnabled(true);
        ui.stopLevelDoubleSpinBox->setEnabled(true);
        ui.reStartPushButton->setEnabled(true);
        inner_thread.setRiskLevel(ui.timeSpanSpinBox->value(), ui.warnLevelSpinBox->value(), ui.stopLevelDoubleSpinBox->value(), false);

        inner_thread.wait();
    }


}
void RiskManagerApp::on_reStartPushButton_clicked(){

}

void RiskManagerApp::handleStatusMsg(QString msg){
    ui.statusLabel->setText(msg);
}

void RiskManagerApp::handleResults(sTradingAccountInfo futureInfo, o32_account_info_map o32_account_info){
    double totalAsset = 0,fixedIncome = 0,equityAsset = 0,futureDeriAsset = 0;
    // future account
    ui.AccountID->setText(futureInfo.AccountID);
    ui.BrokerID->setText(futureInfo.BrokerID);
    ui.PreBalance->setText(QString::number(futureInfo.PreBalance));
    ui.Balance->setText(QString::number(futureInfo.Balance));
    ui.WithdrawQuota->setText(QString::number(futureInfo.WithdrawQuota));
    ui.CurrMargin->setText(QString::number(futureInfo.CurrMargin));
    ui.CloseProfit->setText(QString::number(futureInfo.CloseProfit));
    ui.PositionProfit->setText(QString::number(futureInfo.PositionProfit));
    ui.Commission->setText(QString::number(futureInfo.Commission));
    ui.FrozenMargin->setText(QString::number(futureInfo.FrozenMargin));
    //o32 account
    if (o32_account_info.size() > 0){
        ui.account_code->setText(QString::fromStdString(o32_account_info["account_code"]));
        ui.currency_code->setText(QString::fromStdString(o32_account_info["currency_code"]));
        ui.total_asset->setText(QString::fromStdString(o32_account_info["total_asset"]));
        ui.nav->setText(QString::fromStdString(o32_account_info["nav"]));
        ui.yesterday_nav->setText(QString::fromStdString(o32_account_info["yesterday_nav"]));
        ui.current_balance->setText(QString::fromStdString(o32_account_info["current_balance"]));
        ui.begin_balance->setText(QString::fromStdString(o32_account_info["begin_balance"]));
        ui.futu_deposit_balance->setText(QString::fromStdString(o32_account_info["futu_deposit_balance"]));
        ui.occupy_deposit_balance->setText(QString::fromStdString(o32_account_info["occupy_deposit_balance"]));
        ui.futu_asset->setText(QString::fromStdString(o32_account_info["futu_asset"]));
        ui.stock_asset->setText(QString::fromStdString(o32_account_info["stock_asset"]));
        ui.bond_asset->setText(QString::fromStdString(o32_account_info["bond_asset"]));
        ui.fund_asset->setText(QString::fromStdString(o32_account_info["fund_asset"]));
        ui.repo_asset->setText(QString::fromStdString(o32_account_info["repo_asset"]));
        ui.other_asset->setText(QString::fromStdString(o32_account_info["other_asset"]));
        ui.fund_share->setText(QString::fromStdString(o32_account_info["fund_share"]));
        ui.fund_net_asset->setText(QString::fromStdString(o32_account_info["fund_net_asset"]));

        ui.ErrorCode->setText(QString::fromStdString(o32_account_info["ErrorCode"]));
        ui.ErrorMsg->setText(QString::fromLocal8Bit((o32_account_info["ErrorMsg"].c_str())));

        // Risk InfoMation
        totalAsset = std::atof(o32_account_info["total_asset"].c_str()) + futureInfo.Balance;
        if (totalAsset != 0){
            fixedIncome = (std::atof(o32_account_info["bond_asset"].c_str()) + std::atof(o32_account_info["fund_asset"].c_str()) + std::atof(o32_account_info["repo_asset"].c_str())) / totalAsset;
            equityAsset = (std::atof(o32_account_info["stock_asset"].c_str())) / totalAsset;
            futureDeriAsset = (std::atof(o32_account_info["futu_asset"].c_str()) + futureInfo.CurrMargin) / totalAsset;


        }
        ui.totalAssetRatioLabel->setText(QString::number(totalAsset,'f',2));
        ui.BondsRatioLabel->setText(QString::number(fixedIncome,'f',3));
        ui.StockRatioLabel->setText(QString::number(equityAsset,'f',3));
        ui.futureAndDeriRatioLabel->setText(QString::number(futureDeriAsset,'f',3));

    }
    if (futureDeriAsset > stop_level_){
        ui.statusLabel->setText(QString::fromLocal8Bit("到达平仓线！商品衍生品比例：") + QString::number(futureDeriAsset, 'g', 3) + QString::fromLocal8Bit(" 平仓线：") + QString::number(stop_level_, 'g', 3));
    }
    else if (futureDeriAsset > warning_level_){
        ui.statusLabel->setText(QString::fromLocal8Bit("到达预警线！商品衍生品比例：") + QString::number(futureDeriAsset, 'g', 3) + QString::fromLocal8Bit(" 预警线：") + QString::number(warning_level_, 'g', 3));
    }
    else{
        ui.statusLabel->setText(QString::fromLocal8Bit("正常状态！"));
    }

}
