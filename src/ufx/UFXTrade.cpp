#include "UFXTrade.h"
#include <sstream>
using std::string;


UFXTrade::UFXTrade(){

    lpConnection_ = nullptr;

    o32_fundasset_ = std::make_shared<o32_fundasset>();

    on_disconnect_ = {};
}
UFXTrade::~UFXTrade(){
    //std::lock_guard<std::mutex> guard(mut_);
    if (lpConnection_ != nullptr){
        lpConnection_->Release();
    }
    
   
}
unsigned long UFXTrade::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long UFXTrade::AddRef()
{
    return 0;
}

unsigned long UFXTrade::Release()
{
    return 0;
}



void UFXTrade::OnConnect(CConnectionInterface *lpConnection)
{
    puts("UFXTrade::OnConnect");
}

void UFXTrade::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("UFXTrade::OnSafeConnect");
}

void UFXTrade::OnRegister(CConnectionInterface *lpConnection)
{
    puts("UFXTrade::OnRegister");
}

//断开连接后会调用Onclose函数，提示连接断开
void UFXTrade::OnClose(CConnectionInterface *lpConnection)
{
    //std::lock_guard<std::mutex> guard(mut_);
    if (on_disconnect_){
        on_disconnect_();

    }
    puts("UFXTrade::OnClose");
    //delete this;
}

void UFXTrade::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
    puts("UFXTrade::Onsent");
}

void UFXTrade::Reserved1(void *a, void *b, void *c, void *d)
{
    puts("UFXTrade::Reserved1");
}


void UFXTrade::Reserved2(void *a, void *b, void *c, void *d)
{
    puts("UFXTrade::Reserved2");
}

void UFXTrade::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
    puts("UFXTrade::OnReceivedBizEx");
}

//异步接受调用函数，用于异步发送包请求后，线程接受返回包
void UFXTrade::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
    puts("UFXTrade::OnReceivedBizMsg");
    if (lpMsg != NULL)
    {
        //成功,应用程序不能释放lpBizMessageRecv消息
        if (lpMsg->GetReturnCode() == 0)
        {
            //如果要把消息放到其他线程处理，必须自行拷贝，操作如下：
            //int iMsgLen = 0;
            //void * lpMsgBuffer = lpBizMessageRecv->GetBuff(iMsgLen);
            //将lpMsgBuffer拷贝走，然后在其他线程中恢复成消息可进行如下操作：
            //lpBizMessageRecv->SetBuff(lpMsgBuffer,iMsgLen);
            //没有错误信息
            int iLen = 0;
            const void * lpBuffer = lpMsg->GetContent(iLen);
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer, iLen);
            switch (lpMsg->GetFunction())
            {
            case 331100:
                if (lpUnPacker){
                    lpUnPacker->AddRef();
                    ShowPacket(lpUnPacker);
                    OnResponse_331100(lpUnPacker);
                    lpUnPacker->Release();
                }
                break;
            case 333104:
                if (lpUnPacker){
                    lpUnPacker->AddRef();
                    OnResponse_333104(lpUnPacker);
                    lpUnPacker->Release();
                }
                break;
            default:
                if (lpUnPacker){
                    lpUnPacker->AddRef();
                    ShowPacket(lpUnPacker);
                    lpUnPacker->Release();
                }
                break;
            }
        }
        else
        {
            int iLen = 0;
            const void * lpBuffer = lpMsg->GetContent(iLen);
            IF2UnPacker * lpUnPacker = NewUnPacker((void *)lpBuffer, iLen);
            if (lpUnPacker != NULL)
            {
                lpUnPacker->AddRef();//添加释放内存引用
                ShowPacket(lpUnPacker);
                lpUnPacker->Release();
            }
            else
            {
                printf("业务包是空包，错误代码：%d，错误信息:%s\n", lpMsg->GetErrorNo(), lpMsg->GetErrorInfo());
            }
        }
    }
}
void UFXTrade::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{

}

int  UFXTrade::Reserved3()
{
    return 0;
}

void UFXTrade::Reserved4()
{
}

void UFXTrade::Reserved5()
{
}

void UFXTrade::Reserved6()
{
}

void UFXTrade::Reserved7()
{
}

void UFXTrade::SetConfigInterface(CConfigInterface* lpConfig)
{
    //lpConfig_ = lpConfig;
    puts("UFXTrade::SetConfigInterface");
}

