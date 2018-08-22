/*
 *   UFX�ӿ��첽ʹ��ʾ��(C++��)
*
*    UFX��ʹ�ù�����ʵ����ʹ��t2sdk��������UFX�������������ӡ����Ͳ�����ҵ����Ϣ�Ĺ��̡�
*    ���У�T2SDK��������ҵ���޹صģ���ʹ�ÿ��Բο���T2SDK �ⲿ�濪��ָ��.docx��
*               ҵ����Ϣ�Ķ�������ҵ����صģ�ÿ���ӿڶ����Լ��Ķ��壬���Բο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶_x.x.x.x.xls��
*/

#include "t2sdk_interface.h"
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <Windows.h>
//#include <process.h>
#include "ResultSet.h"
using namespace std;


class CCallback : public CCallbackInterface
{
public:
    // ��ΪCCallbackInterface�����մ��������IKnown��������Ҫʵ��һ����3������
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    // �����¼�����ʱ�Ļص�������ʵ��ʹ��ʱ���Ը�����Ҫ��ѡ��ʵ�֣����ڲ���Ҫ���¼��ص���������ֱ��return
    // Reserved?Ϊ����������Ϊ�Ժ���չ��׼����ʵ��ʱ��ֱ��return��return 0��
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);
    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);
    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);
    int  FUNCTION_CALL_MODE Reserved3();
    void FUNCTION_CALL_MODE Reserved4();
    void FUNCTION_CALL_MODE Reserved5();
    void FUNCTION_CALL_MODE Reserved6();
    void FUNCTION_CALL_MODE Reserved7();
    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);
    void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
    void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);
};

class CSubCallback : public CSubCallbackInterface
{
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);
    unsigned long  FUNCTION_CALL_MODE AddRef();
    unsigned long  FUNCTION_CALL_MODE Release();

    void FUNCTION_CALL_MODE OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData);
    void FUNCTION_CALL_MODE OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo);
};



bool gbIsRun = false;
list<CResultSet*> gReportList;
list<CResultSet*> gResultList;
map<int,int> gWaitRspMap;
map<int,CSubscribeParamInterface*> gAllSubscribeParam;
HANDLE ghThreadReportProcess = NULL;
CRITICAL_SECTION critical_sec_,critical_sec2_;

int Connect(const char* serverAddr, CConnectionInterface** connection, CCallback* callback);
int Login(CConnectionInterface* connection, const char* operatorNo, const char* password);
int BasketEntrust(CConnectionInterface* connection, const char* userToken);
int Withdraw(CConnectionInterface* connection, const char* userToken, int entrustNo);
void processLoginRsp(CResultSet* result, string& userToken);
void processEntrustRsp(CResultSet* result, int& entrustNo);
void processWithdrawRsp(CResultSet* result, int& entrustNo2);
void runReportProcess();
void ShowSubscribe(int subIndex,LPSUBSCRIBE_RECVDATA lpRecvData);
void ShowPacket(IF2UnPacker* unPacker);

static DWORD WINAPI startReportProcessThread (LPVOID)
{
    gbIsRun = true;
    runReportProcess();
    return 0;
}

