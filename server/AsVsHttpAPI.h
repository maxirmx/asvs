class AsVsApiHandler : public RequestHandler
{
public:
    explicit AsVsApiHandler()  { }
    void onRequest(unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    void onBody(unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(ProxygenError err) noexcept override;

private:
    void logError(uint16_t code, const string& reason, const string& msg);

    unique_ptr<HTTPMessage> message;
    bool waiting_post;
};

class AsVsApiHandlerFactory : public RequestHandlerFactory
{
public:
    AsVsApiHandlerFactory(void)  {   }

    void onServerStart(folly::EventBase* /*evb*/) noexcept override {   }
    void onServerStop() noexcept override {   }

    RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override { return new AsVsApiHandler(); }

private:
    shared_ptr<DbConnection> db_connection;
};
