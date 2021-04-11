#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

#include "Util.h"
#include "AccountDb.h"

spAccountInfo::spAccountInfo(const string& body, shared_ptr<DbConnection> dbc):
    d(dbc),
    has_sp_account_uuid(false),
    has_active(false),
    has_started_on(false),
    has_modified_on(false),
    has_expired_on(false),
    has_cps_limit(false),
    has_as_enabled(false),
    has_vs_enabled(false),
    has_default_att_level_with_sp_ani(false),
    has_default_att_level_wo_sp_ani(false),
    has_sp_name(false)

{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_account_uuid =     pt.get_optional<string> ("sp_account_uuid");
    auto _active =              pt.get_optional<bool>   ("active");
    auto _started_on =          pt.get_optional<string> ("started_on");
    auto _modified_on =         pt.get_optional<string> ("modified_on");
    auto _expired_on =          pt.get_optional<string> ("expired_on");
    auto _cps_limit =           pt.get_optional<int32_t>("cps_limit");
    auto _as_enabled =          pt.get_optional<bool>   ("as_enabled");
    auto _vs_enabled =          pt.get_optional<bool>   ("vs_enabled");
    auto _sp_name =             pt.get_optional<string> ("sp_name");
    auto _default_att_level_with_sp_ani =   pt.get_optional<string>("default_att_level_with_sp_ani");
    auto _default_att_level_wo_sp_ani =     pt.get_optional<string>("default_att_level_wo_sp_ani");

    if (_sp_account_uuid)   { has_sp_account_uuid = true;  sp_account_uuid =    *_sp_account_uuid;      }
    if (_active)            { has_active =          true;  active =             *_active;               }
    if (_started_on)        { has_started_on =      true;  started_on =         str2time(*_started_on); }
    if (_modified_on)       { has_modified_on =     true;  modified_on =        str2time(*_modified_on);}
    if (_expired_on)        { has_expired_on =      true;  expired_on =         str2time(*_expired_on); }
    if (_cps_limit)         { has_cps_limit =       true;  cps_limit =          *_cps_limit;            }
    if (_as_enabled)        { has_as_enabled =      true;  as_enabled =         *_as_enabled;           }
    if (_vs_enabled)        { has_vs_enabled =      true;  vs_enabled =         *_vs_enabled;           }
    if (_sp_name)           { has_sp_name =         true;  sp_name =            *_sp_name;              }
    if (_default_att_level_with_sp_ani) { has_default_att_level_with_sp_ani = true;     default_att_level_with_sp_ani = *_default_att_level_with_sp_ani; }
    if (_default_att_level_wo_sp_ani)   { has_default_att_level_wo_sp_ani = true;       default_att_level_wo_sp_ani =   *_default_att_level_wo_sp_ani;   }

}

