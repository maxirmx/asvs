
class ProvApiHandler : public RequestHandler
{
public:
    explicit ProvApiHandler(shared_ptr<DbConnection> dbc) : db_connection(dbc), waiting_post(false) { }
    void onRequest(unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    void onBody(unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(ProxygenError err) noexcept override;

private:
    void logError(uint16_t code, const string& reason, const string& msg);

    string customerCreate     (const string& body);
    string customerUpdate     (const string& body);
    string customerDelete     (const string& body);
    string customerIpCreate   (const string& body);
    string customerIpUpdate   (const string& body);
    string customerIpDelete   (const string& body);
    string accountCreate      (const string& body);
    string accountUpdate      (const string& body);
    string accountDelete      (const string& body);
    string gatewayIpCreate    (const string& body);
    string gatewayIpUpdate    (const string& body);
    string gatewayIpDelete    (const string& body);
    string certCreate         (const string& body);
    string certUpdate         (const string& body);
    string certDelete         (const string& body);
    string tnCreate           (const string& body);
    string tnUpdate           (const string& body);
    string tnDelete           (const string& body);


    shared_ptr<DbConnection> db_connection;
    unique_ptr<HTTPMessage> message;
    bool waiting_post;
};

class ProvApiHandlerFactory : public RequestHandlerFactory 
{
public:
     ProvApiHandlerFactory(shared_ptr<DbConnection> dbc): db_connection(dbc) {   }

     void onServerStart(folly::EventBase* /*evb*/) noexcept override                 {   }
     void onServerStop() noexcept override                                           {   }

     RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override { return new ProvApiHandler(db_connection); }

private:
     shared_ptr<DbConnection> db_connection;
};
