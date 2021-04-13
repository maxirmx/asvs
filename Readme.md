# Authentication and Verification Server 

Authorization and verfication server  command line parameters:
    -asvs_api_http_ip (AS&VS HTTP API IP/Hostname) type: string  default: "localhost"
    -asvs_api_http_port (AS&VS HTTP API port) type: int32 default: 8081
    -postgres (PostgresSql connection string ("user=postgres host=localhost port=5432 dbname=urls_remap password=changeme")) type: string default: ""
    -prov_api_http_ip (Provisioning API IP/Hostname) type: string  default: "localhost"
    -prov_api_http_port (Provisioning API port) type: int32 default: 8080
    -threads (Number of threads used by AS&VS HTTP API. Zero means to default to the number of cores on this machine.) type: int32 default: 0

