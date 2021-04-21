@echo .. signing
curl -v --header "Content-Type: application/json" --header "X-RequestID: 051ddf12-0001-46dd-b46c-59c1036fae75"  http://localhost:8081/stir/v1/signing
curl -v --header "Content-Type: application/json" --data  "{ \"uuid\" : \"97092d99-e001-4438-9fc3-adabdd3c255c\"}" http://localhost:8081/stir/v1/bad-api
curl -v --header "Content-Type: application/marc" --data  "{ \"uuid\" : \"97092d99-e001-4438-9fc3-adabdd3c255c\"}" http://localhost:8081/stir/v1/signing


    