#include <stdio.h>

#include "client/recv_client.h"
#define NONCE "BCDEFGHIJK"
int main(int ac, char **av) {
  mam_api_t api;
  int ret = EXIT_SUCCESS;

  tryte_t bundle_hash[NUM_TRYTES_ADDRESS];
  tryte_t chid[NUM_TRYTES_ADDRESS];
  tryte_t epid[NUM_TRYTES_ADDRESS];
  mam_ntru_sk_t *ntru = NULL;
  bundle_transactions_t *bundle = NULL;
  bundle_transactions_new(&bundle);

  ret = init_mam_recv_objs(&api, bundle_hash);
  if (ret) {
    return EXIT_FAILURE;
  }

  ret = gen_keypair(&api, NONCE, ntru);
  if (ret) {
    return EXIT_FAILURE;
  }

  printf("Input bundle hash:");
  fgets((char *)bundle_hash, NUM_TRYTES_ADDRESS + 2, stdin);
  printf("Input chid:");
  fgets((char *)chid, NUM_TRYTES_ADDRESS + 2, stdin);
  printf("Input epid:");
  fgets((char *)epid, NUM_TRYTES_ADDRESS + 2, stdin);

  ret = recv_mam_msg(&api, bundle, bundle_hash, chid, epid);
  if (ret) {
    return EXIT_FAILURE;
  }

  ret = destroy_mam_recv_objs(&api, &bundle);
  if (ret) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
