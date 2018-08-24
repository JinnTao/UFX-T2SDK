#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdint>

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
    o32Config() { memset(this, 0, sizeof(o32Config)); }
    void reset() { memset(this, 0, sizeof(o32Config)); }
};


#define RISK_LOG(...) std::cout << "[" << __FILE__ << " line #" << __LINE__ <<  "]: "<< __VA_ARGS__ << std::endl;

#endif  // _COMMON_H_
