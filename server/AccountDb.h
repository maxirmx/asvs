class spAccountInfo
{
public:
    spAccountInfo(const string& body, shared_ptr<DbConnection> dbc);

    string __insert(void);
    string __update(void);
    string __delete(void);

    string                              sp_account_uuid;
    bool                                active;
    chrono::system_clock::time_point    started_on;
    chrono::system_clock::time_point    modified_on;
    chrono::system_clock::time_point    expired_on;
    int32_t                             cps_limit;
    bool                                as_enabled;
    bool                                vs_enabled;
//    ATT RFC5213  https ://tools.ietf.org/html/rfc5213#page-74 
//    Access Technology Type Option
    string                              default_att_level_with_sp_ani;              // CHARACTER(1)
    string                              default_att_level_wo_sp_ani;                // CHARACTER(1)
    string                              sp_name;

private:
    string __crtIUS(string eqs, bool with_uuid);
    void   __rmpIUS(pqxx::result r);
    string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_account_uuid) throw (AsVsException("Nothing to delete since 'sp_account_uuid' parameter is missing.")); }

    shared_ptr<DbConnection> d;

    bool                               has_sp_account_uuid;
    bool                               has_active;
    bool                               has_started_on;
    bool                               has_modified_on;
    bool                               has_expired_on;
    bool                               has_cps_limit;
    bool                               has_as_enabled;
    bool                               has_vs_enabled;
    bool                               has_default_att_level_with_sp_ani;
    bool                               has_default_att_level_wo_sp_ani;
    bool                               has_sp_name;
};

class spGatewayIpInfo
{
public:
    spGatewayIpInfo(const string& body, shared_ptr<DbConnection> dbc);

    string __insert(void);
    string __update(void);
    string __delete(void);

    string                             sp_account_uuid;
    string                             sp_gateway_ip_uuid;
    string                             gateway_ip;

private:
    string __crtIUS(string eqs, bool with_uuid);
    void   __rmpIUS(pqxx::result r);
    string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_gateway_ip_uuid) throw (AsVsException("Nothing to update since 'sp_gateway_ip_uuid' parameter is missing.")); }

    shared_ptr<DbConnection> d;

    bool                               has_sp_account_uuid;
    bool                               has_sp_gateway_ip_uuid;
    bool                               has_gateway_ip;

};

class spTnInfo
{
public:
    spTnInfo(const string& body, shared_ptr<DbConnection> dbc);

    string __insert(void);
    string __update(void);
    string __delete(void);

    string                             sp_account_uuid;
    string                             sp_tn_uuid;
    string                             tn;

private:
    string __crtIUS(string eqs, bool with_uuid);
    void   __rmpIUS(pqxx::result r);
    string __srsIUS(void);
    void   __chkUUID(void)  { if (!has_sp_tn_uuid) throw (AsVsException("Nothing to update since 'sp_tn_uuid' parameter is missing.")); }

    shared_ptr<DbConnection> d;

    bool                               has_sp_account_uuid;
    bool                               has_sp_tn_uuid;
    bool                               has_tn;

};

class spCertInfo
{
public:
    spCertInfo(const string& body, shared_ptr<DbConnection> dbc);

    string __insert(void);
    string __update(void);
    string __delete(void);

    string                             sp_account_uuid;
    string                             sp_cert_uuid;
    string                             priv_key;
    string                             pem_file_path;

private:
    string __crtIUS(string eqs, bool with_uuid);
    void   __rmpIUS(pqxx::result r);
    string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_cert_uuid) throw (AsVsException("Nothing to update since 'sp_cert_uuid' parameter is missing.")); }

    shared_ptr<DbConnection> d;

    bool                               has_sp_account_uuid;
    bool                               has_sp_cert_uuid;
    bool                               has_priv_key;
    bool                               has_pem_file_path;
};

