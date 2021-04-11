-- Authentication and Verification Server
-- Upgrade #2 script 
-- "Break out priv_key character varying COLLATE pg_catalog."default" NOT NULL,
--           pem_file_path character varying COLLATE pg_catalog."default" NOT NULL,
-- and put in sp_cert table
-- because each service provider may have multiple certificate""


CREATE TABLE public.sp_cert
(
    sp_cert_uuid uuid default uuid_generate_v4(),
    sp_account_uuid uuid, 
    priv_key character varying COLLATE pg_catalog."default" NOT NULL,
    pem_file_path character varying COLLATE pg_catalog."default" NOT NULL,
    PRIMARY KEY(sp_cert_uuid),
    CONSTRAINT fk_sp_account
	  FOREIGN KEY (sp_account_uuid)
	  REFERENCES sp_accounts(sp_account_uuid)
	  ON DELETE CASCADE
	  ON UPDATE CASCADE
)
TABLESPACE pg_default;

ALTER TABLE public.sp_cert
    OWNER to postgres;


ALTER TABLE public.sp_accounts 
    DROP COLUMN priv_key;

ALTER TABLE public.sp_accounts 
    DROP COLUMN pem_file_path;

INSERT INTO __db_schema_version (version_number, created_on) VALUES  (2, now());
