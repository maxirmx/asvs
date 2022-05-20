#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

// --------------------------------------------------------------------------------------------------------------------------

// [TODO] cps_limit does not look solid enough
// [TODO] check that uuid is valid on Provisioning API

void spCustomerInfo::__proto(void)
{
    proto_fields_pt =
    {
        {"sp_customer_uuid",    &spBaseObject::__crt_pt<spStringColumn,  std::string>},
        {"active",              &spBaseObject::__crt_pt<spBoolColumn,    bool>       },
        {"as_started_on",       &spBaseObject::__crt_pt<spTimeColumn,    std::string>},
        {"vs_started_on",       &spBaseObject::__crt_pt<spTimeColumn,    std::string>},
        {"modified_on",         &spBaseObject::__crt_pt<spTimeColumn,    std::string>},
        {"as_expired_on",       &spBaseObject::__crt_pt<spTimeColumn,    std::string>},
        {"vs_expired_on",       &spBaseObject::__crt_pt<spTimeColumn,    std::string>},
        {"as_cps_limit",        &spBaseObject::__crt_pt<spInt32Column,   int32_t>    },
        {"vs_cps_limit",        &spBaseObject::__crt_pt<spInt32Column,   int32_t>    },
        {"as_enabled",          &spBaseObject::__crt_pt<spBoolColumn,    bool>       },
        {"vs_enabled",          &spBaseObject::__crt_pt<spBoolColumn,    bool>       },
        {"sp_customer_name",    &spBaseObject::__crt_pt<spStringColumn,  std::string>}
    };

    proto_fields_pq =
    {
        {"sp_customer_uuid",    &spBaseObject::__crt_pq<spStringColumn,  std::string>},
        {"active",              &spBaseObject::__crt_pq<spBoolColumn,    bool>       },
        {"as_started_on",       &spBaseObject::__crt_pq<spTimeColumn,    std::string>},
        {"vs_started_on",       &spBaseObject::__crt_pq<spTimeColumn,    std::string>},
        {"modified_on",         &spBaseObject::__crt_pq<spTimeColumn,    std::string>},
        {"as_expired_on",       &spBaseObject::__crt_pq<spTimeColumn,    std::string>},
        {"vs_expired_on",       &spBaseObject::__crt_pq<spTimeColumn,    std::string>},
        {"as_cps_limit",        &spBaseObject::__crt_pq<spInt32Column,   int32_t>    },
        {"vs_cps_limit",        &spBaseObject::__crt_pq<spInt32Column,   int32_t>    },
        {"as_enabled",          &spBaseObject::__crt_pq<spBoolColumn,    bool>       },
        {"vs_enabled",          &spBaseObject::__crt_pq<spBoolColumn,    bool>       },
        {"sp_customer_name",    &spBaseObject::__crt_pq<spStringColumn,  std::string>}
    };

}

spCustomerInfo::spCustomerInfo(const string& b): spBaseObject("sp_customers", "sp_customer_uuid")
{
    __proto();
    __rmpINT(b);
}

spCustomerInfo::spCustomerInfo(const pqxx::row& r): spBaseObject("sp_customers", "sp_customer_uuid")
{
    __proto();
    __rmpIUS(r);
}

// --------------------------------------------------------------------------------------------------------------------------

