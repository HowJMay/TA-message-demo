#include <stdio.h>

#include "client/send_client.h"
#include "core/send_core.h"
#define HOST "node.deviceproof.org"
#define PORT 14265

int main(int ac, char **av) {
  mam_api_t api;
  bundle_transactions_t *bundle = NULL;
  tryte_t channel_id[MAM_CHANNEL_ID_TRYTE_SIZE];
  tryte_t endpoint_id[MAM_ENDPOINT_ID_TRYTE_SIZE];
  retcode_t ret = RC_OK;

  if (ac != 3) {
    fprintf(stderr, "Please provide `payload`, `NTRU key file path`.\n");
    return EXIT_FAILURE;
  }
  char *payload = av[1];
  char *file_path = av[2];
  FILE *fptr;
  trit_t key[MAM_NTRU_PK_SIZE];
  if ((fptr = fopen(file_path, "rb")) == NULL) {
    printf("Error! opening file");
    return EXIT_FAILURE;
  }
  fread(key, sizeof(key), 1, fptr);
  ret = init_mam_send_objs(&api, &bundle, key, channel_id, endpoint_id);
  if (ret) {
    return EXIT_FAILURE;
  }

  ret =
      send_mam_msg(HOST, PORT, &api, bundle, channel_id, endpoint_id, payload);
  if (ret) {
    return EXIT_FAILURE;
  }

  ret = destroy_mam_send_objs(&api, &bundle);
  if (ret) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
