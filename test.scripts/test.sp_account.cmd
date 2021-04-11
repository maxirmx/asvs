echo .. sp_customer
echo ..... create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_name\":\"Account 1\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"1\" , \"default_att_level_wo_sp_ani\" : \"2\" }" http://localhost:8080/api/account_create
echo ..... update
echo ..... delete
echo ..... errors
@rem   UUID already exists
@rem   started_on missing 


    