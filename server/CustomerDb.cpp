#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

#include "Util.h"
#include "CustomerDb.h"


spCustomerInfo::spCustomerInfo(const string& body, shared_ptr<DbConnection> dbc):
                                d(dbc),
                                has_sp_customer_uuid (false),
                                has_active(false), 
                                has_started_on(false),
                                has_modified_on(false),
                                has_expired_on(false),
                                has_cps_limit(false), 
                                has_as_enabled(false),
                                has_vs_enabled(false), 
                                has_sp_customer_name(false)

{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_customer_uuid  = pt.get_optional<string>   ("sp_customer_uuid");
    auto _active            = pt.get_optional<bool>     ("active");
    auto _started_on        = pt.get_optional<string>   ("started_on");
    auto _modified_on       = pt.get_optional<string>   ("modified_on");
    auto _expired_on        = pt.get_optional<string>   ("expired_on");
    auto _cps_limit         = pt.get_optional<int32_t>  ("cps_limit");
    auto _as_enabled        = pt.get_optional<bool>     ("as_enabled");
    auto _vs_enabled        = pt.get_optional<bool>     ("vs_enabled");
    auto _sp_customer_name  = pt.get_optional<string>   ("sp_customer_name");

    if (_sp_customer_uuid)  { has_sp_customer_uuid =    true;  sp_customer_uuid =  *_sp_customer_uuid;  }
    if (_active)            { has_active =              true;  active =            *_active;            }
    if (_started_on)        { has_started_on =          true;  started_on =    str2time(*_started_on);  }
    if (_modified_on)       { has_modified_on =         true;  modified_on =   str2time (*_modified_on);}
    if (_expired_on)        { has_expired_on =          true;  expired_on =    str2time(*_expired_on);  }
    if (_cps_limit)         { has_cps_limit =           true;  cps_limit =         *_cps_limit;         }
    if (_as_enabled)        { has_as_enabled =          true;  as_enabled =        *_as_enabled;        }
    if (_vs_enabled)        { has_vs_enabled =          true;  vs_enabled =        *_vs_enabled;        }
    if (_sp_customer_name)  { has_sp_customer_name =    true;  sp_customer_name =  *_sp_customer_name;  }
       
}