void UFXTrade::ShowPacket(IF2UnPacker *lpUnPacker){
    int i = 0, t = 0, j = 0, k = 0;

    for (i = 0; i < lpUnPacker->GetDatasetCount(); ++i)
    {
        // 设置当前结果集

        printf("记录集：%d/%d\r\n", i + 1, lpUnPacker->GetDatasetCount());
        lpUnPacker->SetCurrentDatasetByIndex(i);

        // 打印所有记录
        for (j = 0; j < (int)lpUnPacker->GetRowCount(); ++j)
        {
            printf("\t第%d/%d条记录：\r\n", j + 1, lpUnPacker->GetRowCount());
            // 打印每条记录
            for (k = 0; k < lpUnPacker->GetColCount(); ++k)
            {
                switch (lpUnPacker->GetColType(k))
                {
                case 'I':
                    printf("\t【整数】%20s = %35d\r\n", lpUnPacker->GetColName(k), lpUnPacker->GetIntByIndex(k));
                    break;

                case 'C':
                    printf("\t【字符】%20s = %35c\r\n", lpUnPacker->GetColName(k), lpUnPacker->GetCharByIndex(k));
                    break;

                case 'S':

                    if (NULL != strstr((char *)lpUnPacker->GetColName(k), "password"))
                    {
                        printf("\t【字串】%20s = %35s\r\n", lpUnPacker->GetColName(k), "******");
                    }
                    else
                        printf("\t【字串】%20s = %35s\r\n", lpUnPacker->GetColName(k), lpUnPacker->GetStrByIndex(k));
                    break;

                case 'F':
                    printf("\t【数值】%20s = %35f\r\n", lpUnPacker->GetColName(k), lpUnPacker->GetDoubleByIndex(k));
                    break;

                case 'R':
                {
                    int nLength = 0;
                    void *lpData = lpUnPacker->GetRawByIndex(k, &nLength);
                    switch (nLength){
                    case 0:
                        printf("\t【数据】%20s = %35s\r\n", lpUnPacker->GetColName(k), "(N/A)");
                        break;
                    default:
                        printf("\t【数据】%20s = 0x", lpUnPacker->GetColName(k));
                        for (t = nLength; t < 11; t++){
                            printf("   ");
                        }
                        unsigned char *p = (unsigned char *)lpData;
                        for (t = 0; t < nLength; t++){
                            printf("%3x", *p++);
                        }
                        printf("\r\n");
                        break;
                    }
                    // 对2进制数据进行处理
                    break;
                }

                default:
                    // 未知数据类型
                    printf("未知数据类型。\n");
                    break;
                }
            }

            putchar('\n');

            lpUnPacker->Next();
        }

        putchar('\n');
    }
}

int32 UFXTrade::init(CConfigInterface* lpConfig,o32Config o32_config){

    int32 init_result = 0;
    try{
        if (lpConnection_ != nullptr){
            lpConnection_->Release();
            lpConnection_ = nullptr;
        }
        lpConnection_ = NewConnection(lpConfig);
        lpConnection_->AddRef();
        // 初始化连接对象
        init_result = lpConnection_->Create2BizMsg(this);
        if (init_result != 0){
            RISK_LOG("Create callBack failed ,result:" << init_result << " Msg: " << lpConnection_->GetErrorMsg(init_result));
            return -1;
        }
        RISK_LOG("Create callBack success");
        // 开始连接/注册
        init_result = lpConnection_->Connect(3000);
        if (init_result != 0){
            RISK_LOG("Connect failed ,result:" << init_result << " Msg: " << lpConnection_->GetErrorMsg(init_result));
            return -2;
        }
        RISK_LOG("O32 connect success");

        //call back 
        {
            on_disconnect_ = [](){
                global::need_reconnect.store(true);
            };
        }
        
        o32_config_ = o32_config;

        return 0;
    }
    catch (std::exception e){
        RISK_LOG("Init failed ,Msg:" << e.what() );
        return -3;
    }
}
int32 UFXTrade::start(){
    int32 start_result = 0;
    try{
        IF2Packer* lpRequestPacker = MakeLoginPacker(o32_config_.fund_account.c_str(), o32_config_.passwd.c_str());
        IF2UnPacker* lpUnPacker;
        //登录功能号：10001
        ErrorInfo errorInfo = CallService(this->lpConnection_, 10001, lpRequestPacker, &lpUnPacker);
        if (errorInfo.ErrorCode != 0)
        {
            RISK_LOG("Connect failed ,code:" << errorInfo.ErrorCode << " Msg: " << lpConnection_->GetErrorMsg(errorInfo.ErrorCode));
            return -1;
        }
        if (lpUnPacker->FindColIndex("user_token") >= 0)
        {
            user_token_ = lpUnPacker->GetStr("user_token");
        }
        else
        {
            RISK_LOG("user token is not at unPacker. ");
            return -2;
        }
        lpRequestPacker->FreeMem(lpRequestPacker->GetPackBuf());
        lpRequestPacker->Release();

        RISK_LOG("O32 login success.token no :" << user_token_);

        ////查询账户信息
        //QueryFundaset();

        return start_result;
        
    }
    catch (std::exception e){
        RISK_LOG("Init failed ,Msg:" << e.what());
        return -1;
    }

}


