/*******************************************************************************************************/
/*
/*    1、此C++ Demo使用异步发送接收模式；
/*    2、Demo连接的是恒生仿真测试环境，如要对接券商环境，需要修改t2sdk.ini文件中的IP和许可证文件等；
/*    3、不同环境的验证信息可能不一样，更换连接环境时，包头字段设置需要确认；
/*    4、接口字段说明请参考接口文档"恒生统一接入平台_周边接口规范(股票期权).xls"；
/*    5、T2函数技术说明参考开发文档“T2SDK 外部版开发指南.docx"；
/*    6、如有UFX接口技术疑问可联系大金融讨论组（261969915）；
/*    7、SecuTrade.h和SecuTrade.cpp用来测试普通交易；
/*    8、SecuMD.h和SecuMD.cpp 用来测试订阅成交回报主推委托主推；
/*    9、UFX技术支持网站 https://ufx.hscloud.cn/；
/*    10、demo仅供参考。
/*
/*******************************************************************************************************/

#include "SecuTrade.h"
#include "SecuMD.h"

int showmenu1()
{
   cerr<<"------------------------------------------------"<<endl;
   cerr<<" [1]  331100 请求登录                           "<<endl;
   cerr<<" [2]  400 证券行情查询                          "<<endl;
   cerr<<" [3]  333002 证券普通委托                       "<<endl;
   cerr<<" [4]  333103 证券持仓查询                       "<<endl;
   cerr<<" [5]  331300 证券股东信息查询                   "<<endl;
   cerr<<" [6]  333101 证券委托查询                       "<<endl;
   cerr<<" [7]  333102 证券成交查询                       "<<endl;
   cerr<<" [0]  EXIT                                      "<<endl;
   cerr<<"------------------------------------------------"<<endl;
   return 0;

}
int showmenu2()
{
   cerr<<"------------------------------------------------"<<endl;
   cerr<<" [1]  331100 请求登录                           "<<endl;
   cerr<<" [2]  62001_12订阅证券成交回报                  "<<endl;
   cerr<<" [3]  62001_23订阅证券委托回报                   "<<endl;
   cerr<<" [4]  333002 普通委托                            "<<endl;
   cerr<<" [0]  EXIT                                      "<<endl;
   cerr<<"------------------------------------------------"<<endl;
   return 0;
}

int TestSecuTrade()
{ 
	char exchange_type[4];
	char stock_code[16];
	double entrust_price=0.0;
	memset(exchange_type,0,sizeof(exchange_type));
	memset(stock_code,0,sizeof(stock_code));
	double entrust_amount=0.0;
	char entrust_bs='\0';
   int chose=0;
   SecuRequestMode* lp_SecuRequestMode = new SecuRequestMode();
   lp_SecuRequestMode->InitConn();
  showmenu1();
   while(1)
   {
   cin>>chose;
   switch(chose)
   {
   case 1:   
	  lp_SecuRequestMode->ReqFunction331100();
		break;
   case 2:
	   cout<<"---------证券行情查询----------"<<endl;
	   cout<<"Enter exchange_type:";cin>>exchange_type;
	   cout<<"Enter stock_code:";cin>>stock_code;
	   lp_SecuRequestMode->ReqFunction400(exchange_type,stock_code);
		break;
   case 3:
	   cout<<"---------证券普通委托----------"<<endl;
	   cout<<"exchange_type:"; cin>>exchange_type;
	   cout<<"stock_code:"; cin>>stock_code;
	   cout<<"entrust_amount:"; cin>>entrust_amount;
	   cout<<"entrust_price:";  cin>>entrust_price;
	   cout<<"entrust_bs:";cin>>entrust_bs;
	   lp_SecuRequestMode->ReqFunction333002(exchange_type, stock_code, entrust_amount, entrust_price,entrust_bs);
	   break;
   case 4:
	   lp_SecuRequestMode->ReqFunction333104(" ");
	   break;
   case 5:
	     lp_SecuRequestMode->ReqFunction331300();
		 break;
   case 6: 
	   lp_SecuRequestMode->ReqFunction333101();
	   break;
   case 7:
	   lp_SecuRequestMode->ReqFunction333102();
	   break;
   case 0:
	    exit(0); 
  }
   Sleep(500);
   system("pause");
   showmenu1();
 }
    
   delete lp_SecuRequestMode;
   return 0;
}

int TestSecutMd()
{   
	char exchange_type[4];
	char stock_code[16];
	double entrust_price=0.0;
	memset(exchange_type,0,sizeof(exchange_type));
	memset(stock_code,0,sizeof(stock_code));
	double entrust_amount=0.0;
	char entrust_bs='\0';
	int chose=0;
	int i=0;
   CMdRequestMode* lp_CMdRequestMode = new CMdRequestMode();
   lp_CMdRequestMode->InitConn();
   showmenu2();
   while(1)
   {
   cin>>chose;
   switch(chose)
   {
   case 1:
	   lp_CMdRequestMode->ReqFunction331100();
   break;
   case 2: 
	   lp_CMdRequestMode->SubFunction12(ISSUE_TYPE_REALTIME_SECU);
	   break;
   case 3:
	   lp_CMdRequestMode->SunFunction23(ISSUE_TYPE_ENTR_BACK);
	   break;
   case 4:
	   cout<<"---------证券普通委托----------"<<endl;
	    lp_CMdRequestMode->ReqFunction333002();
	   break;
   case 0:
	   exit(0);
   }
   Sleep(500);
   system("pause");
   showmenu2();
  }
    delete lp_CMdRequestMode;
   return 0;
}
int main()
{

    cerr<<"-----------------------------------------------"<<endl;
	cerr<<" [1] 证券交易接口测试                          "<<endl;
	cerr<<" [2] 证券主推接口测试                          "<<endl;
	cerr<<" [0] 退出                                      "<<endl;
	cerr<<"-----------------------------------------------"<<endl;
	
	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
			case 1: 
				TestSecuTrade();
				break;
			case 2:
				TestSecutMd();
				break;
			case 0: exit(0);
		}
	}

	//通过getchar阻塞线程，等待服务端应答包到达
    getchar();

	return 0;
}
