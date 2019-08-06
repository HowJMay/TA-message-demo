#include <stdio.h>

#include "send_client.h"

retcode_t init_mam_send_objs(mam_api_t *api, bundle_transactions_t **bundle,
                             tryte_t *channel_id) {
  retcode_t ret = RC_OK;

  // Loading or creating MAM API
  if ((ret = mam_api_init(api, (tryte_t *)SEED)) != RC_OK) {
    fprintf(stderr, "mam_api_init failed with err %d\n", ret);
    return ret;
  }

  // Creating channel
  if ((ret = mam_channel_create_tool(api, channel_id)) != RC_OK) {
    fprintf(stderr, "mam_channel_create_tool failed with err %d\n", ret);
    return ret;
  }

  bundle_transactions_new(bundle);

  return RC_OK;
}

retcode_t send_mam_msg(mam_api_t *api, bundle_transactions_t *bundle,
                       tryte_t *channel_id, char *payload) {
  retcode_t ret = RC_OK;
  trit_t msg_id[MAM_MSG_ID_SIZE];

  // Writing header to bundle
  if ((ret = mam_write_header_on_channel_tool(api, channel_id, bundle,
                                              msg_id)) != RC_OK) {
    fprintf(stderr, "mam_write_header_on_channel_tool failed with err %d\n",
            ret);
    return ret;
  }

  if ((ret = mam_write_packet_tool(api, bundle, payload, msg_id, true)) !=
      RC_OK) {
    fprintf(stderr, "mam_write_packet_tool failed with err %d\n", ret);
    return ret;
  }

  // Sending bundle
  if ((ret = send_bundle(HOST, PORT, bundle)) != RC_OK) {
    fprintf(stderr, "send_bundle failed with err %d\n", ret);
    return ret;
  }

  return RC_OK;
}

retcode_t destroy_mam_send_objs(mam_api_t *api,
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

/*
int main(int ac, char **av) {
  mam_api_t api;
  bundle_transactions_t *bundle = NULL;
  tryte_t channel_id[MAM_CHANNEL_ID_TRYTE_SIZE];
  retcode_t ret = RC_OK;
  char *payload = av[1];

  if (ac != 1) {
    fprintf(stderr, "Please provide `payload`.\n");
    return ret;
  }


  return RC_OK;
}
 */
