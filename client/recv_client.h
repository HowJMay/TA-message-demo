#ifndef RECV_CLIENT_H_
#define RECV_CLIENT_H_

#include "core/core.h"

retcode_t init_mam_recv_objs(mam_api_t *api, tryte_t *bundle_hash);
retcode_t recv_mam_msg(mam_api_t *api, bundle_transactions_t *bundle,
                       tryte_t *bundle_hash, tryte_t *chid);
retcode_t destroy_mam_recv_objs(mam_api_t *api, bundle_transactions_t **bundle);

#endif  // RECV_CLIENT_H_