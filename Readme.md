# Authentication and Verification Server 

## Starting at 192.99.10.113:
```
cd /home/maxim/Development/stirshaken-asvs-server/server/release
./server --alsologtostderr=1 --postgres "user=postgres host=localhost port=5432 dbname=stirshaken" -prov_api_http_port 8082 --v=1
```

## Command line parameters:
    * -asvs_api_http_ip (AS&VS HTTP API IP/Hostname) type: string  default: "localhost"
    * -asvs_api_http_port (AS&VS HTTP API port) type: int32 default: 8081
    * -postgres (PostgresSql connection string ("user=postgres host=localhost port=5432 dbname=urls_remap password=changeme")) type: string default: ""
    * -prov_api_http_ip (Provisioning API IP/Hostname) type: string  default: "localhost"
    * -prov_api_http_port (Provisioning API port) type: int32 default: 8080
    * -threads (Number of threads used by AS&VS HTTP API. Zero means to default to the number of cores on this machine.) type: int32 default: 0

# Provisioning API
## Understanding updates
Provisioning API implements update interface for all database entities - sp_customers, sp_customer_ip, sp_accounts, ets ...
When update interface is invoked the is onle one mandatory parameter - uuid? that is used to identify the record. All other fields are optional and will be loaded from the database.
For example, the following call:
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\", \"vs_enabled\": false }" http://localhost:8082/api/customer_update
```
will change "vs_enabled" to "false", but won't modify "as_enabled".

A blind call, without any fields to change like the following, will force reload of the record from the database:
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\"}" http://localhost:8082/api/customer_update
```

