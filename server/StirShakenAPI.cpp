#include "StirShakenAPI.h"

std::unique_ptr<StirShakenAPI> StirShakenAPI::api;

void StirShakenAPI::initialize(void)
{
    stir_shaken_status_t	status = STIR_SHAKEN_STATUS_FALSE;
	stir_shaken_error_t     error_code = STIR_SHAKEN_ERROR_GENERAL;	
	const char *error_description = "stirshaken error";
    status = stir_shaken_do_init(&ss, NULL, NULL, STIR_SHAKEN_LOGLEVEL_HIGH);
	if (STIR_SHAKEN_STATUS_OK != status) {
        if (stir_shaken_is_error_set(&ss)) error_description = stir_shaken_get_error(&ss, &error_code);        
        throw(StirShakenException(error_code, error_description));
	}
}

void StirShakenAPI::sign(void) {
    stir_shaken_passport_params_t params = {
	    .x5u = "https://shaken.signalwire.cloud/sp.pem",
	    .attest = "B",
	    .desttn_key = "tn",
	    .desttn_val = "01256700800",
	    .iat = time(NULL),
	    .origtn_key = "tn",
	    .origtn_val = "01256500600",
	    .origid = "e32f4189-cb86-460f-bb92-bd3acb89f29c"
    };
}

StirShakenAPI::~StirShakenAPI(void)
{
    stir_shaken_do_deinit();
}