int main(int argc, char** argv)
{
    InitializeCriticalSection(&critical_sec_);
    InitializeCriticalSection(&critical_sec2_);
    /*
    if (argc < 4)
    {
        cout << "Usage: UFXDemo serverAddr operatorNo, password, accountCode combiNo" << endl
             << "Examples:" << endl
             << "    UFXDemo 192.168.54.57:9012 1000 0 184693 1000005" << endl;
        getchar();
        return 0;
    }*/

    //����������Ϣ�����߳�
    ghThreadReportProcess = CreateThread(NULL,
        0,
        startReportProcessThread,
        NULL,
        0,
        NULL
        );
    if (ghThreadReportProcess == NULL)
    {
        cout << "����������Ϣ�����߳�ʧ�ܡ�" << endl;
        getchar();
        return -1;
    }

    //ͨ��T2SDK����������������ȡһ���µ�CConfig����ָ��
    //�˶����ڴ������Ӷ���ʱ�����ݣ��������������������Ӷ���ĸ������ԣ����������IP��ַ����ȫģʽ�ȣ�
    //ֵ��ע����ǣ��������ö�������������Ϣʱ��������Ϣ�ȿ��Դ�ini�ļ������룬
    //Ҳ�����ڳ���������趨��������2�ߵĻ�ϣ������ͬһ���������費ͬ��ֵ���������һ������Ϊ׼
    CConfigInterface * lpConfig = NewConfig();
    lpConfig->AddRef();
    lpConfig->Load("subscriber.ini");
    //�����Ҫʹ�÷������Ĺ��ܣ�������������mc��ǩ�����client_name������ļ��������ˣ�����Ҫ�������������
    //lpConfig->SetString("mc","client_name","xuxp");

    CConnectionInterface* subConnection = NewConnection(lpConfig);
    subConnection->AddRef();

    CCallback callback0;
    CSubCallback subscriberCallback;
    CSubscribeInterface* lpSub = NULL;

    int ret = 0;

    //��ʼ�����Ӷ��󣬷���0��ʾ��ʼ���ɹ���ע���ʱ��û��ʼ���ӷ�����
    if (subConnection->Create2BizMsg(&callback0) == 0)
    {
        //��ʽ��ʼ���ӣ�����1000Ϊ��ʱ��������λ��ms
        if (subConnection->Connect(1000) == 0)
        {
            char* bizName = (char*)lpConfig->GetString("subcribe","biz_name","");
            //�����Ҫʹ�÷������Ĺ��ܣ�������������mc��ǩ�����client_name��
            lpSub = subConnection->NewSubscriber(&subscriberCallback,bizName,5000);
            if (!lpSub)
            {
                printf("NewSubscribe Error: %s\n",subConnection->GetMCLastError());
                return -1;
            }
            lpSub->AddRef();


            //���Ĳ�����ȡ
            CSubscribeParamInterface* lpSubscribeParam = NewSubscribeParam();
            lpSubscribeParam->AddRef();
            char* topicName = (char*)lpConfig->GetString("subcribe","topic_name","");//��������
            lpSubscribeParam->SetTopicName(topicName);
            char* isFromNow = (char*)lpConfig->GetString("subcribe","is_rebulid","");//�Ƿ�ȱ
            if (strcmp(isFromNow,"true")==0)
            {
                lpSubscribeParam->SetFromNow(true);
            }
            else
            {
                lpSubscribeParam->SetFromNow(false);
            }

            char* isReplace = (char*)lpConfig->GetString("subcribe","is_replace","");//�Ƿ񸲸�
            if (strcmp(isReplace,"true")==0)
            {
                lpSubscribeParam->SetReplace(true);
            }
            else
            {
                lpSubscribeParam->SetReplace(false);
            }

            char* lpApp = "lixuebin";
            lpSubscribeParam->SetAppData(lpApp,9);//��Ӹ�������

            //��ӹ����ֶ�
            int nCount = lpConfig->GetInt("subcribe","filter_count",0);
            for (int i=1;i<=nCount;i++)
            {
                char lName[128]={0};
                sprintf(lName,"filter_name%d",i);
                char* filterName = (char*)lpConfig->GetString("subcribe",lName,"");
                char lValue[128]={0};
                sprintf(lValue,"filter_value%d",i);
                char* filterValue = (char*)lpConfig->GetString("subcribe",lValue,"");
                lpSubscribeParam->SetFilter(filterName,filterValue);
            }
            //��ӷ���Ƶ��
            lpSubscribeParam->SetSendInterval(lpConfig->GetInt("subcribe","send_interval",0));
            //��ӷ����ֶ�
            nCount = lpConfig->GetInt("subcribe","return_count",0);
            for (int k=1;k<=nCount;k++)
            {
                char lName[128]={0};
                sprintf(lName,"return_filed%d",k);
                char* filedName = (char*)lpConfig->GetString("subcribe",lName,"");
                lpSubscribeParam->SetReturnFiled(filedName);
            }

            IF2Packer* pack = NewPacker(2);
            pack->AddRef();

            //����һ��ҵ���
            pack->BeginPack();
            pack->AddField("login_operator_no");
            pack->AddField("password");
            pack->AddStr("99991000");
            pack->AddStr("0");
            pack->EndPack();
            IF2UnPacker* lpBack = NULL;

            int subscribeIndex = 0;
            printf("��ʼ����\n");
            int  iRet = lpSub->SubscribeTopic(lpSubscribeParam,5000,&lpBack,pack);
            if(iRet>0)
            {
                subscribeIndex = iRet;
                printf("SubscribeTopic info:[%d] �ɹ�\n",iRet);
                gAllSubscribeParam[subscribeIndex] = lpSubscribeParam;//���浽map�У������Ժ��ȡ������
            }
            else
            {
                if(lpBack != NULL) ShowPacket(lpBack);
                printf("SubscribeTopic info:[%d] %s\n",iRet,subConnection->GetErrorMsg(iRet));
                return-1;
            }
        }    
    }
    else
    {
        puts(subConnection->GetErrorMsg(ret));
    }

    // getVersionInfo
    int versionId = GetVersionInfo();
    cout << versionId << endl;

    string serverAddr  = "192.168.54.57:9115"; //UFX��������ַ
    string operatorNo  = "99991000";               //����Ա
    string password    = "0";                  //����
    //string accountCode = "184693";             //�˻�����
    //string combiNo     = "1000005";            //��ϴ���          
    if (argc >= 2)
    {
        serverAddr  = argv[1];
        //accountCode = argv[2];
        //combiNo     = argv[3];
    }

    //string marketNo         = "1";         //�����г�
    //string stockCode        = "600036";    //֤ȯ����
    //string entrustDirection = "1";         //ί�з���
    //double entrustPrice     = 10.10;       //ί�м۸�
    //int    entrustAmount    = 100;         //ί������

    CCallback             callback;
    CConnectionInterface* connection;
    string                userToken;
    int                   entrustNo;
    int                   entrustNo2;

    //����
    ret = Connect(serverAddr.c_str(), &connection, &callback);
    if (ret != 0)
    {        
        cout << "����ʧ�ܡ�������ϢΪ��" << connection->GetErrorMsg(ret) << endl;
        getchar();
        return -1;
    }     
    else
    {
        cout << "���ӳɹ���" << endl;
    }

    int iOrderID = 0;
    while (1)
    {
        cout << endl;
        cout << "1����¼ 2���µ� 3������ 0���˳�" << endl;
        cout << "������ָ��ţ�";
        scanf("%d",&iOrderID);
        switch(iOrderID)
        {
        case 0:
            {
                gbIsRun = false;
                Sleep(1000);
                connection->Release();
                return 0;
            }
        case 1:
            {
                //��¼
                int handler = Login(connection, operatorNo.c_str(), password.c_str());
                if (handler > 0)
                {
                    gWaitRspMap.insert(make_pair(handler,10001));
                    Sleep(1000);
                }
                else
                {
                    cout << "��¼ʧ�ܡ�������ϢΪ��" << connection->GetErrorMsg(handler) << endl;
                }
                break;
            }
        case 2:
            {
                //�µ�
                int handler = BasketEntrust(connection, userToken.c_str());
                if (handler > 0)
                {
                    gWaitRspMap.insert(make_pair(handler,91090));
                    Sleep(2000);
                }
                else
                {
                    cout << "�µ�ʧ�ܡ�������ϢΪ��" << connection->GetErrorMsg(handler) << endl;
                }
                break;
            }
        case 3:
            {
                //����
                int handler = Withdraw(connection, userToken.c_str(), entrustNo);
                if (handler > 0)
                {
                    gWaitRspMap.insert(make_pair(handler,91101));
                    Sleep(1000);
                }
                else
                {
                    cout << "����ʧ�ܡ�������ϢΪ��" << connection->GetErrorMsg(handler) << endl;
                }
                break;
            }
        default:
            {
                cout << "�����ָ��Ų���ȷ��" << endl;
                //continue;
            }
        }

        EnterCriticalSection(&critical_sec_);
        list<CResultSet*> tmpResultList;
        if (!gResultList.empty())
        {
            tmpResultList.swap(gResultList);
        }
        LeaveCriticalSection(&critical_sec_);

        //����ָ����
        list<CResultSet*>::iterator it = tmpResultList.begin();
        for (; it != tmpResultList.end(); it++)
        {
            CResultSet* resultSet = *it;
            int funcNo = 0;
            map<int,int>::iterator req_it = gWaitRspMap.find(resultSet->getHandlerID());
            if (req_it == gWaitRspMap.end())
            {
                cout << "process result: can't find request for handler(" << resultSet->getHandlerID() << "), discard." << endl;
                delete resultSet;
                continue;
            }
            else
            {
                funcNo = (*req_it).second;
                gWaitRspMap.erase(req_it);
            }

            if (funcNo == 10001)
            {
                processLoginRsp(resultSet, userToken);
            }
            else if (funcNo == 91090)
            {
                processEntrustRsp(resultSet, entrustNo);
            }
            else if (funcNo == 91101)
            {
                processWithdrawRsp(resultSet, entrustNo2);
            }

            delete resultSet;
        }
        tmpResultList.clear();
    }


    connection->Release();
    lpSub->Release();
    subConnection->Release();
    getchar();    
    return 0;
}


