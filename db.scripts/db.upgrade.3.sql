-- Authentication and Verification Server
-- Upgrade #3 script 


DROP TABLE IF EXISTS public.sp_cert;

ALTER TABLE public.cert 
   RENAME TO public.sp_cert;

ALTER TABLE public.sp_cert
   RENAME COLUMN uuid to sp_cert_uuid;

ALTER TABLE public.sp_cert
   ADD COLUMN is_default BOOLEAN DEFAULT false;

INSERT INTO __db_schema_version (version_number, created_on) VALUES  (3, now());
