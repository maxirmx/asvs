// spBaseColumn 
// spStringColumn 
// spBoolColumn 
// spInt32Column 
// spTimeColumn

class spBaseColumn
{
public:
    spBaseColumn(void) {  }
    virtual std::string& getStringVal(void)                         { throw InternalError("PANIC: Unexpected call to spBaseColumn::getStringVal"); }
    virtual bool getBoolVal(void)                                   { throw InternalError("PANIC: Unexpected call to spBaseColumn::getBoolVal"); }
    virtual int32_t getInt32Val(void)                               { throw InternalError("PANIC: Unexpected call to spBaseColumn::getInt32Val"); }
    virtual std::chrono::system_clock::time_point& getTimeVal(void) { throw InternalError("PANIC: Unexpected call to spBaseColumn::getTimeVal"); }

    virtual void setVal(std::string& val)                           { throw InternalError("PANIC: Unexpected call to spBaseColumn::setVal(string)"); }
    virtual void setVal(bool val)                                   { throw InternalError("PANIC: Unexpected call to spBaseColumn::setVal(bool)"); }
    virtual void setVal(int32_t val)                                { throw InternalError("PANIC: Unexpected call to spBaseColumn::setVal(int32)"); }
    virtual void setVal(std::chrono::system_clock::time_point& val) { throw InternalError("PANIC: Unexpected call to spBaseColumn::setVal(time_point)"); }

    virtual spBaseColumn& operator = (std::string& val)                             { setVal(val); return *this; }
    virtual spBaseColumn& operator = (bool val)                                     { setVal(val); return *this; }
    virtual spBaseColumn& operator = (int32_t val)                                  { setVal(val); return *this; }
    virtual spBaseColumn& operator = (std::chrono::system_clock::time_point& val)   { setVal(val); return *this; }

    virtual std::string dump(void)                                              { throw InternalError("PANIC: Unexpected call to spBaseColumn::dump"); }
    virtual void put(const std::string& key, boost::property_tree::ptree& r)    { throw InternalError("PANIC: Unexpected call to spBaseColumn::put"); }

};

class spStringColumn : public spBaseColumn
{
public:
    spStringColumn(std::string& val) : spBaseColumn()                           { setVal(val); }
    virtual std::string& getStringVal(void)                                     { return v; }
    virtual void setVal(std::string& val)                                       { v = val; }
    virtual std::string dump(void)                                              { return DbConnection::d->c->quote(getStringVal()); }
    virtual void put(const std::string& key, boost::property_tree::ptree& r)    { r.put(key, getStringVal()); }
private:
    std::string v;
};

class spBoolColumn : public spBaseColumn
{
public:
    spBoolColumn(bool val) : spBaseColumn()                                     { setVal(val); }
    virtual bool getBoolVal(void)                                               { return v; }
    virtual void setVal(bool val)                                               { v = val; }
    virtual std::string dump(void)                                              { return DbConnection::d->c->quote(getBoolVal()); }
    virtual void put(const std::string& key, boost::property_tree::ptree& r)    { r.put(key, getBoolVal()); }
private:
    bool v;
};

class spInt32Column : public spBaseColumn
{
public:
    spInt32Column(int32_t val) : spBaseColumn()                                 { setVal(val); }
    virtual int32_t getInt32Val(void)                                           { return v; }
    virtual void setVal(int32_t val)                                            { v = val; }
    virtual std::string dump(void)                                              { return DbConnection::d->c->quote(getInt32Val()); }
    virtual void put(const std::string& key, boost::property_tree::ptree& r)    { r.put(key, getInt32Val()); }
private:
    int32_t v;
};

class spTimeColumn : public spBaseColumn
{
public:
    spTimeColumn(std::string& val) : spBaseColumn()                             { setVal(val); }
    spTimeColumn(std::chrono::system_clock::time_point& val) : spBaseColumn()   { setVal(val); }
    virtual std::chrono::system_clock::time_point& getTimeVal(void)             { return v; }
    virtual std::string& getStringVal(void)                                     { vx = time2str(v); return vx; }
    virtual void setVal(std::string& val)                                       { setVal(str2time(val)); }
    virtual void setVal(const std::chrono::system_clock::time_point& val)       { v = val; }
    virtual std::string dump(void)                                              { return DbConnection::d->c->quote(getStringVal()); }
    virtual void put(const std::string& key, boost::property_tree::ptree& r)    { r.put(key, getStringVal()); }
private:
    std::chrono::system_clock::time_point v;
    std::string vx;
};

