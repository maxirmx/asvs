@echo .. sp_customer_ip
@echo ..... create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0001-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0001\" , \"att_level_with_sp_ani\" : \"1\" , \"att_level_wo_sp_ani\" : \"2\"}" http://localhost:8080/api/customer_ip_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0002-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0071\" , \"att_level_with_sp_ani\" : \"1\" , \"att_level_wo_sp_ani\" : \"2\"}" http://localhost:8080/api/customer_ip_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0003-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0701\" , \"att_level_with_sp_ani\" : \"1\" , \"att_level_wo_sp_ani\" : \"2\"}" http://localhost:8080/api/customer_ip_create
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-1003-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0701\" , \"att_level_with_sp_ani\" : \"1\" , \"att_level_wo_sp_ani\" : \"2\"}" http://localhost:8080/api/customer_ip_create
@echo ..... update
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-0001-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"7001\" }" http://localhost:8080/api/customer_ip_update
@echo ..... delete
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-1003-bc7a-e61e08e3869b\"}" http://localhost:8080/api/customer_ip_delete
@echo ..... errors
@rem   att_level_wo_sp_ani too long
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-8003-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0701\" , \"att_level_with_sp_ani\" : \"1\" , \"att_level_wo_sp_ani\" : \"22\"}" http://localhost:8080/api/customer_ip_create
@rem   Bad ip address
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-8003-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"4f4867be-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.1146.20\" , \"tech_prefix\": \"0701\" , \"att_level_with_sp_ani\" : \"1\" , \"att_level_wo_sp_ani\" : \"2\"}" http://localhost:8080/api/customer_ip_create
@rem   Mising customer record
curl -v --header "Content-Type: application/json" --data  "{ \"sp_customer_ip_uuid\":\"8cf1d751-2024-8003-bc7a-e61e08e3869b\",  \"sp_customer_uuid\": \"00000000-0002-4670-9148-3a9f5c70015e\", \"customer_ip\":\"192.168.11.46\" , \"tech_prefix\": \"0701\" , \"att_level_with_sp_ani\" : \"1\" , \"att_level_wo_sp_ani\" : \"2\"}" http://localhost:8080/api/customer_ip_create


    