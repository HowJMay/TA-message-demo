#ifndef SEND_CLIENT_H_
#define SEND_CLIENT_H_

#include "core/send_core.h"

retcode_t init_mam_send_objs(mam_api_t *api, bundle_transactions_t **bundle,
                             tryte_t *channel_id);
retcode_t send_mam_msg(mam_api_t *api, bundle_transactions_t *bundle,
                       tryte_t *channel_id, char *payload);
retcode_t destroy_mam_send_objs(mam_api_t *api, bundle_transactions_t **bundle);

#endif  // SEND_CLIENT_H_