IF2Packer* UFXTrade::MakeLoginPacker(string operatorNo, string password){
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("operator_no", 'S', 16, 0);
    requestPacker->AddField("password", 'S', 32, 0);
    requestPacker->AddField("mac_address", 'S', 255, 0);
    requestPacker->AddField("op_station", 'S', 255, 0);
    requestPacker->AddField("ip_address", 'S', 32, 0);
    requestPacker->AddField("authorization_id", 'S', 64, 0);
    requestPacker->AddStr(operatorNo.c_str());
    requestPacker->AddStr(password.c_str());
    requestPacker->AddStr("5C-26-0A-2F-82-4F");
    requestPacker->AddStr("192.168.88.123|5C-26-0A-2F-82-4F");
    requestPacker->AddStr("192.168.88.123");
    requestPacker->AddStr("hd123456");
    requestPacker->EndPack();
    return requestPacker;

}
ErrorInfo UFXTrade::CallService(CConnectionInterface* connection, int functionNo, IF2Packer* requestPacker, IF2UnPacker** responseUnPacker)
{
    ErrorInfo errorInfo;
    int ret = connection->SendBiz(functionNo, requestPacker);
    if (ret > 0)
    {
        void* Pointer;
        ret = connection->RecvBiz(ret, &Pointer, 3000);
        switch (ret)
        {
        case 0:
        case 1:
            //应答包一般包含两个数据集，
            //第一个数据集是头部信息，包含有ErrorCode和ErrorMsg等信息
            //第二个数据集则包含具体的业务数据
            (*responseUnPacker) = (IF2UnPacker*)Pointer;
            errorInfo = GetErrorInfo(*responseUnPacker);
            if ((*responseUnPacker)->GetDatasetCount() > 1) (*responseUnPacker)->SetCurrentDatasetByIndex(1);
            break;
        case 2:
            errorInfo.ErrorCode = 2;
            errorInfo.ErrorMsg = (char*)Pointer;
            break;
        default:
            errorInfo.ErrorCode = 3;
            errorInfo.ErrorMsg = connection->GetErrorMsg(ret);
            break;

        }
    }
    else
    {
        errorInfo.ErrorCode = ret;
        errorInfo.ErrorMsg = connection->GetErrorMsg(ret);
    }
    return errorInfo;
}

ErrorInfo UFXTrade::GetErrorInfo(IF2UnPacker* responseUnPacker)
{
    ErrorInfo errorInfo;
    errorInfo.ErrorCode = responseUnPacker->GetInt("ErrorCode");
    if (responseUnPacker->GetStr("ErrorMsg"))
        errorInfo.ErrorMsg = responseUnPacker->GetStr("ErrorMsg");
    return errorInfo;
}

ErrorInfo UFXTrade::QueryFundaset(){

    IF2UnPacker*          responseUnPacker;
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token", 'S', 512);
    requestPacker->AddField("account_code", 'S', 32);

    requestPacker->AddStr(user_token_.c_str());
    requestPacker->AddStr(o32_config_.fund_account.c_str());

    requestPacker->EndPack();

    ErrorInfo errorInfo = CallService(this->lpConnection_, 35003, requestPacker, &responseUnPacker);
    requestPacker->FreeMem(requestPacker->GetPackBuf());
    requestPacker->Release();
    // show
    makeFundaAsset(responseUnPacker,"total_");
    return errorInfo;

}



