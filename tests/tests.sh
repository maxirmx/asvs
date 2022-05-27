#! /bin/bash

# -----------------------------------------------------
IFS=' ' read -r -a array <<< "$(ip route)"
c_uuid="4f4867be-0001-4670-9148-3a9f5c70015e"
c_ip="${array[2]}"
# -----------------------------------------------------

test_init_database() {
   psql -h "$DB_HOST" -U postgres -d postgres -f "$DIR_DB_SCRIPTS"/db.drop.sql
   psql -h "$DB_HOST" -U postgres -d postgres -f "$DIR_DB_SCRIPTS"/db.init.sql
}

test_start_app() {
   "$DIR_ROOT"/build/server/server -postgres "user=postgres host="$DB_HOST" port=5432 dbname=postgres password=postgres" -alsologtostderr &
   sleep 2
}

test_sp_customer() {
   ! IFS= read -r -d '' data101 << EOM
      { 
         "sp_customer_uuid":"$c_uuid",
         "started_on":"1917-08-01 12:15:01",
         "expired_on":"2199-12-31 15:12:59",
         "active": true,
         "cps_limit": 12,
         "dnc_lrn_replacement":"LRN1"
      }   
EOM

   ! IFS= read -r -d '' data102 << EOM
      { 
         "sp_customer_uuid": "4f4867be-0002-4670-9148-3a9f5c70015e", 
         "dnc_lrn_replacement":"LRN2", 
         "started_on":"1917-08-01 12:15:01",
         "expired_on":"2199-12-31 15:12:59", 
         "active": false, 
         "cps_limit": 12      
      }   
EOM

   ! IFS= read -r -d '' data103 << EOM
      { 
         "sp_customer_uuid": "4f4867be-0003-4670-9148-3a9f5c70015e",  
         "started_on":"1917-08-01 12:15:01", 
         "expired_on":"2199-12-31 15:12:59", 
         "active": true, 
         "cps_limit": 1111, 
         "as_enabled": false, 
         "vs_enabled": false      
      }   
EOM

   ! IFS= read -r -d '' data104 << EOM
      { 
         "sp_customer_uuid": "4f4867be-0100-4670-9148-3a9f5c70015e",
         "started_on":"1917-08-01 12:15:01",
         "expired_on":"2199-12-31 15:12:59",
         "active": true,
         "cps_limit": 12
      }
EOM

   curl -s --header "Content-Type: application/json" --data "$data101"  http://localhost:8080/api/customer_create
   curl -s --header "Content-Type: application/json" --data "$data102"  http://localhost:8080/api/customer_create
   curl -s --header "Content-Type: application/json" --data "$data103"  http://localhost:8080/api/customer_create
   curl -s --header "Content-Type: application/json" --data "$data104"  http://localhost:8080/api/customer_create
}

test_sp_customer_ip() {
   ! IFS= read -r -d '' data201 << EOM
      { 
         "sp_customer_ip_uuid":"8cf1d751-2024-0001-bc7a-e61e08e3869b",  
         "sp_customer_uuid": "$c_uuid", 
         "customer_ip":"127.0.0.1" , 
         "orig_id": "0001" , 
         "tech_prefix": "0001" , 
         "att_level_wo_sp_ani" : "A"      
      }   
EOM

   ! IFS= read -r -d '' data202 << EOM
      { 
         "sp_customer_ip_uuid":"8cf1d751-2024-0002-bc7a-e61e08e3869b",  
         "sp_customer_uuid": "$c_uuid", 
         "customer_ip":"$c_ip" , 
         "orig_id": "0002" , 
         "tech_prefix": "0002" , 
         "att_level_wo_sp_ani" : "A"      
      }   
EOM

   curl -s --header "Content-Type: application/json" --data "$data201"  http://localhost:8080/api/customer_ip_create
   curl -s --header "Content-Type: application/json" --data "$data202"  http://localhost:8080/api/customer_ip_create

}

test_signing() {
   curl -v --header "Content-Type: application/json" --header "X-RequestID: 051ddf12-0001-46dd-b46c-59c1036fae75"  --data  "{ \"uuid\" : \"97092d99-e001-4438-9fc3-adabdd3c255c\"}" http://localhost:8081/stir/v1/signing
}

test_stop_app() {
   curl --header "Content-Type: application/json"  http://localhost:8080/api/stop
}

test_delete_database() {
   psql -h "$DB_HOST" -U postgres -d postgres -f "$DIR_DB_SCRIPTS"/db.delete.sql
}

# ......................................................................
# main

DIR0=$( dirname "$0" )
DIR_ROOT=$( cd "$DIR0"/.. && pwd )
DIR_DB_SCRIPTS="$DIR_ROOT"/db.scripts
DIR_TESTS="$DIR_ROOT"/tests

DB_HOST="${DB_HOST:-localhost}"
export PGPASSWORD="${PGPASSWORD:-postgres}"


echo "Tests"
# shellcheck source=/dev/null
. "$DIR_TESTS/shunit2/shunit2"
