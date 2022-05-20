echo .. sp_gateway_ip
echo ..... create
curl -v --header "Content-Type: application/json" --data  "{  \"sp_gateway_ip_uuid\": \"1d92bf3d-4001-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.46\" }" http://localhost:8080/api/gateway_ip_create
curl -v --header "Content-Type: application/json" --data  "{  \"sp_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9b\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.47\" }" http://localhost:8080/api/gateway_ip_create
curl -v --header "Content-Type: application/json" --data  "{  \"sp_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9c\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.47\" }" http://localhost:8080/api/gateway_ip_create
echo ..... update
curl -v --header "Content-Type: application/json" --data  "{  \"sp_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9b\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"10.10.11.47\" }" http://localhost:8080/api/gateway_ip_update
echo ..... delete
curl -v --header "Content-Type: application/json" --data  "{  \"sp_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9c\"}" http://localhost:8080/api/gateway_ip_delete
echo ..... errors
# duplicate uuid
curl -v --header "Content-Type: application/json" --data  "{  \"sp_gateway_ip_uuid\": \"1d92bf3d-4001-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.46\" }" http://localhost:8080/api/gateway_ip_create
# account does not exist
curl -v --header "Content-Type: application/json" --data  "{  \"sp_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9c\",\"sp_account_uuid\": \"00000001-e002-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.47\" }" http://localhost:8080/api/gateway_ip_create


