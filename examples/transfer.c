#include <stdio.h>

#include "cclient/api/core/core_api.h"
#include "cclient/api/extended/send_transfer.h"
#include "common/trinary/flex_trit.h"

#define IRI_HOST "node.deviceproof.org"
#define IRI_PORT 14265
#define SEED                                                                   \
  "AMRWQP9BUMJALJHBXUCHOD9HFFD9LGTGEAWMJWWXSDVOF9PI9YGJAPBQLQUOMNYEQCZPGCTHGV" \
  "NNAPGHA"
#define SECURITY 2
#define DEPTH 3
#define MWM 14

void service_init(iota_client_service_t *service) {
  service->http.path = "/";
  service->http.content_type = "application/json";
  service->http.accept = "application/json";
  service->http.host = IRI_HOST;
  service->http.port = IRI_PORT;
  service->http.api_version = 1;
  service->serializer_type = SR_JSON;
}

int main(int ac, char **av) {
  iota_client_service_t service;
  service_init(&service);


  transfer_array_t *transfers = transfer_array_new();
  bundle_transactions_t *out_tx_objs;
  flex_trit_t seed[NUM_FLEX_TRITS_ADDRESS];
  flex_trits_from_trytes(seed, NUM_FLEX_TRITS_ADDRESS, (tryte_t *)SEED,
                         NUM_TRYTES_ADDRESS, NUM_TRYTES_ADDRESS);
  iota_client_send_transfer(&service, seed, SECURITY, DEPTH, MWM, false, transfers, NULL, NULL, NULL, out_tx_objs);
}