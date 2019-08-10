#ifndef SEND_CLIENT_H_
#define SEND_CLIENT_H_

#include "core/send_core.h"
#define NTRU_PB_KEY_LEN_TRYTE 10
#define NTRU_PB_KEY_LEN_TRIT 30

retcode_t init_mam_send_objs(mam_api_t *api, bundle_transactions_t **bundle,
                             char *ntru_pb_key, tryte_t *channel_id,
                             tryte_t *endpoint_id);
retcode_t send_mam_msg(mam_api_t *api, bundle_transactions_t *bundle,
                       tryte_t *channel_id, tryte_t *endpoint_id,
                       char *payload);
retcode_t destroy_mam_send_objs(mam_api_t *api, bundle_transactions_t **bundle);

#endif  // SEND_CLIENT_H_