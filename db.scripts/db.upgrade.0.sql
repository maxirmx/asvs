ALTER TABLE public.sp_customers
    ADD CONSTRAINT sp_customers_pkey PRIMARY KEY (sp_customer_uuid);

ALTER TABLE public.sp_customer_ip
    ADD CONSTRAINT sp_customer_ip_pkey PRIMARY KEY (sp_customer_ip_uuid);

ALTER TABLE public.sp_customer_ip
    ADD CONSTRAINT fk_sp_customer FOREIGN KEY (sp_customer_uuid)
    REFERENCES public.sp_customers (sp_customer_uuid) MATCH SIMPLE
    ON UPDATE CASCADE
    ON DELETE CASCADE;

ALTER TABLE public.sp_accounts
    ADD CONSTRAINT sp_accounts_pkey PRIMARY KEY (sp_account_uuid);

ALTER TABLE public.sp_gateway_ip
    ADD CONSTRAINT sp_gateway_ip_pkey PRIMARY KEY (sp_gateway_ip_uuid);

ALTER TABLE public.sp_gateway_ip
    ADD CONSTRAINT fk_sp_account FOREIGN KEY (sp_account_uuid)
    REFERENCES public.sp_accounts (sp_account_uuid) MATCH SIMPLE
    ON UPDATE CASCADE
    ON DELETE CASCADE;

ALTER TABLE public.sp_tn
    ADD CONSTRAINT sp_tn_pkey PRIMARY KEY (sp_tn_uuid);

ALTER TABLE public.sp_tn
    ADD CONSTRAINT fk_sp_account FOREIGN KEY (sp_account_uuid)
    REFERENCES public.sp_accounts (sp_account_uuid) MATCH SIMPLE
    ON UPDATE CASCADE
    ON DELETE CASCADE;

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