void spCustomerIpInfo::__proto(void)
{
    //    ATT RFC5213  https ://tools.ietf.org/html/rfc5213#page-74
    //    Access Technology Type Option ?


    proto_fields_pt =
    {
        {"sp_customer_ip_uuid",     &spBaseObject::__crt_pt <spStringColumn, string> },
        {"sp_customer_uuid",        &spBaseObject::__crt_pt <spStringColumn, string> },
        {"customer_ip",             &spBaseObject::__crt_pt <spStringColumn, string> },
        {"modified_on",             &spBaseObject::__crt_pt <spTimeColumn,   string> },
        {"orig_id",                 &spBaseObject::__crt_pt <spStringColumn, string> },
        {"tech_prefix",             &spBaseObject::__crt_pt <spStringColumn, string> },
        {"att_level_with_sp_ani",   &spBaseObject::__crt_pt <spStringColumn, string> },
        {"att_level_wo_sp_ani",     &spBaseObject::__crt_pt <spStringColumn, string> }
    };

    proto_fields_pq =
    {
        {"sp_customer_ip_uuid",     &spBaseObject::__crt_pq <spStringColumn, string> },
        {"sp_customer_uuid",        &spBaseObject::__crt_pq <spStringColumn, string> },
        {"modified_on",             &spBaseObject::__crt_pq <spTimeColumn,   string> },
        {"customer_ip",             &spBaseObject::__crt_pq <spStringColumn, string> },
        {"orig_id",                 &spBaseObject::__crt_pq <spStringColumn, string> },
        {"tech_prefix",             &spBaseObject::__crt_pq <spStringColumn, string> },
        {"att_level_with_sp_ani",   &spBaseObject::__crt_pq <spStringColumn, string> },
        {"att_level_wo_sp_ani",     &spBaseObject::__crt_pq <spStringColumn, string> }
    };
}

spCustomerIpInfo::spCustomerIpInfo(const string& b) : spBaseObject("sp_customer_ip", "sp_customer_ip_uuid")
{
    __proto();
    __rmpINT(b);
}

spCustomerIpInfo::spCustomerIpInfo(const pqxx::row& r) : spBaseObject("sp_customer_ip", "sp_customer_ip_uuid")
{
    __proto();
    __rmpIUS(r);
}

// --------------------------------------------------------------------------------------------------------------------------

void spAccountInfo::__proto(void)
{
    proto_fields_pt =
    {
        {"sp_account_uuid",                 &spBaseObject::__crt_pt <spStringColumn, string> },
        {"active",                          &spBaseObject::__crt_pt <spBoolColumn,  bool>    },
        {"started_on",                      &spBaseObject::__crt_pt <spTimeColumn,  string>  },
        {"modified_on",                     &spBaseObject::__crt_pt <spTimeColumn,  string>  },
        {"expired_on",                      &spBaseObject::__crt_pt <spTimeColumn,  string>  },
        {"cps_limit",                       &spBaseObject::__crt_pt <spInt32Column, int32_t> },
        {"as_enabled",                      &spBaseObject::__crt_pt <spBoolColumn,  bool>    },
        {"vs_enabled",                      &spBaseObject::__crt_pt <spBoolColumn,  bool>    },
        {"sp_name",                         &spBaseObject::__crt_pt <spStringColumn, string> },
        {"default_att_level_with_sp_ani",   &spBaseObject::__crt_pt <spStringColumn, string> },
        {"default_att_level_wo_sp_ani",     &spBaseObject::__crt_pt <spStringColumn, string> }
    };

    proto_fields_pq =
    {
        {"sp_account_uuid",                 &spBaseObject::__crt_pq <spStringColumn, string> },
        {"active",                          &spBaseObject::__crt_pq <spBoolColumn,   bool>   },
        {"started_on",                      &spBaseObject::__crt_pq <spTimeColumn,   string> },
        {"modified_on",                     &spBaseObject::__crt_pq <spTimeColumn,   string> },
        {"expired_on",                      &spBaseObject::__crt_pq <spTimeColumn,   string> },
        {"cps_limit",                       &spBaseObject::__crt_pq <spInt32Column,  int32_t>},
        {"as_enabled",                      &spBaseObject::__crt_pq <spBoolColumn,   bool>   },
        {"vs_enabled",                      &spBaseObject::__crt_pq <spBoolColumn,   bool>   },
        {"sp_name",                         &spBaseObject::__crt_pq <spStringColumn, string> },
        {"default_att_level_with_sp_ani",   &spBaseObject::__crt_pq <spStringColumn, string> },
        {"default_att_level_wo_sp_ani",     &spBaseObject::__crt_pq <spStringColumn, string> }
    };
}

spAccountInfo::spAccountInfo(const string& b) : spBaseObject("sp_accounts", "sp_account_uuid")
{
    __proto();
    __rmpINT(b);
}

spAccountInfo::spAccountInfo(const pqxx::row& r) : spBaseObject("sp_accounts", "sp_account_uuid")
{
    __proto();
    __rmpIUS(r);
}

