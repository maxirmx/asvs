-- Authentication and Verification Server
-- Database destruction script

DROP TABLE IF EXISTS sp_customer_ip;
DROP TABLE IF EXISTS sp_customers;
DROP TABLE IF EXISTS sp_gateway_ip;
DROP TABLE IF EXISTS sp_tn;
DROP TABLE IF EXISTS sp_accounts;
DROP TYPE IF EXISTS att_level;
DROP TABLE IF EXISTS __db_schema_version;
