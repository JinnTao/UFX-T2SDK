#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdint>
#include <iostream>
#include <string>

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
};
struct o32_fundasset{
    
    string account_code;//�˻����
    string currency_code;

    double total_asset;
    double nav;//T-1�ջ���λ��ֵ
    double yesterday_nav;//���յ�λ��ֵ
    double current_balance;//��ǰ���
    double begin_balance;//�ڳ����
    double futu_deposit_balance;//�ڻ���֤���˻����
    double occupy_deposit_balance;//ռ�ñ�֤��
    double futu_asset;//�ڻ��ʲ�����Լ��ֵ��?
    double stock_asset;
    double bond_asset;
    double fund_asset;
    double repo_asset;
    double other_asset;
    double fund_share;//�����ܷݶ�
    double fund_net_asset;//�����ʲ�
    double payable_balance;//���뷽Ӧ�����
    double receivable_balance;//Ӧ�տ�

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


char* hs_strncpy(char* dest, const char* src, size_t size)
{
    if (dest == NULL)
        return 0;
    if (src == NULL)
    {
        dest[0] = 0;
        return 0;
    }
    char* ret = strncpy(dest, src, size - 1);
    dest[size - 1] = 0;
    return ret;
}


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















#endif  // _COMMON_H_