string spAccountInfo::__insert(void)
{
    has_modified_on = true; 
    modified_on = chrono::system_clock::now();

    string statement = "INSERT INTO sp_accounts " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spAccountInfo::__update(void)
{
    __chkUUID();

    has_modified_on = true;
    modified_on = chrono::system_clock::now();
  
    string statement = "UPDATE sp_accounts SET " + __crtIUS(" = ", false) + "WHERE sp_account_uuid = " + d->c->quote(sp_account_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spAccountInfo::__delete(void)
{
    __chkUUID();

    string statement = "DELETE FROM sp_accounts WHERE sp_account_uuid = " + d->c->quote(sp_account_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spAccountInfo::__crtIUS(string eqs, bool with_uuid)
{
    stringstream prefix; 
    stringstream suffix; 
    string delim = "(";

    if (with_uuid) 
    {
       if (has_sp_account_uuid) { prefix << delim << "sp_account_uuid";     suffix << delim << d->c->quote(sp_account_uuid);         delim = ","; }
    }
    if (has_active)             { prefix << delim << "active";              suffix << delim << d->c->quote(active);                  delim = ","; }
    if (has_started_on)         { prefix << delim << "started_on";          suffix << delim << d->c->quote(time2str(started_on));    delim = ","; }
    if (has_expired_on)         { prefix << delim << "expired_on";          suffix << delim << d->c->quote(time2str(expired_on));    delim = ","; }
    if (has_modified_on)        { prefix << delim << "modified_on";         suffix << delim << d->c->quote(time2str(modified_on));   delim = ","; }
    if (has_cps_limit)          { prefix << delim << "cps_limit";           suffix << delim << d->c->quote(cps_limit);               delim = ","; }
    if (has_as_enabled)         { prefix << delim << "as_enabled";          suffix << delim << d->c->quote(as_enabled);              delim = ","; }
    if (has_vs_enabled)         { prefix << delim << "vs_enabled";          suffix << delim << d->c->quote(vs_enabled);              delim = ","; }
    if (has_sp_name)   		{ prefix << delim << "sp_name";    	    suffix << delim << d->c->quote(sp_name);        	     delim = ","; }
    if (has_default_att_level_with_sp_ani) { prefix << delim << "default_att_level_with_sp_ani"; suffix << delim << d->c->quote(default_att_level_with_sp_ani);  delim = ","; }
    if (has_default_att_level_wo_sp_ani)   { prefix << delim << "default_att_level_wo_sp_ani";   suffix << delim << d->c->quote(default_att_level_wo_sp_ani);    delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();

}

void   spAccountInfo::__rmpIUS(pqxx::result r)
{
    has_sp_account_uuid = true; sp_account_uuid =   r[0]["sp_account_uuid"].as<string>();
    has_active = true;          active =            r[0]["active"].as<bool>();
    has_started_on = true;      started_on =        str2time(r[0]["started_on"].as<string>());
    has_modified_on = true;     modified_on =       str2time(r[0]["modified_on"].as<string>());
    has_expired_on = true;      expired_on =        str2time(r[0]["expired_on"].as<string>());
    has_cps_limit = true;       cps_limit =         r[0]["cps_limit"].as<int>();
    has_as_enabled = true;      as_enabled =        r[0]["as_enabled"].as<bool>();
    has_vs_enabled = true;      vs_enabled =        r[0]["vs_enabled"].as<bool>();
    has_sp_name = true;		sp_name =  	    r[0]["sp_name"].as<string>();
    has_default_att_level_with_sp_ani = true; default_att_level_with_sp_ani = r[0]["default_att_level_with_sp_ani"].as<string>();
    has_default_att_level_wo_sp_ani   = true; default_att_level_wo_sp_ani =   r[0]["default_att_level_wo_sp_ani"].as<string>();

}

string   spAccountInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_account_uuid)        r.put("sp_account_uuid",    sp_account_uuid);
    if (has_active)                 r.put("active",             active);
    if (has_started_on)             r.put("started_on",         time2str(started_on));
    if (has_expired_on)             r.put("expired_on",         time2str(expired_on));
    if (has_modified_on)            r.put("modified_on",        time2str(modified_on));
    if (has_cps_limit)              r.put("cps_limit",          cps_limit);
    if (has_as_enabled)             r.put("as_enabled",         as_enabled);
    if (has_vs_enabled)             r.put("vs_enabled",         vs_enabled);
    if (has_sp_name)		        r.put("sp_name",   		sp_name);
    if (has_default_att_level_with_sp_ani) r.put("default_att_level_with_sp_ani", default_att_level_with_sp_ani);
    if (has_default_att_level_wo_sp_ani)   r.put("default_att_level_wo_sp_ani",   default_att_level_wo_sp_ani);
    
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}

// --------------------------------------------------------------------------------------------------------------------------

spGatewayIpInfo::spGatewayIpInfo(const string& body, shared_ptr<DbConnection> dbc):
    d(dbc),
    has_sp_account_uuid(false),
    has_sp_gateway_ip_uuid(false),
    has_gateway_ip(false)
{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_gateway_ip_uuid    = pt.get_optional<string>("sp_gateway_ip_uuid");
    auto _sp_account_uuid       = pt.get_optional<string>("sp_account_uuid");
    auto _gateway_ip            = pt.get_optional<string>("gateway_ip");

    if (_sp_gateway_ip_uuid)    { has_sp_gateway_ip_uuid =  true;  sp_gateway_ip_uuid =     *_sp_gateway_ip_uuid; }
    if (_sp_account_uuid)       { has_sp_account_uuid   =   true;  sp_account_uuid =        *_sp_account_uuid;    }
    if (_gateway_ip)            { has_gateway_ip        =   true;  gateway_ip =             *_gateway_ip;         }

}

string spGatewayIpInfo::__insert(void)
{
    string statement = "INSERT INTO sp_gateway_ip " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spGatewayIpInfo::__update(void)
{
    __chkUUID();

    string statement = "UPDATE sp_gateway_ip SET " + __crtIUS(" = ", false) + "WHERE sp_gateway_ip_uuid = " + d->c->quote(sp_gateway_ip_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spGatewayIpInfo::__delete(void)
{
    __chkUUID();

    string statement = "DELETE FROM sp_gateway_ip WHERE sp_gateway_ip_uuid = " + d->c->quote(sp_gateway_ip_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spGatewayIpInfo::__crtIUS(string eqs, bool with_uuid)
{
    stringstream prefix;
    stringstream suffix;
    string delim = "(";

    if (with_uuid)
    {
        if (has_sp_gateway_ip_uuid) { prefix << delim << "sp_gateway_ip_uuid";         suffix << delim << d->c->quote(sp_gateway_ip_uuid);    delim = ","; }
    }
    if (has_sp_account_uuid) { prefix << delim << "sp_account_uuid";            suffix << delim << d->c->quote(sp_account_uuid);       delim = ","; }
    if (has_gateway_ip) { prefix << delim << "gateway_ip";                 suffix << delim << d->c->quote(gateway_ip);            delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();
}

void   spGatewayIpInfo::__rmpIUS(pqxx::result r)
{
    has_sp_gateway_ip_uuid = true; sp_gateway_ip_uuid = r[0]["sp_gateway_ip_uuid"].as<string>();
    has_sp_account_uuid = true; sp_account_uuid = r[0]["sp_account_uuid"].as<string>();
    has_gateway_ip = true; gateway_ip = r[0]["gateway_ip"].as<string>();
}

string   spGatewayIpInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_gateway_ip_uuid)    r.put("sp_gateway_ip_uuid", sp_gateway_ip_uuid);
    if (has_sp_account_uuid)       r.put("sp_account_uuid", sp_account_uuid);
    if (has_gateway_ip)            r.put("gateway_ip", gateway_ip);

    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}

// --------------------------------------------------------------------------------------------------------------------------

spTnInfo::spTnInfo(const string& body, shared_ptr<DbConnection> dbc):
    d(dbc),
    has_sp_account_uuid(false),
    has_sp_tn_uuid(false),
    has_tn(false)
{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_tn_uuid = pt.get_optional<string>("sp_tn_uuid");
    auto _sp_account_uuid = pt.get_optional<string>("sp_account_uuid");
    auto _tn = pt.get_optional<string>("tn");

    if (_sp_tn_uuid) 		{ has_sp_tn_uuid = true;  	    sp_tn_uuid = *_sp_tn_uuid; 	        	}
    if (_sp_account_uuid) 	{ has_sp_account_uuid = true;  	sp_account_uuid = *_sp_account_uuid; 	}
    if (_tn) 			    { has_tn = true;  		        tn = *_tn; 				                }
        
}

string spTnInfo::__insert(void)
{
    string statement = "INSERT INTO sp_tn " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spTnInfo::__update(void)
{
    __chkUUID();

    string statement = "UPDATE sp_tn SET " + __crtIUS(" = ", false) + "WHERE sp_tn_uuid = " + d->c->quote(sp_tn_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spTnInfo::__delete(void)
{
    __chkUUID();

    string statement = "DELETE FROM sp_tn WHERE sp_tn_uuid = " + d->c->quote(sp_tn_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spTnInfo::__crtIUS(string eqs, bool with_uuid)
{
    stringstream prefix;
    stringstream suffix;
    string delim = "(";

    if (with_uuid)
    {
        if (has_sp_tn_uuid) { prefix << delim << "sp_tn_uuid";         suffix << delim << d->c->quote(sp_tn_uuid);    delim = ","; }
    }
    if (has_sp_account_uuid) { prefix << delim << "sp_account_uuid";            suffix << delim << d->c->quote(sp_account_uuid);       delim = ","; }
    if (has_tn) { prefix << delim << "tn";                 suffix << delim << d->c->quote(tn);            delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();
}

void   spTnInfo::__rmpIUS(pqxx::result r)
{
    has_sp_tn_uuid = true; sp_tn_uuid = r[0]["sp_tn_uuid"].as<string>();
    has_sp_account_uuid = true; sp_account_uuid = r[0]["sp_account_uuid"].as<string>();
    has_tn = true; tn = r[0]["tn"].as<string>();
}

string   spTnInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_tn_uuid)    r.put("sp_tn_uuid", sp_tn_uuid);
    if (has_sp_account_uuid)       r.put("sp_account_uuid", sp_account_uuid);
    if (has_tn)            r.put("tn", tn);

    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}

// --------------------------------------------------------------------------------------------------------------------------

spCertInfo::spCertInfo(const string& body, shared_ptr<DbConnection> dbc):
    d(dbc),
    has_sp_account_uuid(false),
    has_sp_cert_uuid(false),
    has_priv_key(false),
    has_pem_file_path(false)
{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_account_uuid =     pt.get_optional<string>("sp_account_uuid");
    auto _sp_cert_uuid =        pt.get_optional<string>("sp_cert_uuid");
    auto _priv_key =            pt.get_optional<string>("priv_key");
    auto _pem_file_path =       pt.get_optional<string>("pem_file_path");

    if (_sp_cert_uuid)      { has_sp_cert_uuid =    true;  sp_cert_uuid =       *_sp_cert_uuid;         }
    if (_sp_account_uuid)   { has_sp_account_uuid = true;  sp_account_uuid =    *_sp_account_uuid;      }
    if (_priv_key)          { has_priv_key =        true;  priv_key =           *_priv_key;             }
    if (_pem_file_path)     { has_pem_file_path =   true;  pem_file_path =      *_pem_file_path;        }
}

string spCertInfo::__insert(void)
{
    string statement = "INSERT INTO sp_cert " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCertInfo::__update(void)
{
    __chkUUID();

    string statement = "UPDATE sp_cert SET " + __crtIUS(" = ", false) + "WHERE sp_cert_uuid = " + d->c->quote(sp_cert_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCertInfo::__delete(void)
{
    __chkUUID();

    string statement = "DELETE FROM sp_cert WHERE sp_cert_uuid = " + d->c->quote(sp_cert_uuid) + " RETURNING * ";

    pqxx::result r = d->exec(statement);
    __rmpIUS(r);

    return __srsIUS();
}

string spCertInfo::__crtIUS(string eqs, bool with_uuid)
{
    stringstream prefix;
    stringstream suffix;
    string delim = "(";

    if (with_uuid)
    {
        if (has_sp_cert_uuid) { prefix << delim << "sp_cert_uuid";         suffix << delim << d->c->quote(sp_cert_uuid);    delim = ","; }
    }
    if (has_sp_account_uuid) { prefix << delim << "sp_account_uuid";            suffix << delim << d->c->quote(sp_account_uuid);       delim = ","; }
    if (has_priv_key) { prefix << delim << "priv_key";                 suffix << delim << d->c->quote(priv_key);            delim = ","; }
    if (has_pem_file_path) { prefix << delim << "pem_file_path";                 suffix << delim << d->c->quote(pem_file_path);            delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();
}

void   spCertInfo::__rmpIUS(pqxx::result r)
{
    has_sp_cert_uuid = true; sp_cert_uuid = r[0]["sp_cert_uuid"].as<string>();
    has_sp_account_uuid = true; sp_account_uuid = r[0]["sp_account_uuid"].as<string>();
    has_priv_key = true; priv_key = r[0]["priv_key"].as<string>();
    has_pem_file_path = true; pem_file_path = r[0]["pem_file_path"].as<string>();
}

string   spCertInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_cert_uuid)    r.put("sp_cert_uuid", sp_cert_uuid);
    if (has_sp_account_uuid)       r.put("sp_account_uuid", sp_account_uuid);
    if (has_priv_key)            r.put("priv_key", priv_key);
    if (has_pem_file_path)            r.put("pem_file_path", pem_file_path);

    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}

