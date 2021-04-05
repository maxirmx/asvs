-- Authentication and Verification Server
-- Database initialization script

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE public.__db_schema_version
(
    version_uuid uuid default uuid_generate_v4(),
    version_number integer, 
    created_on timestamp without time zone not null
)
TABLESPACE pg_default;

ALTER TABLE public.__db_schema_version
    OWNER to postgres;

INSERT INTO __db_schema_version (version_number, created_on) VALUES  (1, now());


CREATE TABLE public.cert
(
    uuid character varying COLLATE pg_catalog."default" NOT NULL,
    certificate character varying COLLATE pg_catalog."default" NOT NULL,
    private_key character varying COLLATE pg_catalog."default" NOT NULL
)
TABLESPACE pg_default;

ALTER TABLE public.cert
    OWNER to postgres;

CREATE TABLE public.sp_customers
(
    sp_customer_uuid uuid default uuid_generate_v4(),
    active boolean default true, 
    started_on timestamp without time zone not null,
    modified_on timestamp without time zone not null,
    expired_on timestamp without time zone not null,
    cps_limit integer,
    as_enabled boolean default true,
    vs_enabled boolean default true,
    sp_customer_name character varying COLLATE pg_catalog."default" NOT NULL,
    PRIMARY KEY(sp_customer_uuid)
)
TABLESPACE pg_default;

ALTER TABLE public.sp_customers
    OWNER to postgres;

CREATE TABLE public.sp_customer_ip
(
    sp_customer_ip_uuid uuid  default uuid_generate_v4(), 
    sp_customer_uuid uuid, 
    customer_ip inet not null, 
    tech_prefix character varying COLLATE pg_catalog."default" NOT NULL,
    att_level_with_sp_ani character(1) COLLATE pg_catalog."default" NOT NULL,
    att_level_wo_sp_ani character(1) COLLATE pg_catalog."default" NOT NULL,
    PRIMARY KEY(sp_customer_ip_uuid),
    CONSTRAINT fk_sp_customer
	  FOREIGN KEY (sp_customer_uuid)
	  REFERENCES sp_customers(sp_customer_uuid)
	  ON DELETE CASCADE
	  ON UPDATE CASCADE
)
TABLESPACE pg_default;

ALTER TABLE public.sp_customer_ip
    OWNER to postgres;

CREATE TABLE public.sp_accounts
(
    sp_account_uuid uuid default  uuid_generate_v4(),
    active boolean default true, 
    started_on timestamp without time zone not null,
    modified_on timestamp without time zone not null,
    expired_on timestamp without time zone not null,
    cps_limit integer,
    as_enabled boolean default true,
    vs_enabled boolean default true,
    priv_key character varying COLLATE pg_catalog."default" NOT NULL,
    pem_file_path character varying COLLATE pg_catalog."default" NOT NULL,
    default_att_level_with_sp_ani character(1) COLLATE pg_catalog."default" NOT NULL,
    default_att_level_wo_sp_ani character(1) COLLATE pg_catalog."default" NOT NULL,
    sp_name character varying COLLATE pg_catalog."default" NOT NULL,
    priv_phase character varying COLLATE pg_catalog."default",
    PRIMARY KEY(sp_account_uuid)
)
TABLESPACE pg_default;

ALTER TABLE public.sp_accounts
    OWNER to postgres;


CREATE TABLE public.sp_gateway_ip
(
    sp_gateway_ip_uuid uuid default uuid_generate_v4(),
    sp_account_uuid uuid not null,
    gateway_ip inet not null ,
	PRIMARY KEY(sp_gateway_ip_uuid),
	CONSTRAINT fk_sp_account
	  FOREIGN KEY (sp_account_uuid)
	  REFERENCES sp_accounts(sp_account_uuid)
	  ON DELETE CASCADE
	  ON UPDATE CASCADE

)
TABLESPACE pg_default;

ALTER TABLE public.sp_gateway_ip
    OWNER to postgres;

CREATE TABLE public.sp_tn
(
    sp_tn_uuid uuid default uuid_generate_v4(),
    sp_account_uuid uuid, 
    tn character varying COLLATE pg_catalog."default" NOT NULL,
    PRIMARY KEY(sp_tn_uuid),
    CONSTRAINT fk_sp_account
	  FOREIGN KEY (sp_account_uuid)
	  REFERENCES sp_accounts(sp_account_uuid)
	  ON DELETE CASCADE
	  ON UPDATE CASCADE
)
TABLESPACE pg_default;

ALTER TABLE public.sp_tn
    OWNER to postgres;

