#include "server.h"
#include <stir_shaken.h>

class StirShakenException : public AsVsException {
public:
    explicit StirShakenException(stir_shaken_error_t cd, const std::string& message) : AsVsException(message), code(cd) {  }
    stir_shaken_error_t which(void) const { return code; }
protected:
    stir_shaken_error_t code;
};

class StirShakenAPI  
{
public:
    ~StirShakenAPI();
    void initialize(void);
    void sign(void);

    static std::unique_ptr<StirShakenAPI> api;

private:
    StirShakenAPI(void) { } ;

    stir_shaken_context_t ss = { 0 };
    bool is_initialized = false;
};
