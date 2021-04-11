class spCustomerInfo
{
// [TODO] cps_limit does not look solid enough 
// [TODO] check that uuid is valid on Provisioning API

public:
    spCustomerInfo(const string& body, shared_ptr<DbConnection> dbc);

    string __insert(void);
    string __update(void);
    string __delete(void);

    string                              sp_customer_uuid;
    bool                                active;
    chrono::system_clock::time_point    started_on;
    chrono::system_clock::time_point    modified_on;
    chrono::system_clock::time_point    expired_on;
    int32_t                             cps_limit;
    bool                                as_enabled;
    bool                                vs_enabled;
    string                              sp_customer_name;

private:
    string __crtIUS(string eqs, bool with_uuid);
    void   __rmpIUS(pqxx::result r);
    string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_customer_uuid) throw (AsVsException("Nothing to update since 'sp_customer_uuid' parameter is missing.")); }

    shared_ptr<DbConnection> d;

    bool                                has_sp_customer_uuid;
    bool                                has_active;
    bool                                has_started_on;
    bool                                has_modified_on;
    bool                                has_expired_on;
    bool                                has_cps_limit;
    bool                                has_as_enabled;
    bool                                has_vs_enabled;
    bool                                has_sp_customer_name;

};

class spCustomerIpInfo
{
public:
    spCustomerIpInfo(const string& body, shared_ptr<DbConnection> dbc);

    string __insert(void);
    string __update(void);
    string __delete(void);

    string                              sp_customer_ip_uuid;
    string                              sp_customer_uuid;
    string                              customer_ip;
    string                              tech_prefix;

//    ATT RFC5213  https ://tools.ietf.org/html/rfc5213#page-74 
//    Access Technology Type Option
    string                              att_level_with_sp_ani;                   // CHARACTER(1)
    string                              att_level_wo_sp_ani;                     // CHARACTER(1)

private:
    string __crtIUS(string eqs, bool with_uuid);
    void   __rmpIUS(pqxx::result r);
    string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_customer_ip_uuid) throw (AsVsException("Nothing to update since 'sp_customer_ip_uuid' parameter is missing.")); }

    shared_ptr<DbConnection> d;
    bool                               has_sp_customer_ip_uuid;
    bool                               has_sp_customer_uuid;
    bool                               has_customer_ip;
    bool                               has_tech_prefix;
    bool                               has_att_level_with_sp_ani;
    bool                               has_att_level_wo_sp_ani;
};

