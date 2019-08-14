#include <stdio.h>

#include "recv_client.h"
static mam_api_t *api = NULL;

static retcode_t recv_mam_msg(char *host, int port,
                              bundle_transactions_t *bundle,
                              tryte_t *bundle_hash, tryte_t *chid,
                              tryte_t *epid) {
  retcode_t ret = RC_OK;
  tryte_t *payload_trytes = NULL;
  size_t payload_size = 0;
  bool is_last_packet;

  if ((ret = receive_bundle(host, port, bundle_hash, bundle)) != RC_OK) {
    return ret;
  }

  mam_psk_t_set_add(&api->psks, &psk);
  if (chid) {
    mam_api_add_trusted_channel_pk(api, chid);
  }
  if (epid) {
    mam_api_add_trusted_endpoint_pk(api, epid);
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

static retcode_t destroy_mam_recv_objs(bundle_transactions_t **bundle) {
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

retcode_t init_mam_recv_objs() {
  retcode_t ret = RC_OK;
  api = (mam_api_t *)malloc(sizeof(mam_api_t));

  if ((ret = mam_api_init(api, (tryte_t *)SEED)) != RC_OK) {
    fprintf(stderr, "mam_api_init failed with err %d\n", ret);
  }

  return ret;
}

retcode_t gen_keypair(char *nonce_str) {
  retcode_t ret = RC_OK;

  // Generating a keypair for the receiver api
  mam_ntru_sk_t *ntru = (mam_ntru_sk_t *)malloc(sizeof(mam_ntru_sk_t));
  MAM_TRITS_DEF(nonce, 3 * NONCE_LEN);
  nonce = MAM_TRITS_INIT(nonce, 3 * NONCE_LEN);
  trits_from_str(nonce, nonce_str);
  ntru_sk_reset(ntru);
  ntru_sk_gen(ntru, &api->prng, nonce);

  // Setting the generated keypair to the receiver API
  ERR_BIND_RETURN(mam_api_add_ntru_sk(api, ntru), ret);

  flex_trit_t flex_pk[MAM_NTRU_PK_SIZE];
  tryte_t tryte_pk[MAM_NTRU_PK_SIZE / 3];
  flex_trits_from_trits(flex_pk, MAM_NTRU_PK_SIZE, ntru->public_key.key,
                        MAM_NTRU_PK_SIZE, MAM_NTRU_PK_SIZE);
  flex_trits_to_trytes(tryte_pk, MAM_NTRU_PK_SIZE / 3, flex_pk,
                       MAM_NTRU_PK_SIZE, MAM_NTRU_PK_SIZE);
  fprintf(stderr, "NTRU public key: ");
  for (size_t i = 0; i < MAM_NTRU_PK_SIZE; i++) {
    fprintf(stderr, "%c", tryte_pk[i]);
  }
  fprintf(stderr, "\n");

  return ret;
}

retcode_t extra_recv_msg(char *host, int port, tryte_t *bundle_hash,
                         tryte_t *chid, tryte_t *epid) {
  retcode_t ret = RC_OK;
  bundle_transactions_t *bundle = NULL;
  bundle_transactions_new(&bundle);

  if (!api) {
    ret = init_mam_recv_objs();
    if (ret) {
      return EXIT_FAILURE;
    }
  }

  ret = recv_mam_msg(host, port, bundle, bundle_hash, chid, epid);
  if (ret) {
    return EXIT_FAILURE;
  }

  ret = destroy_mam_recv_objs(&bundle);
  if (ret) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

retcode_t extra_recv_msg_chid(char *host, int port, tryte_t *chid,
                              tryte_t *epid) {
  retcode_t ret = RC_OK;
  tryte_t bundle_hash[NUM_TRYTES_BUNDLE];
  bundle_transactions_t *bundle = NULL;
  bundle_transactions_new(&bundle);

  if (!api) {
    ret = init_mam_recv_objs();
    if (ret) {
      goto done;
    }
  }

  find_transactions_req_t *txn_req = find_transactions_req_new();
  find_transactions_res_t *txn_res = find_transactions_res_new();
  transaction_array_t *obj_res = transaction_array_new();

  if (txn_req == NULL || txn_res == NULL || obj_res == NULL) {
    ret = RC_OOM;
    goto done;
  }

  iota_client_service_t serv;
  init_client_service(&serv, host, port);

  flex_trit_t flex_chid[NUM_FLEX_TRITS_ADDRESS];
  flex_trits_from_trytes(flex_chid, NUM_FLEX_TRITS_ADDRESS, chid,
                         NUM_TRYTES_ADDRESS, NUM_TRYTES_ADDRESS);
  find_transactions_req_address_add(txn_req, chid);

  ret = iota_client_find_transaction_objects(&serv, txn_req, obj_res);
  if (ret != RC_OK) {
    goto done;
  }

  mam_psk_t_set_add(&api->psks, &psk);
  if (chid) {
    ret = mam_api_add_trusted_channel_pk(api, chid);
    if (ret) {
      goto done;
    }
  }
  if (epid) {
    ret = mam_api_add_trusted_endpoint_pk(api, epid);
    if (ret) {
      goto done;
    }
  }

  iota_transaction_t *curr_tx = NULL;
  TX_OBJS_FOREACH(obj_res, curr_tx) {
    flex_trits_to_trytes(bundle_hash, NUM_TRYTES_BUNDLE,
                         transaction_bundle(curr_tx), NUM_TRITS_BUNDLE,
                         NUM_TRITS_BUNDLE);
    ret = extra_recv_msg(host, port, bundle_hash, chid, epid);
    if (ret) {
      goto done;
    }
  }

  ret = destroy_mam_recv_objs(&bundle);
  if (ret) {
    goto done;
  }

done:
  find_transactions_req_free(&txn_req);
  find_transactions_res_free(&txn_res);
  transaction_array_free(&obj_res);
  iota_client_core_destroy(&serv);
  return ret;
}