void ShowSubscribe(int subIndex,LPSUBSCRIBE_RECVDATA lpRecvData)
{
    map<int,CSubscribeParamInterface*>::iterator it = gAllSubscribeParam.find(subIndex);
    if (it == gAllSubscribeParam.end())
    {
        printf("û�����������\n");
        return;
    }
    CSubscribeParamInterface* lpSubParam = (*it).second;

    printf("----------�������-------\n");
    printf("�������֣�           %s\n",lpSubParam->GetTopicName());
    printf("�������ݳ��ȣ�       %d\n",lpRecvData->iAppDataLen);
    if (lpRecvData->iAppDataLen > 0)
    {
        printf("�������ݣ�           %s\n",lpRecvData->lpAppData);
    }
    printf("�����ֶβ��֣�\n");
    if (lpRecvData->iFilterDataLen > 0)
    {
        IF2UnPacker* lpUnpack = NewUnPacker(lpRecvData->lpFilterData,lpRecvData->iFilterDataLen);
        lpUnpack->AddRef();
        ShowPacket(lpUnpack);
        lpUnpack->Release();
    }
    printf("---------------------------\n");
}

void ShowPacket(IF2UnPacker* unPacker)
{
    int i = 0, t = 0, j = 0, k = 0;

    for (i = 0; i < unPacker->GetDatasetCount(); ++i)
    {
        // ���õ�ǰ�����
        unPacker->SetCurrentDatasetByIndex(i);

        // ��ӡ�ֶ�
        for (t = 0; t < unPacker->GetColCount(); ++t)
        {
            printf("%20s", unPacker->GetColName(t));
        }

        putchar('\n');

        // ��ӡ���м�¼
        for (j = 0; j < (int)unPacker->GetRowCount(); ++j)
        {
            // ��ӡÿ����¼
            for (k = 0; k < unPacker->GetColCount(); ++k)
            {
                switch (unPacker->GetColType(k))
                {
                case 'I':
                    printf("%20d", unPacker->GetIntByIndex(k));
                    break;

                case 'C':
                    printf("%20c", unPacker->GetCharByIndex(k));
                    break;

                case 'S':
                    printf("%20s", unPacker->GetStrByIndex(k));
                    break;

                case 'F':
                    printf("%20f", unPacker->GetDoubleByIndex(k));
                    break;

                case 'R':
                    {
                        int nLength = 0;
                        void *lpData = unPacker->GetRawByIndex(k, &nLength);

                        // ��2�������ݽ��д���
                        break;
                    }

                default:
                    // δ֪��������
                    printf("δ֪�������͡�\n");
                    break;
                }
            }

            putchar('\n');

            unPacker->Next();
        }

        putchar('\n');
    }
}

