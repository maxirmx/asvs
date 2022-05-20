echo .. sp_account
echo ..... create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"sp_name\":\"Account 1\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"C\" , \"default_att_level_wo_sp_ani\" : \"C\"}" http://localhost:8080/api/account_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e002-4438-9fc3-adabdd3c255c\", \"sp_name\":\"Account 2\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"C\" , \"default_att_level_wo_sp_ani\" : \"C\", \"priv_phase\":\"alfa beta gamma\"}" http://localhost:8080/api/account_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e101-4438-9fc3-adabdd3c255c\", \"sp_name\":\"Account 101\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"C\" , \"default_att_level_wo_sp_ani\" : \"C\"}" http://localhost:8080/api/account_create
echo ..... update
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e101-4438-9fc3-adabdd3c255c\", \"default_att_level_wo_sp_ani\" : \"A\", \"cps_limit\":\"100\"}" http://localhost:8080/api/account_update
echo ..... delete
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e101-4438-9fc3-adabdd3c255c\"}" http://localhost:8080/api/account_delete
echo ..... errors
# UUID does not exist
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e102-4438-9fc3-adabdd3c255c\"}" http://localhost:8080/api/account_delete
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e111-4438-9fc3-adabdd3c255c\", \"default_att_level_wo_sp_ani\" : \"D\", \"cps_limit\":\"100\"}" http://localhost:8080/api/account_update
# started_on missing
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e112-4438-9fc3-adabdd3c255c\", \"sp_name\":\"Account 1\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"C\" , \"default_att_level_wo_sp_ani\" : \"C\"}" http://localhost:8080/api/account_create


