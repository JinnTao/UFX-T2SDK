#include "RiskManagerApp.h" 
#include <qmetatype.h>
#include "easylogging++.h"

RiskManagerApp::RiskManagerApp(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // connect signals & slot
    connect(&inner_thread, SIGNAL(resultReady(sTradingAccountInfo, o32_account_info_map)), this, SLOT(handleResults(sTradingAccountInfo, o32_account_info_map)));
    connect(&inner_thread, SIGNAL(sendStatusMsg(QString)), this, SLOT(handleStatusMsg(QString)));

    ui.loggingBrowser->document()->setMaximumBlockCount(1000);

}
void RiskManagerApp::on_startMonitorPushButton_clicked(){
    if (ui.timeSpanSpinBox->isEnabled()){
        ui.startMonitorPushButton->setText(QString::fromLocal8Bit("��ֹ���"));
        ui.timeSpanSpinBox->setEnabled(false);


        ui.fixedStopLevelDoubleSpinBox->setEnabled(false);
        ui.fixedWarnLevelSpinBox->setEnabled(false);

        ui.fAndDstopLevelDoubleSpinBox->setEnabled(false);
        ui.fAndDWarnLevelSpinBox->setEnabled(false);

        ui.stockStopLevelDoubleSpinBox->setEnabled(false);
        ui.stockWarnLevelSpinBox->setEnabled(false);

        ui.futurestopLevelDoubleSpinBox->setEnabled(false);
        ui.futureWarnLevelSpinBox->setEnabled(false);

        inner_thread.setRiskLevel(ui.timeSpanSpinBox->value(), 0, 0,true);

        fixed_warning_level_ = ui.fixedWarnLevelSpinBox->value();
        fixed_stop_level_    = ui.fixedStopLevelDoubleSpinBox->value();

        futureAndDeri_warning_level_ = ui.fAndDWarnLevelSpinBox->value();
        futureAndDeri_stop_level_ = ui.fAndDstopLevelDoubleSpinBox->value();

        stock_warning_level_ = ui.stockWarnLevelSpinBox->value();
        stock_stop_level_ = ui.stockStopLevelDoubleSpinBox->value();

        future_warning_level_ = ui.futureWarnLevelSpinBox->value();
        future_stop_level_ = ui.futurestopLevelDoubleSpinBox->value();

        ui.loggingBrowser->append(QString::fromLocal8Bit("�������"));
        if (!inner_thread.isRunning()){
            inner_thread.start();
        }
    }
    else{
        ui.startMonitorPushButton->setText(QString::fromLocal8Bit("�������"));
        ui.timeSpanSpinBox->setEnabled(true);

        ui.fixedStopLevelDoubleSpinBox->setEnabled(true);
        ui.fixedWarnLevelSpinBox->setEnabled(true);

        ui.fAndDstopLevelDoubleSpinBox->setEnabled(true);
        ui.fAndDWarnLevelSpinBox->setEnabled(true);

        ui.stockStopLevelDoubleSpinBox->setEnabled(true);
        ui.stockWarnLevelSpinBox->setEnabled(true);

        ui.futurestopLevelDoubleSpinBox->setEnabled(true);
        ui.futureWarnLevelSpinBox->setEnabled(true);

        inner_thread.setRiskLevel(ui.timeSpanSpinBox->value(), 0, 0, false);

        inner_thread.wait();
        ui.loggingBrowser->append(QString::fromLocal8Bit("��ֹ���"));
    }


}


void RiskManagerApp::handleStatusMsg(QString msg){
    ui.statusLabel->setText(msg);
    ui.loggingBrowser->append(msg);
}