// --------------------------------------------------------------------------------------------------------------------------

void spGatewayIpInfo::__proto(void)
{
    proto_fields_pt =
    {
        {"sp_gateway_ip_uuid",              &spBaseObject::__crt_pt <spStringColumn, string> },
        {"sp_account_uuid",                 &spBaseObject::__crt_pt <spStringColumn, string> },
        {"gateway_ip",                      &spBaseObject::__crt_pt <spStringColumn, string> }
    };

    proto_fields_pq =
    {
        {"sp_gateway_ip_uuid",              &spBaseObject::__crt_pq <spStringColumn, string> },
        {"sp_account_uuid",                 &spBaseObject::__crt_pq <spStringColumn, string> },
        {"gateway_ip",                      &spBaseObject::__crt_pq <spStringColumn, string> }
    };

}

spGatewayIpInfo::spGatewayIpInfo(const string& b): spBaseObject("sp_gateway_ip", "sp_gateway_ip_uuid")
{
    __proto();
    __rmpINT(b);
}

spGatewayIpInfo::spGatewayIpInfo(const pqxx::row& r): spBaseObject("sp_gateway_ip", "sp_gateway_ip_uuid")
{
    __proto();
    __rmpIUS(r);
}

// --------------------------------------------------------------------------------------------------------------------------

void spTnInfo::__proto(void)
{
    proto_fields_pt =
    {
         {"sp_tn_uuid",                   &spBaseObject::__crt_pt <spStringColumn, string> },
         {"sp_account_uuid",              &spBaseObject::__crt_pt <spStringColumn, string> },
         {"tn",                           &spBaseObject::__crt_pt <spStringColumn, string> },
         {"modified_on",                  &spBaseObject::__crt_pt <spTimeColumn,   string> }
    };

    proto_fields_pq =
    {
         {"sp_tn_uuid",                   &spBaseObject::__crt_pq <spStringColumn, string> },
         {"sp_account_uuid",              &spBaseObject::__crt_pq <spStringColumn, string> },
         {"tn",                           &spBaseObject::__crt_pq <spStringColumn, string> },
         {"modified_on",                  &spBaseObject::__crt_pq <spTimeColumn,   string> }
    };

}

spTnInfo::spTnInfo(const string& b): spBaseObject("sp_tn", "sp_tn_uuid")
{
    __proto();
    __rmpINT(b);
}

spTnInfo::spTnInfo(const pqxx::row& r): spBaseObject("sp_tn", "sp_tn_uuid")
{
   __proto();
   __rmpIUS(r);
}

// --------------------------------------------------------------------------------------------------------------------------

void spCertInfo::__proto(void)
{
    proto_fields_pt =
    {
        {"sp_cert_uuid",    &spBaseObject::__crt_pt <spStringColumn, string> },
        {"sp_account_uuid", &spBaseObject::__crt_pt <spStringColumn, string> },
        {"is_default",      &spBaseObject::__crt_pt <spBoolColumn,   bool>   },
        {"priv_key",        &spBaseObject::__crt_pt <spStringColumn, string> },
        {"pem",             &spBaseObject::__crt_pt <spStringColumn, string> },
        {"modified_on",     &spBaseObject::__crt_pt <spTimeColumn,   string> }
    };

    proto_fields_pq =
    {
        {"sp_cert_uuid",    &spBaseObject::__crt_pq <spStringColumn, string> },
        {"sp_account_uuid", &spBaseObject::__crt_pq <spStringColumn, string> },
        {"is_default",      &spBaseObject::__crt_pq <spBoolColumn,   bool>   },
        {"priv_key",        &spBaseObject::__crt_pq <spStringColumn, string> },
        {"pem",             &spBaseObject::__crt_pq <spStringColumn, string> },
        {"modified_on",     &spBaseObject::__crt_pq <spTimeColumn,   string> }
    };
}


spCertInfo::spCertInfo(const string& b): spBaseObject("sp_cert", "sp_cert_uuid")
{
    __proto();
    __rmpINT(b);
}

spCertInfo::spCertInfo(const pqxx::row& r): spBaseObject("sp_cert", "sp_cert_uuid")
{
    __proto();
    __rmpIUS(r);
}


