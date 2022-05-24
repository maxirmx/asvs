echo .. sp_tn
echo ..... create
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5001-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"tn\":\"1921\" }" http://localhost:8080/api/tn_create
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9b\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"tn\":\"1923\" }" http://localhost:8080/api/tn_create
echo ..... update
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9b\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"tn\":\"1010\" }" http://localhost:8080/api/tn_update
echo ..... delete
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9b\"}" http://localhost:8080/api/tn_delete
echo ..... errors
# duplicate uuid
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5001-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"tn\":\"1926\" }" http://localhost:8080/api/tn_create
# account does not exist
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"00000001-e002-4438-9fc3-adabdd3c255c\", \"tn\":\"1927\" }" http://localhost:8080/api/tn_create