void RiskManagerApp::handleResults(sTradingAccountInfo futureInfo, o32_account_info_map o32_account_info){

    double totalAsset = 0,fixedIncome = 0,equityAsset = 0,futureDeriAsset = 0,futureAsset = 0;
    bool allowFutureTradeWarning = true, allowFutureDerivativeWarning = true, allowFixedTradeWarning = true, allowStockTradeWarning = true;
    bool allowFutureTradeStop = true, allowFutureDerivativeStop = true, allowFixedTradeStop = true, allowStockTradeStop = true;

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
        ui.account_code->setText(QString::fromStdString(o32_account_info["total_account_code"]));
        ui.currency_code->setText(QString::fromStdString(o32_account_info["total_currency_code"]));
        ui.total_asset->setText(QString::fromStdString(o32_account_info["total_total_asset"]));
        ui.nav->setText(QString::fromStdString(o32_account_info["total_nav"]));
        ui.yesterday_nav->setText(QString::fromStdString(o32_account_info["total_yesterday_nav"]));
        ui.current_balance->setText(QString::fromStdString(o32_account_info["total_current_balance"]));
        ui.begin_balance->setText(QString::fromStdString(o32_account_info["total_begin_balance"]));
        ui.futu_deposit_balance->setText(QString::fromStdString(o32_account_info["total_futu_deposit_balance"]));
        ui.occupy_deposit_balance->setText(QString::fromStdString(o32_account_info["total_occupy_deposit_balance"]));
        ui.futu_asset->setText(QString::fromStdString(o32_account_info["total_futu_asset"]));
        ui.stock_asset->setText(QString::fromStdString(o32_account_info["total_stock_asset"]));
        ui.bond_asset->setText(QString::fromStdString(o32_account_info["total_bond_asset"]));
        ui.fund_asset->setText(QString::fromStdString(o32_account_info["total_fund_asset"]));
        ui.repo_asset->setText(QString::fromStdString(o32_account_info["total_repo_asset"]));
        ui.other_asset->setText(QString::fromStdString(o32_account_info["total_other_asset"]));
        ui.fund_share->setText(QString::fromStdString(o32_account_info["total_fund_share"]));
        ui.fund_net_asset->setText(QString::fromStdString(o32_account_info["total_fund_net_asset"]));

        ui.fund_share->setText(QString::fromStdString(o32_account_info["total_fund_share"]));
        ui.fund_net_asset->setText(QString::fromStdString(o32_account_info["total_fund_net_asset"]));

        ui.asset_no->setText(QString::fromStdString(o32_account_info["future_asset_no"]));

        ui.futureOccupyBalance->setText(QString::number(QString::fromStdString(o32_account_info["future_occupy_deposit_balance"]).toDouble(), 'f', 2));
        ui.futureEnableBalance->setText(QString::number(QString::fromStdString(o32_account_info["future_enable_deposit_balance"]).toDouble(), 'f', 2));

        ui.optionOccupyBalance->setText(QString::number(QString::fromStdString(o32_account_info["option_occupy_deposit_balance"]).toDouble(),'f',2));
        ui.optionEnableBalance->setText(QString::number(QString::fromStdString(o32_account_info["option_enable_deposit_balance"]).toDouble(),'f',2));

        // Risk InfoMation
        totalAsset = std::atof(o32_account_info["total_total_asset"].c_str()) + futureInfo.Balance;
        if (totalAsset != 0){
            fixedIncome = (std::atof(o32_account_info["total_bond_asset"].c_str()) + std::atof(o32_account_info["total_fund_asset"].c_str()) + std::atof(o32_account_info["total_repo_asset"].c_str())) / totalAsset;
            equityAsset = (std::atof(o32_account_info["total_stock_asset"].c_str())) / totalAsset;
            futureDeriAsset = (std::atof(o32_account_info["future_occupy_deposit_balance"].c_str())+ std::atof(o32_account_info["option_occupy_deposit_balance"].c_str()) + futureInfo.CurrMargin) / totalAsset;
            futureAsset = (std::atof(o32_account_info["future_occupy_deposit_balance"].c_str()) + futureInfo.CurrMargin) / totalAsset;

        }
        ui.totalAssetRatioLabel->setText(QString::number(totalAsset,'f',2));
        ui.BondsRatioLabel->setText(QString::number(fixedIncome,'f',3));
        ui.StockRatioLabel->setText(QString::number(equityAsset,'f',3));
        ui.futureAndDeriRatioLabel->setText(QString::number(futureDeriAsset,'f',3));
        ui.futurAssetRatioLabel->setText(QString::number(futureAsset, 'f', 3));

    }
    // ��Ʒ�Լ���������Ʒ
    if (futureDeriAsset > futureAndDeri_stop_level_){
        allowFutureDerivativeStop = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����ƽ���ߣ���Ʒ����Ʒ������") + QString::number(futureDeriAsset, 'g', 3) + QString::fromLocal8Bit(" ƽ���ߣ�") + QString::number(futureAndDeri_stop_level_, 'g', 3));
        LOG(ERROR) << "����ƽ���ߣ���Ʒ����Ʒ����:" << futureDeriAsset << ",ƽ����:" << futureAndDeri_stop_level_;
    }
    if (futureDeriAsset > futureAndDeri_warning_level_){
        allowFutureDerivativeWarning = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����Ԥ���ߣ���Ʒ����Ʒ������") + QString::number(futureDeriAsset, 'g', 3) + QString::fromLocal8Bit(" Ԥ���ߣ�") + QString::number(futureAndDeri_warning_level_, 'g', 3));
        LOG(WARNING) << "����Ԥ���ߣ���Ʒ����Ʒ����:" << futureDeriAsset << ",Ԥ����:" << futureAndDeri_warning_level_;
    }
    // ��Ʒ�ڻ�
    if (futureAsset > future_stop_level_){
        allowFutureTradeStop = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����ƽ���ߣ���Ʒ������") + QString::number(futureAsset, 'g', 3) + QString::fromLocal8Bit(" ƽ���ߣ�") + QString::number(future_stop_level_, 'g', 3));
        LOG(ERROR) << "����ƽ���ߣ���Ʒ����:" << futureAsset << ",ƽ����:" << future_stop_level_;
    }
    if (futureAsset > future_warning_level_){
        allowFutureTradeWarning = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����Ԥ���ߣ���Ʒ������") + QString::number(futureAsset, 'g', 3) + QString::fromLocal8Bit(" Ԥ���ߣ�") + QString::number(future_warning_level_, 'g', 3));
        LOG(WARNING) << "����Ԥ���ߣ���Ʒ����:" << futureAsset << ",Ԥ����:" << future_warning_level_;
    }
    // ��ƱȨ�����ʲ�
    if (equityAsset > stock_stop_level_){
        allowStockTradeStop = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����ƽ���ߣ�Ȩ���������") + QString::number(equityAsset, 'g', 3) + QString::fromLocal8Bit(" ƽ���ߣ�") + QString::number(stock_stop_level_, 'g', 3));
        LOG(ERROR) << "����ƽ���ߣ�Ȩ�������:" << equityAsset << ",ƽ����:" << stock_stop_level_;
    }
    if (equityAsset > stock_warning_level_){
        allowStockTradeWarning = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����Ԥ���ߣ�Ȩ���������") + QString::number(equityAsset, 'g', 3) + QString::fromLocal8Bit(" Ԥ���ߣ�") + QString::number(stock_warning_level_, 'g', 3));
        LOG(WARNING) << "����Ԥ���ߣ�Ȩ�������:" << equityAsset << ",Ԥ����:" << stock_warning_level_;
    }

    // �������ʲ�
    if (fixedIncome < fixed_stop_level_){
        allowFixedTradeStop = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����ƽ���ߣ������������") + QString::number(fixedIncome, 'g', 3) + QString::fromLocal8Bit(" ƽ���ߣ�") + QString::number(fixed_stop_level_, 'g', 3));
        LOG(ERROR) << "����ƽ���ߣ����������:" << fixedIncome << ",ƽ����:" << fixed_stop_level_;
    }
    else if (fixedIncome < fixed_warning_level_){
        allowFixedTradeWarning = false;
        ui.statusLabel->setText(QString::fromLocal8Bit("�����쳣��"));
        ui.loggingBrowser->append(QString::fromLocal8Bit("����Ԥ���ߣ������������") + QString::number(fixedIncome, 'g', 3) + QString::fromLocal8Bit(" Ԥ���ߣ�") + QString::number(fixed_warning_level_, 'g', 3));
        LOG(WARNING) << "����Ԥ���ߣ����������:" << fixedIncome << ",Ԥ����:" << fixed_warning_level_;
    }
    // show Status
    if (allowFutureTradeWarning && allowFutureDerivativeWarning && allowFixedTradeWarning && allowStockTradeWarning &&
        allowFutureTradeStop && allowFutureDerivativeStop && allowFixedTradeStop && allowStockTradeStop){
        ui.statusLabel->setText(QString::fromLocal8Bit("һ��������"));
    }
    LOG(INFO) << "Commodity:" << allowFutureTradeWarning << " " << allowFutureTradeStop <<
        " Derivative:" << allowFutureDerivativeWarning << " " << allowFutureDerivativeStop <<
        " FixedIncome:" << allowFixedTradeWarning << " " << allowFixedTradeStop <<
        " Equity:" << allowStockTradeWarning << " " << allowStockTradeStop;
}
