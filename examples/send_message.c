#include <stdio.h>

#include "client/send_client.h"
#include "core/send_core.h"

int main(int ac, char **av) {
  mam_api_t api;
  bundle_transactions_t *bundle = NULL;
  tryte_t channel_id[MAM_CHANNEL_ID_TRYTE_SIZE];
  retcode_t ret = RC_OK;
  char *payload = av[1];

  if (ac != 2) {
    fprintf(stderr, "Please provide `payload`.\n");
    return EXIT_FAILURE;
  }

  ret = init_mam_send_objs(&api, &bundle, channel_id);
  if (ret) {
    return EXIT_FAILURE;
  }

  ret = send_mam_msg(&api, bundle, channel_id, payload);
  if (ret) {
    return EXIT_FAILURE;
  }

  ret = destroy_mam_send_objs(&api, &bundle);
  if (ret) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
