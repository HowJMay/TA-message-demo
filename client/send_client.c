#include <stdio.h>

#include "send_client.h"

retcode_t init_mam_send_objs(mam_api_t *api, bundle_transactions_t **bundle,
                             trit_t *ntru_pb_key, tryte_t *channel_id,
                             tryte_t *endpoint_id) {
  retcode_t ret = RC_OK;

  // Loading or creating MAM API
  if ((ret = mam_api_init(api, (tryte_t *)SEED)) != RC_OK) {
    fprintf(stderr, "mam_api_init failed with err %d\n", ret);
    return ret;
  }

  // Adding the receiver APIs public key to the sender API public key list
  mam_ntru_pk_t ntru_pk;
  memcpy(ntru_pk.key, ntru_pb_key, NTRU_PB_KEY_LEN_TRIT);

  ERR_BIND_RETURN(mam_api_add_ntru_pk(api, &ntru_pk), ret);

  // Creating channel
  if ((ret = mam_channel_create_tool(api, channel_id)) != RC_OK) {
    fprintf(stderr, "mam_channel_create_tool failed with err %d\n", ret);
    return ret;
  }

  ERR_BIND_RETURN(
      mam_api_endpoint_create(api, MSS_DEPTH, channel_id, endpoint_id), ret);
  fprintf(stderr, "Endpoint: ");
  for (size_t i = 0; i < FLEX_TRIT_SIZE_243; i++) {
    fprintf(stderr, "%c", endpoint_id[i]);
  }
  fprintf(stderr, "\n");
  bundle_transactions_new(bundle);

  return RC_OK;
}

retcode_t send_mam_msg(char *host, int port, mam_api_t *api,
                       bundle_transactions_t *bundle, tryte_t *channel_id,
                       tryte_t *endpoint_id, char *payload) {
  retcode_t ret = RC_OK;
  trit_t msg_id[MAM_MSG_ID_SIZE];

  // Writing header to bundle
  if ((ret = mam_write_header_on_channel_tool(api, channel_id, endpoint_id,
                                              bundle, msg_id)) != RC_OK) {
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
  if ((ret = send_bundle(host, port, bundle)) != RC_OK) {
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
