@echo .. sp_customer
@echo ..... create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0001-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Customer 1\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"cps_limit\": 1 }" http://localhost:8080/api/customer_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Customer 2\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2198-12-31 16:12:59\", \"active\": false, \"cps_limit\": 2 }" http://localhost:8080/api/customer_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0003-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Customer 3\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2197-12-31 17:12:59\", \"active\": true, \"cps_limit\": 3 }" http://localhost:8080/api/customer_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Customer 4\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2196-12-31 18:12:59\", \"active\": true, \"cps_limit\": 0 }" http://localhost:8080/api/customer_create
@echo ..... update
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\", \"active\": false }" http://localhost:8080/api/customer_update
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0003-4670-9148-3a9f5c70015e\", \"vs_enabled\": false , \"cps_limit\": 12345 }" http://localhost:8080/api/customer_update
@echo ..... delete
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0100-4670-9148-3a9f5c70015e\" }" http://localhost:8080/api/customer_delete
@echo ..... errors
@rem   UUID already exists
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-0001-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Bad Customer 1\", \"started_on\":\"1917-08-01 12:15:01\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"cps_limit\": 1 }" http://localhost:8080/api/customer_create
@rem   started_on missing 
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_uuid\": \"4f4867be-1001-4670-9148-3a9f5c70015e\", \"sp_customer_name\":\"Bad Customer 2\", \"expired_on\":\"2199-12-31 15:12:59\", \"active\": true, \"cps_limit\": 1 }" http://localhost:8080/api/customer_create


    