int CallService(CConnectionInterface* connection, IBizMessage* requestBizMessage)
{
    return connection->SendBizMsg(requestBizMessage,1);
}

int Connect(const char* serverAddr, CConnectionInterface** connection, CCallback* callback)
{
    //CConfigInterface��CConnectionInterface��ʹ�ÿ��Բο���T2SDK �ⲿ�濪��ָ��.docx��
    //����T2SDK���ö��󣬲�����UFX��������ַ�Լ�License�ļ�
    CConfigInterface * lpConfig = NewConfig();
    lpConfig->AddRef();
    lpConfig->SetString("t2sdk", "servers", serverAddr);    
    lpConfig->SetString("t2sdk", "license_file", "license.dat");

    //�������Ӷ��󣬲�����UFX������
    (*connection) = NewConnection(lpConfig);
    (*connection)->AddRef();
    int ret = (*connection)->Create2BizMsg(callback);
    if (ret != 0)
    {
        (*connection)->Release();
        lpConfig->Release();
        return ret;
    }

    //����UFX������������3000Ϊ��ʱ��������λ����
    ret = (*connection)->Connect(3000); 
    if (ret != 0)
    {
        (*connection)->Release();
        lpConfig->Release();
        return ret;
    }

    lpConfig->Release();
    return ret;
}

