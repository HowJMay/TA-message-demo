#include <stdio.h>

#include "client/recv_client.h"
int main(int ac, char **av) {
  retcode_t ret = RC_OK;
  if (ac != 2) {
    fprintf(stderr, "Please provide `file_path`.\n");
    return EXIT_FAILURE;
  }
  char *file_path = av[1];

  FILE *fp = fopen(file_path, "wb");  // read mode

  mam_api_t *api = (mam_api_t *)malloc(sizeof(mam_api_t));

  if ((ret = mam_api_init(api, (tryte_t *)SEED)) != RC_OK) {
    fprintf(stderr, "mam_api_init failed with err %d\n", ret);
    return EXIT_FAILURE;
  }

  // Generating a keypair for the receiver api
  mam_ntru_sk_t *ntru = (mam_ntru_sk_t *)malloc(sizeof(mam_ntru_sk_t));
  MAM_TRITS_DEF(nonce, 3 * NTRU_NONCE_LEN);
  nonce = MAM_TRITS_INIT(nonce, 3 * NTRU_NONCE_LEN);
  trits_from_str(nonce, NTRU_NONCE);
  ntru_sk_reset(ntru);
  ntru_sk_gen(ntru, &api->prng, nonce);

  // Setting the generated keypair to the receiver API
  // ERR_BIND_RETURN(mam_api_add_ntru_sk(api, ntru), ret);

  fwrite(ntru->public_key.key, sizeof(ntru->public_key.key), 1, fp);
  fclose(fp);
  free(ntru);

  if ((ret = mam_api_destroy(api)) != RC_OK) {
    fprintf(stderr, "mam_api_destroy failed with err %d\n", ret);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}