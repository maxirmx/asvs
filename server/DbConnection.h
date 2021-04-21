#pragma once

class DbConnection
{
public:
    DbConnection(std::string postgr);
    pqxx::result exec(std::string stmt);
    std::string describe(void);

    std::unique_ptr<pqxx::connection> c;
    static std::unique_ptr<DbConnection> d;
private:
    std::string postgres;
};