IF2Packer* MakeLoginPacker(const char* operatorNo, const char* password)
{
    //ҵ��������Ϣ���ɽӿڹ淶����ģ����Բο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶_x.x.x.x.xls��
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("operator_no",     'S',16, 0);
    requestPacker->AddField("password",        'S',32, 0);
    requestPacker->AddField("mac_address",     'S',255,0);
    requestPacker->AddField("op_station",      'S',255,0);
    requestPacker->AddField("ip_address",      'S',32, 0);
    requestPacker->AddField("authorization_id",'S',64, 0);
    requestPacker->AddStr(operatorNo);
    requestPacker->AddStr(password);
    requestPacker->AddStr("5C-26-0A-2F-82-4F");
    requestPacker->AddStr("192.168.88.123|5C-26-0A-2F-82-4F");
    requestPacker->AddStr("192.168.88.123");
    requestPacker->AddStr("hd123456");    
    requestPacker->EndPack();
    return requestPacker;
}

int Login(CConnectionInterface* connection, const char* operatorNo, const char* password)
{
    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();
    //��¼���ܺţ�10001
    lpBizMessage->SetFunction(10001);
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    IF2Packer* lpPacker = MakeLoginPacker(operatorNo,password);
    lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
    int ret = CallService(connection,lpBizMessage);
    lpPacker->FreeMem(lpPacker->GetPackBuf());
    lpPacker->Release();
    lpBizMessage->Release();
    return ret;
}

