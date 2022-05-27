#! /bin/bash

test_init_database() {
   psql -h "$DB_HOST" -U postgres -d postgres -f "$DIR_DB_SCRIPTS"/db.drop.sql
   psql -h "$DB_HOST" -U postgres -d postgres -f "$DIR_DB_SCRIPTS"/db.init.sql
}

test_start_app() {
   "$DIR_ROOT"/build/server/server -postgres "user=postgres host="$DB_HOST" port=5432 dbname=postgres password=postgres" -alsologtostderr &
   sleep 2
}

test_sp_account() {
   ! IFS= read -r -d '' data1 << EOM
      { 
         "sp_customer_uuid":"4f4867be-0001-4670-9148-3a9f5c70015e",
         "started_on":"1917-08-01 12:15:01",
         "expired_on":"2199-12-31 15:12:59",
         "active": true,
         "cps_limit": 12,
         "dnc_lrn_replacement":"LRN1"
      }   
EOM

   ! IFS= read -r -d '' data2 << EOM
      { 
         "sp_customer_uuid": "4f4867be-0002-4670-9148-3a9f5c70015e", 
         "dnc_lrn_replacement":"LRN2", 
         "started_on":"1917-08-01 12:15:01",
         "expired_on":"2199-12-31 15:12:59", 
         "active": false, 
         "cps_limit": 12      
      }   
EOM

   ! IFS= read -r -d '' data3 << EOM
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

   ! IFS= read -r -d '' data4 << EOM
      { 
         "sp_customer_uuid": "4f4867be-0100-4670-9148-3a9f5c70015e",
         "started_on":"1917-08-01 12:15:01",
         "expired_on":"2199-12-31 15:12:59",
         "active": true,
         "cps_limit": 12
      }
EOM

   curl -s --header "Content-Type: application/json" --data "$data1"  http://localhost:8080/api/customer_create
   curl -s --header "Content-Type: application/json" --data "$data2"  http://localhost:8080/api/customer_create
   curl -s --header "Content-Type: application/json" --data "$data3"  http://localhost:8080/api/customer_create
   curl -s --header "Content-Type: application/json" --data "$data4"  http://localhost:8080/api/customer_create
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