## Sample provisioning API calls
### sp_customer
#### ..... create
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_name\":\"Customer 1\", \"as_started_on\":\"1917-08-01 12:15:01\", \"as_expired_on\":\"2199-12-31 15:12:59\", \"vs_started_on\":\"1917-08-01 12:15:01\", \"vs_expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"as_cps_limit\": 12, \"vs_cps_limit\": 100 }" http://localhost:8082/api/customer_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Customer 2\", \"as_started_on\":\"1917-08-01 12:15:01\", \"as_expired_on\":\"2199-12-31 15:12:59\", \"vs_started_on\":\"1917-08-01 12:15:01\", \"vs_expired_on\":\"2199-12-31 15:12:59\", \"active\": false, \"as_cps_limit\": 12, \"vs_cps_limit\": 100 }" http://localhost:8082/api/customer_create
curl -v --header "Content-Type: application/json" --data  "{  \"sp_customer_name\":\"Customer 3\", \"as_started_on\":\"1917-08-01 12:15:01\", \"as_expired_on\":\"2199-12-31 15:12:59\", \"vs_started_on\":\"1917-08-01 12:15:01\", \"vs_expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"as_cps_limit\": 1111, \"vs_cps_limit\": 2000, \"as_enabled\": false, \"vs_enabled\": false}" http://localhost:8082/api/customer_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Customer X\", \"as_started_on\":\"1917-08-01 12:15:01\", \"as_expired_on\":\"2199-12-31 15:12:59\", \"vs_started_on\":\"1917-08-01 12:15:01\", \"vs_expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"as_cps_limit\": 12, \"vs_cps_limit\": 0 }" http://localhost:8082/api/customer_create
```
#### ..... update
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\", \"active\": false }" http://localhost:8082/api/customer_update
```
#### ..... delete
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\" }" http://localhost:8082/api/customer_delete
```
### sp_customer_ip
#### ..... create
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0001-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0001\" , \"att_level_wo_sp_ani\" : \"A\"}" http://localhost:8082/api/customer_ip_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0002-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0071\" , \"att_level_non_US_ani\" : \"C\" , \"att_level_wo_sp_ani\" : \"B\"}" http://localhost:8082/api/customer_ip_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0003-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0701\" , \"att_level_with_sp_ani\" : \"A\" , \"att_level_wo_sp_ani\" : \"A\"}" http://localhost:8082/api/customer_ip_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-1003-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0701\" , \"att_level_with_sp_ani\" : \"A\" , \"att_level_wo_sp_ani\" : \"C\"}" http://localhost:8082/api/customer_ip_create
```
#### ..... update
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0001-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"7001\" }" http://localhost:8082/api/customer_ip_update
```
#### ..... delete
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-1003-bc7a-e61e08e3869b\"}" http://localhost:8082/api/customer_ip_delete
```
### sp_account
#### ..... create
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"sp_name\":\"Account 1\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"C\" , \"default_att_level_wo_sp_ani\" : \"C\"}" http://localhost:8082/api/account_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e002-4438-9fc3-adabdd3c255c\", \"sp_name\":\"Account 2\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"C\" , \"default_att_level_wo_sp_ani\" : \"C\", \"priv_phase\":\"alfa beta gamma\"}" http://localhost:8082/api/account_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e101-4438-9fc3-adabdd3c255c\", \"sp_name\":\"Account 101\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"default_att_level_with_sp_ani\" : \"C\" , \"default_att_level_wo_sp_ani\" : \"C\"}" http://localhost:8082/api/account_create
```
#### ..... update
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e101-4438-9fc3-adabdd3c255c\", \"default_att_level_wo_sp_ani\" : \"A\", \"cps_limit\":\"100\"}" http://localhost:8080/api/account_update
```
#### ..... delete
```
curl -v --header "Content-Type: application/json" --data  "{ \"sp_account_uuid\" : \"97092d99-e101-4438-9fc3-adabdd3c255c\"}" http://localhost:8080/api/account_delete
```
### sp_as_gateway_ip
#### ..... create
```
curl -v --header "Content-Type: application/json" --data  "{  \"sp_as_gateway_ip_uuid\": \"1d92bf3d-4001-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.46\" }" http://localhost:8082/api/as_gateway_ip_create
curl -v --header "Content-Type: application/json" --data  "{  \"sp_as_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.47\" }" http://localhost:8082/api/as_gateway_ip_create
```
#### ..... update
```
curl -v --header "Content-Type: application/json" --data  "{  \"sp_as_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"10.10.11.47\" }" http://localhost:8082/api/as_gateway_ip_update
```
#### ..... delete
```
curl -v --header "Content-Type: application/json" --data  "{  \"sp_as_gateway_ip_uuid\": \"1d92bf3d-4A01-4ed9-bbda-541ed9589c9a\"}" http://localhost:8080/api/as_gateway_ip_delete
```
### sp_vs_gateway_ip
#### ..... create
```
curl -v --header "Content-Type: application/json" --data  "{  \"sp_vs_gateway_ip_uuid\": \"1d92bf3d-5001-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.46\" }" http://localhost:8082/api/vs_gateway_ip_create
curl -v --header "Content-Type: application/json" --data  "{  \"sp_vs_gateway_ip_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"192.168.11.47\" }" http://localhost:8082/api/vs_gateway_ip_create
```
#### ..... update
```
curl -v --header "Content-Type: application/json" --data  "{  \"sp_vs_gateway_ip_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"gateway_ip\":\"10.10.11.47\" }" http://localhost:8082/api/vs_gateway_ip_update
```
#### ..... delete
```
curl -v --header "Content-Type: application/json" --data  "{  \"sp_vs_gateway_ip_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9a\"}" http://localhost:8080/api/vs_gateway_ip_delete
```
### sp_tn
#### ..... create
```
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5001-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"tn\":\"1921\" }" http://localhost:8082/api/tn_create
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"tn\":\"1923\" }" http://localhost:8082/api/tn_create
```
#### ..... update
```
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9a\",\"sp_account_uuid\": \"97092d99-e001-4438-9fc3-adabdd3c255c\", \"tn\":\"1010\" }" http://localhost:8082/api/tn_update
```
#### ..... delete
```
curl --header "Content-Type: application/json" --data  "{  \"sp_tn_uuid\": \"1d92bf3d-5A01-4ed9-bbda-541ed9589c9a\"}" http://localhost:8080/api/tn_delete
```
