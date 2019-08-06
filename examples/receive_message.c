#include <stdio.h>

#include "client/recv_client.h"

int main(int ac, char **av) {
  mam_api_t api;
  int ret = EXIT_SUCCESS;

  tryte_t *bundle_hash = NULL;
  tryte_t *chid = NULL;
  bundle_transactions_t *bundle = NULL;

  bundle_transactions_new(&bundle);

  if (ac < 2 || ac > 3) {
    fprintf(stderr, "usage: recv <bundle> <chid> (optional)\n");
    return EXIT_FAILURE;
  }
  bundle_hash = av[1];
  if (ac == 3) {
    chid = av[2];
  }

  init_mam_recv_objs(&api, bundle_hash);

  recv_mam_msg(&api, bundle, bundle_hash, chid);

  destroy_mam_recv_objs(&api, &bundle);

  return ret;
}