void UFXTrade::makeFundaAsset(IF2UnPacker *lpUnPacker,std::string prefix){
    bool isEmpty = lpUnPacker->IsEmpty();
    //先取第二个结果集，如果失败则取第一个结果集
    if (isEmpty){
        return;
    }
    if (lpUnPacker->SetCurrentDatasetByIndex(1) == 0){
        lpUnPacker->SetCurrentDatasetByIndex(0);
    }
    string fieldName;

    unsigned int rowCount = 0;
    int colCount = 0;
    for (rowCount = 0; rowCount < lpUnPacker->GetRowCount(); rowCount++){
        for (colCount = 0; colCount < lpUnPacker->GetColCount(); colCount++){
            int type = lpUnPacker->GetColType(colCount);
            std::stringstream value;
            fieldName = lpUnPacker->GetColName(colCount);
            //std::cout << fieldName << std::endl;
            switch (type){
            case 'I':
                value << lpUnPacker->GetIntByIndex(colCount);
                break;
            case 'C':
                value << lpUnPacker->GetCharByIndex(colCount);
                break;
            case 'S':
                value << lpUnPacker->GetStrByIndex(colCount);
                break;
            case 'F':
                value << lpUnPacker->GetDoubleByIndex(colCount);
                break;
            case 'R':
                value << "RawData";
                //this->o32_fund_asset_map_.insert(std::make_pair("RawData", "RawData"));
                break;
            }
            this->o32_fund_asset_map_[prefix+fieldName] = value.str();
        }
        lpUnPacker->Next();
    }
}

bool UFXTrade::ParseFundaAssetInfo_UFX(o32_fundasset *fundassetInfo, char *inDataStr){

    memset(fundassetInfo, 0, sizeof(o32_fundasset));
    BEGINPPARSE(o32_fundasset, fundassetInfo, inDataStr, UDP_DELIMITE_STR);
    PARSEVALUE(account_code);
    PARSEVALUE(currency_code);
    PARSEVALUE(total_asset);
    PARSEVALUE(nav);
    PARSEVALUE(yesterday_nav);
    PARSEVALUE(current_balance);
    PARSEVALUE(begin_balance);
    PARSEVALUE(futu_deposit_balance);
    PARSEVALUE(occupy_deposit_balance);
    PARSEVALUE(futu_asset);
    PARSEVALUE(stock_asset);
    PARSEVALUE(bond_asset);
    PARSEVALUE(fund_asset);
    PARSEVALUE(repo_asset);
    PARSEVALUE(other_asset);
    PARSEVALUE(fund_share);
    PARSEVALUE(fund_net_asset);
    PARSEVALUE(payable_balance);
    PARSEVALUE(receivable_balance);
    ENDPARSE;
    return ISPACKValid;

}

std::map<std::string, string> UFXTrade::getFundAssetMap(){
    return this->o32_fund_asset_map_;

}

void UFXTrade::ShowFundAssetMap(){
    for (auto iter = o32_fund_asset_map_.begin(); iter != o32_fund_asset_map_.end(); iter++){
        std::cout << iter->first << " " << iter->second << std::endl;
    }

}


// 查询期货保证金
ErrorInfo UFXTrade::QueryFutuBail(){
    IF2UnPacker*          responseUnPacker;
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token", 'S', 512);
    requestPacker->AddField("account_code", 'S', 32);
    requestPacker->AddField("asset_no", 'S', 32);

    requestPacker->AddStr(user_token_.c_str());
    requestPacker->AddStr(o32_config_.fund_account.c_str());
    requestPacker->AddStr(o32_config_.asset_no.c_str());

    requestPacker->EndPack();

    ErrorInfo errorInfo = CallService(this->lpConnection_, 34003, requestPacker, &responseUnPacker);
    requestPacker->FreeMem(requestPacker->GetPackBuf());
    requestPacker->Release();
    // show
    makeFundaAsset(responseUnPacker,"future_");
    return errorInfo;
}
// 查询期权保证金
ErrorInfo UFXTrade::QueryOptionBail(){

    IF2UnPacker*          responseUnPacker;
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token", 'S', 512);
    requestPacker->AddField("account_code", 'S', 32);
    requestPacker->AddField("asset_no", 'S', 32);
    requestPacker->AddField("market_no", 'S', 3);

    requestPacker->AddStr(user_token_.c_str());
    requestPacker->AddStr(o32_config_.fund_account.c_str());
    requestPacker->AddStr(o32_config_.asset_no.c_str());
    requestPacker->AddStr("1");// 默认上交所

    requestPacker->EndPack();

    ErrorInfo errorInfo = CallService(this->lpConnection_, 34004, requestPacker, &responseUnPacker);
    requestPacker->FreeMem(requestPacker->GetPackBuf());
    requestPacker->Release();
    // show
    makeFundaAsset(responseUnPacker,"option_");
    return errorInfo;



}