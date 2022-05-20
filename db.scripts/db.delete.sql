-- Authentication and Verification Server
-- Data destruction script

DELETE FROM public.sp_accounts;
DELETE FROM public.sp_customer_ip;
DELETE FROM public.sp_customers;
DELETE FROM public.sp_gateway_ip;
DELETE FROM public.sp_tn;
DELETE FROM public.sp_cert;
