#include <stdio.h>

#include "recv_client.h"

retcode_t init_mam_recv_objs(mam_api_t *api, tryte_t *bundle_hash) {
  retcode_t ret = RC_OK;
  if ((ret = mam_api_init(api, bundle_hash)) != RC_OK) {
    fprintf(stderr, "mam_api_init failed with err %d\n", ret);
  }

  return ret;
}

retcode_t recv_mam_msg(mam_api_t *api, bundle_transactions_t *bundle,
                       tryte_t *bundle_hash, tryte_t *chid) {
  retcode_t ret = RC_OK;
  tryte_t *payload_trytes = NULL;
  size_t payload_size = 0;
  bool is_last_packet;

  if ((ret = receive_bundle(HOST, PORT, bundle_hash, bundle)) != RC_OK) {
    return ret;
  }

  mam_psk_t_set_add(&api->psks, &psk);
  if (chid) {
    mam_api_add_trusted_channel_pk(api, chid);
  }

  if (mam_api_bundle_read(api, bundle, &payload_trytes, &payload_size,
                          &is_last_packet) == RC_OK) {
    if (payload_trytes == NULL || payload_size == 0) {
      fprintf(stderr, "No payload\n");
    } else {
      char *payload = calloc(payload_size * 2 + 1, sizeof(char));

      trytes_to_ascii(payload_trytes, payload_size, payload);
      fprintf(stderr, "Payload: %s\n", payload);
      free(payload);
    }
  } else {
    fprintf(stderr, "mam_api_bundle_read_msg failed\n");
  }

  return ret;
}

retcode_t destroy_mam_recv_objs(mam_api_t *api,
                                bundle_transactions_t **bundle) {
  retcode_t ret = RC_OK;
  // Destroying MAM API
  if ((ret = mam_api_destroy(api)) != RC_OK) {
    fprintf(stderr, "mam_api_destroy failed with err %d\n", ret);
    return ret;
  }

  // Cleanup
  bundle_transactions_free(bundle);

  return RC_OK;
}
