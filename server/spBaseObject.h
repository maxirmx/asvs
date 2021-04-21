// spBaseObject 

class spBaseObject;

typedef void (spBaseObject::* fptr_pt)(const std::string& key, const boost::property_tree::ptree& pt);
typedef void (spBaseObject::* fptr_pq)(const std::string& key, const pqxx::row& r);

class spBaseObject
{
public:
    spBaseObject(const std::string& nme, const std::string& key) : tbl_name(nme), master_key(key) {  }

    virtual std::string __insert(void);
    virtual std::string __update(void);
    virtual std::string __delete(void);

    template<typename Ti, typename Tr>
    void __crt_p(const std::string& k, Tr v)
    {
        auto search = fields.find(k);
        if (search != fields.end()) *fields[k] = v;
        else                        fields.insert({ k, std::make_shared<Ti>(v) });

    };

    template<typename Ti, typename Tr>
    void __crt_pt(const std::string& key, const boost::property_tree::ptree& pt)
    {
        auto f = pt.get_optional<Tr>(key);
        if (f) __crt_p<Ti, Tr>(key, *f);
    };

    template<typename Ti, typename Tr>
    void __crt_pq(const std::string& key, const pqxx::row& r)
    {
        auto f = r[key];
        if (!f.is_null()) __crt_p<Ti, Tr>(key, f.as<Tr>());
    };

protected:
    virtual void __rmpINT(const std::string& body);
    virtual void __rmpIUS(const pqxx::row& r);
    virtual std::string __crtIUS(const std::string& eqs, bool with_uuid);
    virtual std::string __srsIUS(void);

    virtual const std::string& __getNME(void) { return tbl_name; }
    virtual const std::string& __getMKE(void) { return master_key; }

    virtual void   __chkUUID(void) { if (fields.find(__getMKE()) == fields.end()) throw (AsVsException("Cannot identify record since '" + __getMKE() + "' parameter is missing.")); }

    std::string tbl_name;
    std::string master_key;
    std::unordered_map<std::string, std::shared_ptr<spBaseColumn>> fields;
    std::unordered_map<std::string, fptr_pt> proto_fields_pt;
    std::unordered_map<std::string, fptr_pq> proto_fields_pq;

};