IF2Packer* MakeEntrustPacker(const char* userToken)
{
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token",        'S',512,0);
    requestPacker->AddField("batch_no",          'I',8,0);
    requestPacker->AddField("account_code",      'S',32,0);
    requestPacker->AddField("combi_no"  ,        'S',8,0); 
    requestPacker->AddField("market_no" ,        'S',3,0);
    requestPacker->AddField("stock_code",        'S',16,0);
    requestPacker->AddField("entrust_direction", 'S',1,0);
    requestPacker->AddField("futures_direction", 'S',1,0);
    requestPacker->AddField("price_type",        'S',1,0);
    requestPacker->AddField("entrust_price",     'F',9,3);
    requestPacker->AddField("entrust_amount",    'F',16,2);
    requestPacker->AddStr(userToken);
    requestPacker->AddInt(5088888);
    requestPacker->AddStr("106000");
    requestPacker->AddStr("106A1");
    requestPacker->AddStr("1");
    requestPacker->AddStr("600000");
    requestPacker->AddStr("1");
    requestPacker->AddStr("");                   //��Ʊû�п�ƽ����,���Դ���
    requestPacker->AddStr("0");                  //�޼�
    requestPacker->AddDouble(13.34);
    requestPacker->AddDouble(100);
    requestPacker->AddStr(userToken);
    requestPacker->AddInt(5088888);
    requestPacker->AddStr("106000");
    requestPacker->AddStr("106A1");
    requestPacker->AddStr("2");
    requestPacker->AddStr("000001");
    requestPacker->AddStr("2");
    requestPacker->AddStr("");                   //��Ʊû�п�ƽ����,���Դ���
    requestPacker->AddStr("0");                  //�޼�
    requestPacker->AddDouble(15.00);
    requestPacker->AddDouble(100);
    requestPacker->AddStr(userToken);
    requestPacker->AddInt(5088888);
    requestPacker->AddStr("106000");
    requestPacker->AddStr("106FA1");
    requestPacker->AddStr("7");
    requestPacker->AddStr("IF1503");
    requestPacker->AddStr("1");                   //����
    requestPacker->AddStr("1");                   //����
    requestPacker->AddStr("0");                  //�޼�
    requestPacker->AddDouble(3622.8);
    requestPacker->AddDouble(1);
    requestPacker->EndPack();
    return requestPacker;
}

int BasketEntrust(CConnectionInterface* connection, const char* userToken)
{
    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();
    //ί�й��ܺţ�91090���ӿڹ��ܺż��������������������Բο�������Ͷ�ʹ���ϵͳO3.2_�ܱ߽ӿڹ淶_x.x.x.x.xls��
    lpBizMessage->SetFunction(91090);
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    IF2Packer* lpPacker = MakeEntrustPacker(userToken);
    lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
    int ret = CallService(connection,lpBizMessage);
    lpPacker->FreeMem(lpPacker->GetPackBuf());
    lpPacker->Release();
    lpBizMessage->Release();
    return ret;
}

IF2Packer* MakeWithdrawPacker(const char* userToken, int entrustNo)
{
    IF2Packer* requestPacker = NewPacker(2);
    requestPacker->AddRef();
    requestPacker->BeginPack();
    requestPacker->AddField("user_token",'S',512,0);
    requestPacker->AddField("entrust_no",'I');
    requestPacker->AddStr(userToken);
    requestPacker->AddInt(entrustNo);
    requestPacker->EndPack();
    return requestPacker;
}

int Withdraw(CConnectionInterface* connection, const char* userToken, int entrustNo)
{
    IBizMessage* lpBizMessage = NewBizMessage();
    lpBizMessage->AddRef();
    //�������ܺţ�91101
    lpBizMessage->SetFunction(91101);
    lpBizMessage->SetPacketType(REQUEST_PACKET);
    IF2Packer* lpPacker = MakeWithdrawPacker(userToken,entrustNo);
    lpBizMessage->SetContent(lpPacker->GetPackBuf(),lpPacker->GetPackLen());
    int ret = CallService(connection,lpBizMessage);
    lpPacker->FreeMem(lpPacker->GetPackBuf());
    lpPacker->Release();
    lpBizMessage->Release();
    return ret;
}

void processLoginRsp(CResultSet* result, string& userToken)
{
    if (result->hasField("user_token"))
    {
        result->getValueByName("user_token",userToken);
        cout << "login response: user_token = " << userToken.c_str() << endl;
    }
}

