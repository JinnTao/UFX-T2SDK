#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <map>
#include <csignal>
#include <atomic>
#include "qmetatype.h"
using int32 = int32_t;
using int64 = int64_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using std::string;

// Put this in the declarations for a class to be uncopyable.
#define DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete
// Put this in the declarations for a class to be unassignable.
#define DISALLOW_ASSIGN(TypeName) TypeName& operator=(const TypeName&) = delete
// Put this in the declarations for a class to be uncopyable and unassignable.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    DISALLOW_COPY(TypeName);               \
    DISALLOW_ASSIGN(TypeName)
// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
// This is especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
    TypeName() = delete;                         \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

char* strtok_t(char* instr, char* delimit, char** saveptr);
char* hs_strncpy(char* dest, const char* src, size_t size);

namespace global{
    extern std::atomic<bool> need_reconnect;
}
struct sTradingAccountInfo {
    char   BrokerID[32];
    char   AccountID[32];
    double PreBalance;
    double Balance;
    double Available;
    double WithdrawQuota;
    double CurrMargin;
    double CloseProfit;
    double PositionProfit;
    double Commission;
    double FrozenMargin;
    string updateTime;
};
struct o32_fundasset{
    
    char account_code[32];//账户编号
    char currency_code[3];

    double total_asset;
    double nav;//T-1日基金单位净值
    double yesterday_nav;//昨日单位净值
    double current_balance;//当前余额
    double begin_balance;//期初余额
    double futu_deposit_balance;//期货保证金账户余额
    double occupy_deposit_balance;//占用保证金
    double futu_asset;//期货资产（合约价值）?
    double stock_asset;
    double bond_asset;
    double fund_asset;
    double repo_asset;
    double other_asset;
    double fund_share;//基金总份额
    double fund_net_asset;//基金净资产
    double payable_balance;//融入方应付金额
    double receivable_balance;//应收款

    o32_fundasset() { memset(this, 0, sizeof(o32_fundasset)); }
    void reset() { memset(this, 0, sizeof(o32_fundasset)); }

};

struct ctpConfig {
    string brokerId;
    string userId;
    string passwd;
    string mdAddress;
    string tdAddress;
    string md_flow_path;
    string td_flow_path;
    ctpConfig() { memset(this, 0, sizeof(ctpConfig)); }
    void reset() { memset(this, 0, sizeof(ctpConfig)); }
};

struct o32Config {
    string server_address;
    string license_file;
    string login_name;
    string fund_account;
    string passwd;
    string authorization_id;
    string asset_no;
    o32Config() { memset(this, 0, sizeof(o32Config)); }
    void reset() { memset(this, 0, sizeof(o32Config)); }
};


struct ErrorInfo
{
public:
    ErrorInfo() :ErrorCode(0), ErrorMsg(""){}
    int    ErrorCode;
    string ErrorMsg;
};

#ifdef _RELEASE
#define RISK_LOG(...) std::cout << "[" << __FILE__ << " line #" << __LINE__ <<  "]: "<< __VA_ARGS__ << std::endl;
#else
#define RISK_LOG(...) std::cout << __VA_ARGS__ << std::endl;
#endif



#define	 UDP_DELIMITE_STR  "\1"
#define	 UDP_DELIMITE_CHAR  '\1'

inline void ParseValue(double &val, int size, char* str)
{
    val = atof(str);
}

inline void ParseValue(char& val, int size, char* str)
{
    val = str[0];
}

inline void ParseValue(int& val, int size, char* str)
{
    val = atoi(str);
}

inline void ParseValue(float& val, int size, char* str)
{
    val = atof(str);
}

inline void ParseValue(char* val, int size, char* str)
{
    hs_strncpy(val, str, size);
}

inline void ParseValue(unsigned int& val, int size, char* str)
{
    val = 0;
    for (int i = 0; str[i] != 0; i++)
    {
        if ('0' <= str[i] && str[i] <= '9')
        {
            val = val * 10 + str[i] - '0';
        }
        else
        {
            val = 0;
            break;
        }
    }
}

#define PARSE_DATASET(outType,outdataPtr,inType,inName)  outType *__dataptr = outdataPtr; \
    ParseValue(__dataptr->inName,sizeof(__dataptr->inName), inName)

#define BEGINPPARSE(type,outdataPtr,indataStrptr, delim) bool __packValid = true; \
    do{   type* __dataptr = outdataPtr; \
	char* __delim = delim; \
	char* __toksaveptr; \
	char* __token;\
	__token = strtok_t(indataStrptr, __delim, &__toksaveptr);

#define PARSEVALUE(val) if( __token != NULL ) \
{	\
	ParseValue(__dataptr->val,sizeof(__dataptr->val), __token); \
	__token = strtok_t(NULL, __delim, &__toksaveptr);				\
}else {  __packValid = false; break; }

#define INGOREVALUE(val) if( __token != NULL ) \
{	\
	__token = strtok_t(NULL, __delim, &__toksaveptr); \
}else {  __packValid = false; break; }

#define ENDPARSE }while(0);

#define ISPACKValid  __packValid


using o32_account_info_map = std::map<std::string, std::string>;

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(o32_account_info_map)
Q_DECLARE_METATYPE(sTradingAccountInfo)

#endif


