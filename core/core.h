/*
 * Copyright (c) 2018 IOTA Stiftung
 * https:github.com/iotaledger/entangled
 *
 * MAM is based on an original implementation & specification by apmi.bsu.by
 * [ITSec Lab]
 *
 * Refer to the LICENSE file for licensing information
 */

#ifndef CORE_H__
#define CORE_H__

#include "cclient/api/core/core_api.h"
#include "cclient/api/extended/extended_api.h"
#include "common/trinary/trit_tryte.h"
#include "common/trinary/tryte_ascii.h"
#include "mam/api/api.h"

#define SEED                                                                   \
  "AARWQP9BUMJALJHBXUCHOD9HFFD9LGTGEAWMJWWXSDVOF9PI9YGJAPBQLQUOMNYEQCZPGCTHGV" \
  "NNAPGHA"
#define MSS_DEPTH 1

#ifdef __cplusplus
extern "C" {
#endif

find_transactions_req_t recv_example_req;

extern mam_psk_t const psk;

void init_client_service(iota_client_service_t *const serv,
                         char const *const host, uint16_t const port);

retcode_t mam_channel_create_tool(mam_api_t *const api,
                                  tryte_t *const channel_id);

retcode_t send_bundle(char const *const host, uint16_t const port,
                      bundle_transactions_t *const bundle);
retcode_t receive_bundle(char const *const host, uint16_t const port,
                         tryte_t const *const bundle_hash,
                         bundle_transactions_t *const bundle);

#ifdef __cplusplus
}
#endif

#endif  // CORE_H__