void processEntrustRsp(CResultSet* result, int& entrustNo)
{
    if (result->hasField("entrust_no"))
    {
        result->first();
        while (!result->eof())
        {
            string sBatchNo;
            string sEntrustNo;
            result->getValueByName("batch_no",sBatchNo);
            result->getValueByName("entrust_no",sEntrustNo);
            cout << "Entrust Response: batch_no=" << sBatchNo.c_str() << " entrust_no=" << sEntrustNo.c_str() << endl;
            entrustNo = atoi(sEntrustNo.c_str());
            result->next();
        }
    }
}

void processWithdrawRsp(CResultSet* result, int& entrustNo2)
{
    if (result->hasField("entrust_no"))
    {
        string sEntrustNo2;
        result->getValueByName("entrust_no",sEntrustNo2);
        cout << "withdraw response: entrust_no = " << sEntrustNo2.c_str() << endl;
        entrustNo2 = atoi(sEntrustNo2.c_str());
    }
}

void runReportProcess()
{
    while (gbIsRun)
    {
        EnterCriticalSection(&critical_sec2_);
        list<CResultSet*> tmpReportList;
        if (!gReportList.empty())
        {
            tmpReportList.swap(gReportList);
        }
        LeaveCriticalSection(&critical_sec2_);

        //����ָ����
        list<CResultSet*>::iterator it = tmpReportList.begin();
        for (; it != tmpReportList.end(); it++)
        {
            CResultSet* reportSet = *it;
            if (reportSet->hasField("msgtype"))
            {
                string sMsgType;
                reportSet->getValueByName("msgtype",sMsgType);

                if (sMsgType == "a")
                {
                    string sEntrustNo;
                    string sEntrustStatus;
                    reportSet->getValueByName("entrust_no",sEntrustNo);
                    reportSet->getValueByName("entrust_state",sEntrustStatus);
                    cout << "ί���´�: entrust_no=" << sEntrustNo.c_str() << " entrust_state=" << sEntrustStatus.c_str() << endl;
                }
                else if (sMsgType == "b")
                {
                    string sEntrustNo;
                    string sEntrustStatus;
                    reportSet->getValueByName("entrust_no",sEntrustNo);
                    reportSet->getValueByName("entrust_state",sEntrustStatus);
                    cout << "ί��ȷ��: entrust_no=" << sEntrustNo.c_str() << " entrust_state=" << sEntrustStatus.c_str() << endl;
                }
                else if (sMsgType == "g")
                {
                    string sDealNo;
                    string sEntrustNo;
                    string sEntrustStatus;
                    string sDealAmount;
                    string sDealPrice;
                    reportSet->getValueByName("deal_no",sDealNo);
                    reportSet->getValueByName("entrust_no",sEntrustNo);
                    reportSet->getValueByName("entrust_state",sEntrustStatus);
                    reportSet->getValueByName("deal_amount",sDealAmount);
                    reportSet->getValueByName("deal_price",sDealPrice);
                    cout << "ί�гɽ�: deal_no=" << sDealNo.c_str() << " entrust_no=" << sEntrustNo.c_str() << " entrust_state=" << sEntrustStatus.c_str() << " deal_amount=" << sDealAmount.c_str() << " deal_price=" << sDealPrice.c_str() << endl;
                }
            }
            else
            {
                cout << "��Ϣȱ��msgtype�ֶ�!" << endl;
            }
        }

        Sleep(3000);
    }
}


unsigned long CCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    //δʹ��
    return 0;
}

unsigned long CCallback::AddRef()
{
    //δʹ��
    return 0;
}

unsigned long CCallback::Release()
{
    //δʹ��
    return 0;
}

void CCallback::OnConnect(CConnectionInterface *lpConnection)
{
    cout << "OnConnect: successfully connected." << endl;
}

void CCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    cout << "OnSafeConnect: successfully connected." << endl;
}

void CCallback::OnRegister(CConnectionInterface *lpConnection)
{
    cout << "OnRegister: successfully registered." << endl;
}

void CCallback::OnClose(CConnectionInterface *lpConnection)
{
    cout << "OnClose: sdk connection closed." << endl;
}

void CCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
    cout << "OnSend: hSend(" << hSend << ") send successed, queuingData(" << nQueuingData << ")." << endl;
}

void CCallback::Reserved1(void *a, void *b, void *c, void *d)
{
    //δʹ��
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
    //δʹ��
}

int  CCallback::Reserved3()
{
    //δʹ��
    return 0;
}

void CCallback::Reserved4()
{
    //δʹ��
}

void CCallback::Reserved5()
{
    //δʹ��
}

void CCallback::Reserved6()
{
    //δʹ��
}

void CCallback::Reserved7()
{
    //δʹ��
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    //δʹ��
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
    //δʹ��
}

void CCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
    if (lpMsg == NULL)
    {
        cout << "��Ϣ��ָ��Ϊ��" << endl;
        return;
    }

    cout << "CCallback::OnReceivedBizMsg: HanderID(" << hSend << "), FunctionNo(" << lpMsg->GetFunction() << "), MessageType(" << lpMsg->GetPacketType() << ")." << endl;

    int iReturnCode = lpMsg->GetReturnCode();
    if (iReturnCode == 1 || iReturnCode == -1)
    {
        cout << "errorNo:" << lpMsg->GetErrorNo() << ",errorInfo:" << lpMsg->GetErrorInfo() << endl;
    }
    else
    {
        if (iReturnCode == 0)
        {
            cout << "ҵ������ɹ�" << endl;
        }
        else
        {
            cout << "ҵ�����ʧ��" << endl;
        }

        int iLen = 0;
        const void* responseBuffer = lpMsg->GetContent(iLen);
        IF2UnPacker* responseUnPacker = NewUnPacker((void *)responseBuffer,iLen);
        //ShowPacket(responseUnPacker);

        CResultSet *resultSet = new CResultSet;
        resultSet->setHandlerID(hSend);
        resultSet->unpack(responseUnPacker);
        EnterCriticalSection(&critical_sec_);
        gResultList.push_back(resultSet);
        LeaveCriticalSection(&critical_sec_);
    }
}

unsigned long CSubCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    //δʹ��
    return 0;
}

unsigned long CSubCallback::AddRef()
{
    //δʹ��
    return 0;
}

unsigned long CSubCallback::Release()
{
    //δʹ��
    return 0;
}

void CSubCallback::OnReceived(CSubscribeInterface *lpSub,int subscribeIndex, const void *lpData, int nLength,LPSUBSCRIBE_RECVDATA lpRecvData)
{
    if (lpData == NULL)
    {
        cout << "��������ָ��Ϊ��" << endl;
        return;
    }

    IF2UnPacker* lpUnPack = NewUnPacker((void*)lpData,nLength);
    cout << "CSubCallback::OnReceived: SubscribeIndex(" << subscribeIndex << "), MsgType(" << lpUnPack->GetStr("msgtype") << "), DataLength(" << nLength << ")." << endl;
    //ShowPacket(lpUnPack);

    if (lpRecvData == NULL)
    {
        cout << "��������ָ��Ϊ��" << endl;
    }
    else
    {
        ShowSubscribe(subscribeIndex,lpRecvData);
    }

    CResultSet *reportSet = new CResultSet;
    reportSet->unpack(lpUnPack);
    EnterCriticalSection(&critical_sec2_);
    gReportList.push_back(reportSet);
    LeaveCriticalSection(&critical_sec2_);
}

void CSubCallback::OnRecvTickMsg(CSubscribeInterface *lpSub,int subscribeIndex,const char* TickMsgInfo)
{
}