string spCustomerInfo::__insert(void)
{
    has_modified_on = true; 
    modified_on = chrono::system_clock::now();

    string statement = "INSERT INTO sp_customers " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCustomerInfo::__update(void)
{
    __chkUUID();

    has_modified_on = true;
    modified_on = chrono::system_clock::now();
    
    string statement = "UPDATE sp_customers SET " + __crtIUS(" = ", false) + "WHERE sp_customer_uuid = " + d->c->quote(sp_customer_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCustomerInfo::__delete(void)
{
    __chkUUID();

    string statement = "DELETE FROM sp_customers WHERE sp_customer_uuid = " + d->c->quote(sp_customer_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}


string spCustomerInfo::__crtIUS(string eqs, bool with_uuid)
{
    stringstream prefix; 
    stringstream suffix; 
    string delim = "(";

    if (with_uuid) 
    {
       if (has_sp_customer_uuid){ prefix << delim << "sp_customer_uuid";    suffix << delim << d->c->quote(sp_customer_uuid);        delim = ","; }
    }
    if (has_active)             { prefix << delim << "active";              suffix << delim << d->c->quote(active);                  delim = ","; }
    if (has_started_on)         { prefix << delim << "started_on";          suffix << delim << d->c->quote(time2str(started_on));    delim = ","; }
    if (has_expired_on)         { prefix << delim << "expired_on";          suffix << delim << d->c->quote(time2str(expired_on));    delim = ","; }
    if (has_modified_on)        { prefix << delim << "modified_on";         suffix << delim << d->c->quote(time2str(modified_on));   delim = ","; }
    if (has_cps_limit)          { prefix << delim << "cps_limit";           suffix << delim << d->c->quote(cps_limit);               delim = ","; }
    if (has_as_enabled)         { prefix << delim << "as_enabled";          suffix << delim << d->c->quote(as_enabled);              delim = ","; }
    if (has_vs_enabled)         { prefix << delim << "vs_enabled";          suffix << delim << d->c->quote(vs_enabled);              delim = ","; }
    if (has_sp_customer_name)   { prefix << delim << "sp_customer_name";    suffix << delim << d->c->quote(sp_customer_name);        delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();

}

void   spCustomerInfo::__rmpIUS(pqxx::result r)
{
    has_sp_customer_uuid = true;sp_customer_uuid =  r[0]["sp_customer_uuid"].as<string>();
    has_active = true;          active =            r[0]["active"].as<bool>();
    has_started_on = true;      started_on =        str2time(r[0]["started_on"].as<string>());
    has_modified_on = true;     modified_on =       str2time(r[0]["modified_on"].as<string>());
    has_expired_on = true;      expired_on =        str2time(r[0]["expired_on"].as<string>());
    has_cps_limit = true;       cps_limit =         r[0]["cps_limit"].as<int>();
    has_as_enabled = true;      as_enabled =        r[0]["as_enabled"].as<bool>();
    has_vs_enabled = true;      vs_enabled =        r[0]["vs_enabled"].as<bool>();
    has_sp_customer_name = true;sp_customer_name =  r[0]["sp_customer_name"].as<string>();

}

string   spCustomerInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_customer_uuid)       r.put("sp_customer_uuid",   sp_customer_uuid);
    if (has_active)                 r.put("active",             active);
    if (has_started_on)             r.put("started_on",         time2str(started_on));
    if (has_expired_on)             r.put("expired_on",         time2str(expired_on));
    if (has_modified_on)            r.put("modified_on",        time2str(modified_on));
    if (has_cps_limit)              r.put("cps_limit",          cps_limit);
    if (has_as_enabled)             r.put("as_enabled",         as_enabled);
    if (has_vs_enabled)             r.put("vs_enabled",         vs_enabled);
    if (has_sp_customer_name)       r.put("sp_customer_name",   sp_customer_name);
    
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}

// --------------------------------------------------------------------------------------------------------------------------

spCustomerIpInfo::spCustomerIpInfo(const string& body, shared_ptr<DbConnection> dbc) :
                               d(dbc),
                               has_sp_customer_ip_uuid(false),
                               has_sp_customer_uuid(false),
                               has_customer_ip(false),
                               has_tech_prefix(false),
                               has_att_level_with_sp_ani(false),
                               has_att_level_wo_sp_ani(false)

{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_customer_ip_uuid   = pt.get_optional<string>   ("sp_customer_ip_uuid");
    auto _sp_customer_uuid      = pt.get_optional<string>   ("sp_customer_uuid");
    auto _customer_ip           = pt.get_optional<string>   ("customer_ip");
    auto _tech_prefix           = pt.get_optional<string>   ("tech_prefix");
    auto _att_level_with_sp_ani = pt.get_optional<string>   ("att_level_with_sp_ani");
    auto _att_level_wo_sp_ani   = pt.get_optional<string>   ("att_level_wo_sp_ani");

    if (_sp_customer_ip_uuid)   { has_sp_customer_ip_uuid =     true;  sp_customer_ip_uuid =    *_sp_customer_ip_uuid;  }
    if (_sp_customer_uuid)      { has_sp_customer_uuid =        true;  sp_customer_uuid =       *_sp_customer_uuid;     }
    if (_customer_ip)           { has_customer_ip =             true;  customer_ip =            *_customer_ip;          }
    if (_tech_prefix)           { has_tech_prefix =             true;  tech_prefix =            *_tech_prefix;          }
    if (_att_level_with_sp_ani) { has_att_level_with_sp_ani =   true;  att_level_with_sp_ani =  *_att_level_with_sp_ani;}
    if (_att_level_wo_sp_ani)   { has_att_level_wo_sp_ani =     true;  att_level_wo_sp_ani =    *_att_level_wo_sp_ani;  }

}

string spCustomerIpInfo::__insert(void)
{
    string statement = "INSERT INTO sp_customer_ip " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCustomerIpInfo::__update(void)
{
    __chkUUID();

    string statement = "UPDATE sp_customer_ip SET " + __crtIUS(" = ", false) + "WHERE sp_customer_ip_uuid = " + d->c->quote(sp_customer_ip_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCustomerIpInfo::__delete(void)
{
    if (!has_sp_customer_ip_uuid) throw (AsVsException("Nothing to delete since 'sp_customer_ip_uuid' parameter is missing."));

    string statement = "DELETE FROM sp_customer_ip WHERE sp_customer_ip_uuid = " + d->c->quote(sp_customer_ip_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCustomerIpInfo::__crtIUS(string eqs, bool with_uuid)
{
    stringstream prefix;
    stringstream suffix;
    string delim = "(";

    if (with_uuid)
    {
       if (has_sp_customer_ip_uuid) { prefix << delim << "sp_customer_ip_uuid";         suffix << delim << d->c->quote(sp_customer_ip_uuid);    delim = ","; }
    }
    if (has_sp_customer_uuid)       { prefix << delim << "sp_customer_uuid";            suffix << delim << d->c->quote(sp_customer_uuid);       delim = ","; }
    if (has_customer_ip)            { prefix << delim << "customer_ip";                 suffix << delim << d->c->quote(customer_ip);            delim = ","; }
    if (has_tech_prefix)            { prefix << delim << "tech_prefix";                 suffix << delim << d->c->quote(tech_prefix);            delim = ","; }
    if (has_att_level_with_sp_ani)  { prefix << delim << "att_level_with_sp_ani";       suffix << delim << d->c->quote(att_level_with_sp_ani);  delim = ","; }
    if (has_att_level_wo_sp_ani)    { prefix << delim << "att_level_wo_sp_ani";         suffix << delim << d->c->quote(att_level_wo_sp_ani);    delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();

}

void   spCustomerIpInfo::__rmpIUS(pqxx::result r)
{
    has_sp_customer_ip_uuid =   true; sp_customer_ip_uuid   = r[0]["sp_customer_ip_uuid"].as<string>();
    has_sp_customer_uuid =      true; sp_customer_uuid      = r[0]["sp_customer_uuid"].as<string>();
    has_customer_ip =           true; customer_ip           = r[0]["customer_ip"].as<string>();
    has_tech_prefix =           true; tech_prefix           = r[0]["tech_prefix"].as<string>();
    has_att_level_with_sp_ani = true; att_level_with_sp_ani = r[0]["att_level_with_sp_ani"].as<string>();
    has_att_level_wo_sp_ani =   true; att_level_wo_sp_ani   = r[0]["att_level_wo_sp_ani"].as<string>();

}

string   spCustomerIpInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_customer_ip_uuid)    r.put("sp_customer_ip_uuid",    sp_customer_ip_uuid);
    if (has_sp_customer_uuid)       r.put("sp_customer_uuid",       sp_customer_uuid);
    if (has_customer_ip)            r.put("customer_ip",            customer_ip);
    if (has_tech_prefix)            r.put("tech_prefix",            tech_prefix);
    if (has_att_level_with_sp_ani)  r.put("att_level_with_sp_ani",  att_level_with_sp_ani);
    if (has_att_level_wo_sp_ani)    r.put("att_level_wo_sp_ani",    att_level_wo_sp_ani);

    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}